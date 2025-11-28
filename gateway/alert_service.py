import json
import logging
import threading
from typing import Any, Tuple
from urllib import request, error



from settings import ALERT_URL, HELLO_WORLD_URL

logger = logging.getLogger(__name__)


def _http_post(url: str, body: dict[str,Any], timeout: float) -> Tuple[int, Any]:
        data= json.dumps(body).encode()
        req=request.Request(url, data=data, headers={"Content-Type":"application/json"})
        with request.urlopen(req, timeout=timeout) as resp:
             raw=resp.read()
        try:
             payload = json.loads(raw.decode()) if raw else {}
        
        except ValueError:
             payload = raw.decode()
        return resp.getcode(), payload


def _post_alert(body: dict[str,Any], timeout: float) -> Tuple[int | None, Any]:
     try:
          return _http_post(ALERT_URL, body, timeout)
     except error.URLError as exc:
          logger.warning("Failed to send alert to %s: %s", body.get("title", "<unknown>"), exc)
          return None, None
     


def send_alert(
        message: str,
        title: str,
        severity: str,
        timeout: float = 2.0,
        async_send : bool = True,
    ) -> Tuple[int | None, Any | None]:

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
