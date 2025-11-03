from __future__ import annotations
import os
from typing import Optional

RABBIT_URL = os.getenv("RABBIT_URL", "amqp://guest:guest@localhost:5672/%2F")
EXCHANGE_NAME = os.getenv("EXCHANGE_NAME", "ddes.readings")
ROUTING_KEY = os.getenv("ROUTING_KEY", "billing")
HTTP_PORT = int(os.getenv("HTTP_PORT", "5000"))
UDP_PORT = int(os.getenv("UDP_PORT", "9002"))
QUEUE_SIZE = int(os.getenv("QUEUE_SIZE", "1024"))
AUTH_TOKEN = os.getenv("AUTH_TOKEN")
LOG_LEVEL = os.getenv("LOG_LEVEL", "INFO")
PUBLISHER_RECONNECT_BASE = float(os.getenv("PUBLISHER_RECONNECT_BASE", "0.5"))
PUBLISHER_RECONNECT_MAX = float(os.getenv("PUBLISHER_RECONNECT_MAX", "30"))
PUBLISHER_CONFIRM = os.getenv("PUBLISHER_CONFIRM", "1") in ("1", "true", "True")

# graceful shutdown
SHUTDOWN_TIMEOUT = int(os.getenv("SHUTDOWN_TIMEOUT", "10"))

# Flask settings
FLASK_DEBUG = os.getenv("FLASK_DEBUG", "0") in ("1", "true", "True")

