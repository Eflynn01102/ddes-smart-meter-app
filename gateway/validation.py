from __future__ import annotations
from pydantic import BaseModel, Field, ValidationError, field_validator
from typing import Optional, Dict, Any
from datetime import datetime, timezone
from dateutil import parser as dateparser


class Reading(BaseModel):
    meter_id: str = Field(..., min_length=1)
    reading_kwh: float = Field(..., ge=0)
    ts: Optional[datetime] = None
    auth: Optional[str] = None
    ingested_at: Optional[datetime] = None

    @field_validator("ts", mode="before")
    def parse_ts(cls, v):
        if v is None:
            return None
        if isinstance(v, datetime):
            return v
        try:
            dt = dateparser.parse(v)
            if dt.tzinfo is None:
                dt = dt.replace(tzinfo=timezone.utc)
            return dt
        except Exception:
            raise ValueError("ts must be ISO8601")

    def normalize(self) -> Dict[str, Any]:
        # set ingested_at in UTC
        self.ingested_at = datetime.now(timezone.utc)
        out = {
            "meter_id": self.meter_id,
            "reading_kwh": float(self.reading_kwh),
            "ts": (self.ts.isoformat() if self.ts else None),
            "ingested_at": self.ingested_at.isoformat(),
        }
        if self.auth:
            out["auth"] = self.auth
        return out


def validate_reading(payload: dict, require_auth: bool = False, expected_token: Optional[str] = None):
    try:
        # pydantic v2 preferred entry point
        r = Reading.model_validate(payload)
        if require_auth and expected_token:
            if not r.auth or r.auth != expected_token:
                return None, "unauthorized"
        return r, None
    except ValidationError as e:
        # return structured errors
        try:
            return None, e.errors()
        except Exception:
            return None, str(e)
