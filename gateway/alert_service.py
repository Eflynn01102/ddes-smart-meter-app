import requests
from settings import ALERT_URL, HELLO_WORLD_URL

def send_alert(message: str, title: str, severity: str):
    body = {
        "clientID": "client-131",
        "title": title,
        "severity": severity,
        "message": message
    }
    res= requests.post(ALERT_URL, json=body)
    return res.status_code, res.json()


send_alert("Alert Service is up and running", "Service Start", "info")
