from __future__ import annotations
import threading
import time
import json
import logging
from typing import Any
import pika
from pika.adapters.blocking_connection import BlockingConnection
from pika.exceptions import AMQPError
from queue import Queue, Empty
from .settings import RABBIT_URL, EXCHANGE_NAME, ROUTING_KEY, PUBLISHER_RECONNECT_BASE, PUBLISHER_RECONNECT_MAX, PUBLISHER_CONFIRM

logger = logging.getLogger("gateway.publisher")


class Publisher(threading.Thread):
    """Single owner of RabbitMQ connection and channel. Uses publisher confirms and durable exchange.

    All publishes must go through the publish() method which queues messages locally and the run loop
    sends them to RabbitMQ ensuring only one connection/channel exist.
    """

    def __init__(self, out_queue: Queue, stop_event: threading.Event):
        super().__init__(daemon=True, name="publisher-thread")
        self.out_queue = out_queue
        self.stop_event = stop_event
        self._conn: BlockingConnection | None = None
        self._channel = None
        self._connected = threading.Event()

    def _connect(self):
        backoff = PUBLISHER_RECONNECT_BASE
        while not self.stop_event.is_set():
            try:
                params = pika.URLParameters(RABBIT_URL)
                self._conn = BlockingConnection(params)
                self._channel = self._conn.channel()
                # durable exchange
                self._channel.exchange_declare(exchange=EXCHANGE_NAME, exchange_type="direct", durable=True)
                if PUBLISHER_CONFIRM:
                    self._channel.confirm_delivery()
                self._connected.set()
                logger.info("Connected to RabbitMQ and exchange declared")
                return
            except AMQPError as e:
                logger.exception("RabbitMQ connect failed, retrying in %.1fs: %s", backoff, e)
                time.sleep(backoff)
                backoff = min(backoff * 2, PUBLISHER_RECONNECT_MAX)

    def _close(self):
        try:
            if self._channel:
                self._channel.close()
            if self._conn:
                self._conn.close()
        except Exception:
            pass
        finally:
            self._connected.clear()
            self._conn = None
            self._channel = None

    def publish(self, body: bytes, properties: pika.BasicProperties | None = None) -> bool:
        """Called by other threads to enqueue a message for publishing. Returns True if queued, False if stopping."""
        if self.stop_event.is_set():
            return False
        try:
            self.out_queue.put_nowait((body, properties))
            return True
        except Exception:
            logger.error("Out queue full, rejecting message")
            return False

    def _do_publish(self, body: bytes, properties: pika.BasicProperties | None = None) -> bool:
        try:
            if not self._channel:
                raise RuntimeError("Not connected")
            self._channel.basic_publish(
                exchange=EXCHANGE_NAME,
                routing_key=ROUTING_KEY,
                body=body,
                properties=properties or pika.BasicProperties(content_type="application/json", delivery_mode=2),
                mandatory=False,
            )
            return True
        except Exception as e:
            logger.exception("Publish failed: %s", e)
            return False

    def run(self):
        while not self.stop_event.is_set():
            if not self._connected.is_set():
                self._connect()

            try:
                item = self.out_queue.get(timeout=0.5)
            except Empty:
                continue

            body, properties = item
            ok = self._do_publish(body, properties)
            if not ok:
                # on publish error, try reconnect and requeue
                logger.warning("Publish failed, reconnecting and requeueing")
                self._close()
                # requeue at head by putting back (simple approach: put with blocking)
                try:
                    self.out_queue.put((body, properties), timeout=1)
                except Exception:
                    logger.error("Failed to requeue message; dropping")
                time.sleep(0.5)
            else:
                self.out_queue.task_done()

        # flush remaining
        logger.info("Publisher stopping, flushing remaining messages up to timeout")
        start = time.time()
        while not self.out_queue.empty() and (time.time() - start) < 5:
            try:
                body, properties = self.out_queue.get_nowait()
            except Empty:
                break
            try:
                self._do_publish(body, properties)
                self.out_queue.task_done()
            except Exception:
                break
        self._close()
        logger.info("Publisher stopped")
