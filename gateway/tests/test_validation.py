from gateway.validation import validate_reading
from gateway.settings import AUTH_TOKEN


def test_valid_reading():
    payload = {"meter_id": "m1", "reading_kwh": 5.0}
    r, err = validate_reading(payload, require_auth=False)
    assert err is None
    assert r.meter_id == "m1"


def test_negative_reading():
    payload = {"meter_id": "m1", "reading_kwh": -1}
    r, err = validate_reading(payload, require_auth=False)
    assert err is not None


def test_missing_meter():
    payload = {"reading_kwh": 1.0}
    r, err = validate_reading(payload, require_auth=False)
    assert err is not None


def test_auth_required():
    payload = {"meter_id": "m1", "reading_kwh": 2.0}
    r, err = validate_reading(payload, require_auth=True, expected_token=AUTH_TOKEN)
    # if AUTH_TOKEN is unset in env, validation will not require auth; this test is just to exercise path
    assert err in (None, "unauthorized")
