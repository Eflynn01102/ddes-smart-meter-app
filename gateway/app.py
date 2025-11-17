from __future__ import annotations
import logging
import logging.config
import signal
import threading
import time
import json
from queue import Queue
from flask import Flask, request, jsonify
from publisher import Publisher
from validation import validate_reading
from settings import HTTP_PORT, QUEUE_SIZE, AUTH_TOKEN, SHUTDOWN_TIMEOUT
import settings
from settings import ALERT_URL, HELLO_WORLD_URL
from alert_service import send_alert

# configure logging
logging.config.fileConfig("logging.yaml")
logger = logging.getLogger("gateway.app")

app = Flask(__name__)

# bounded queue for back-pressure
out_queue = Queue(maxsize=QUEUE_SIZE)
stop_event = threading.Event()

publisher = Publisher(out_queue, stop_event)


@app.route("/healthz", methods=["GET"]) 
def healthz():
    return "ok", 200


@app.route("/readyz", methods=["GET"])
def readyz():
    return ("ready" if publisher._connected.is_set() else "not-ready"), (200 if publisher._connected.is_set() else 503)


@app.route("/metrics", methods=["GET"])
def metrics():
    # stub; prometheus client could be added
    return "# metrics stub\n", 200, {"Content-Type": "text/plain; version=0.0.4"}


@app.route("/readings", methods=["POST"]) 
def readings():
    payload = request.get_json(force=True, silent=True)
    if payload is None:
        send_alert("Payload is None and there is an invalid json-400", "None Payload", "error")
        return jsonify({"error": "invalid json"}), 400

    require_auth = bool(AUTH_TOKEN)
    r, err = validate_reading(payload, require_auth=require_auth, expected_token=AUTH_TOKEN)
    if err:
        send_alert("Unauthorised: Error when validating reading", "Error when validating reading", "error")
        return jsonify({"error": err}), 400 if err != "unauthorized" else 401

    norm = r.normalize()
    body = json.dumps(norm).encode()
    ok = publisher.publish(body, None)
    if not ok:
        send_alert("Server is Busy- 503", "Server Busy-503", "info")
        return jsonify({"error": "server busy"}), 503
    return "", 202


def _graceful_shutdown(signum, frame):
    logger.info("Signal %s received, starting shutdown", signum)
    send_alert("Signal received and starting shutdown", "Signal Recived for shutdown", "info")
    stop_event.set()


def serve():
    publisher.start()

    # start flask in separate thread to allow graceful shutdown
    srv_thread = threading.Thread(target=lambda: app.run(host="0.0.0.0", port=HTTP_PORT, debug=settings.FLASK_DEBUG, use_reloader=False), daemon=True)
    srv_thread.start()

    # wait for termination signal
    signal.signal(signal.SIGINT, _graceful_shutdown)
    signal.signal(signal.SIGTERM, _graceful_shutdown)

    # wait for stop
    try:
        while not stop_event.is_set():
            time.sleep(0.5)
    except KeyboardInterrupt:
        stop_event.set()

    # wait for queue to drain
    logger.info("Waiting for queue to drain up to %ds", SHUTDOWN_TIMEOUT)
    send_alert("Waiting for queue to drain up", "Queue Drain", "info")
    start = time.time()
    while not out_queue.empty() and (time.time() - start) < SHUTDOWN_TIMEOUT:
        time.sleep(0.1)

    logger.info("Shutdown complete")
    send_alert("Shutdown is complete", "Shutdown Completed", "info")



if __name__ == "__main__":
    serve()
