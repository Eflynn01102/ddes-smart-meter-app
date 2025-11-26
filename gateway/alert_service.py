import logging
import threading
from typing import Any, Tuple

import requests

from requests import RequestException
from settings import ALERT_URL, HELLO_WORLD_URL

logger = logging.getLogger(__name__)


def _post_alert(body: dict[str, Any], timeout: float) -> Tuple[int, None, Any]:
    try:
        res=requests.post(ALERT_URL, json=body, timeout=timeout)
        try:
            payload = res.json()
        except ValueError:
            payload = res.text
        return res.status_code, payload
    except RequestException as exc:
        logger.warning("Failed to send alert '%s':%s", body.get("title", "<unknown>"), exc)
        return None, None


def send_alert(
        message: str, 
        title: str, 
        severity: str, 
        timeout: float = 2.0,
        async_send : bool = True,
    ) -> Tuple[int | None, Any | Any]:

    if not ALERT_URL:
        logger.debug("Alert URL not configured, skipping alert '%s'", title)
        return None, None

    body = {
        "clientID":"client-131",
        "title": title,
        "message": message,
        "severity": severity,
    }

    if async_send:
        threading.Thread(target=_post_alert, args=(body, timeout), daemon=True).start()
        return None, None
        
    
    return _post_alert(body, timeout)
