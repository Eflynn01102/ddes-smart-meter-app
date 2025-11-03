from __future__ import annotations
import asyncio
import json
import logging
from validation import validate_reading
from settings import UDP_PORT, AUTH_TOKEN

logger = logging.getLogger("gateway.udp")


class ReadingDatagramProtocol(asyncio.DatagramProtocol):
    def __init__(self, enqueue_fn):
        self.enqueue = enqueue_fn

    def datagram_received(self, data: bytes, addr):
        try:
            payload = json.loads(data.decode())
        except Exception:
            logger.warning("Invalid JSON from %s", addr)
            return
        r, err = validate_reading(payload, require_auth=bool(AUTH_TOKEN), expected_token=AUTH_TOKEN)
        if err:
            logger.warning("Invalid reading from %s: %s", addr, err)
            return
        norm = r.normalize()
        body = json.dumps(norm).encode()
        ok = self.enqueue(body, None)
        if not ok:
            logger.error("Queue full, dropping UDP message from %s", addr)


async def start_udp_server(enqueue_fn, loop=None):
    loop = loop or asyncio.get_event_loop()
    transport, proto = await loop.create_datagram_endpoint(lambda: ReadingDatagramProtocol(enqueue_fn), local_addr=("0.0.0.0", UDP_PORT))
    logger.info("UDP server listening on %d", UDP_PORT)
    return transport, proto
