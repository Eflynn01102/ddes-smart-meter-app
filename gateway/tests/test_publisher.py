import threading
import json
from gateway.publisher import Publisher
from queue import Queue


class DummyChannel:
    def basic_publish(self, exchange, routing_key, body, properties, mandatory=False):
        # pretend success
        return True


class DummyConn:
    def __init__(self):
        self.closed = False

    def channel(self):
        return DummyChannel()

    def close(self):
        self.closed = True


def test_publish_enqueue(monkeypatch):
    q = Queue(maxsize=10)
    stop = threading.Event()

    p = Publisher(q, stop)

    # monkeypatch connect to use dummy connection
    def dummy_connect(self):
        self._conn = DummyConn()
        self._channel = self._conn.channel()
        self._connected.set()

    monkeypatch.setattr(Publisher, "_connect", dummy_connect)

    p.start()

    assert p.publish(b"{}", None)
    stop.set()
    p.join(timeout=1)
