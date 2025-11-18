# ddes-smart-meter-app

## Client

 Auth over TLS (mTLS or token).

 Random intervals 15–60s (enforced) with realistic kWh deltas.

 Auto-send readings; no manual input.

 Live bill view updates after each send.

 Detect & display comms errors; retry with backoff.

## Server

 Handle ≥12 concurrent clients.

 Receive, verify, and store client info + readings.

 Compute live bills; push to clients.

 Log comms errors.

 Broadcast grid alerts.

 Initialize each client with existing readings on startup.

## Alerts

Need to add the documentation for this bit.

## Ingestion

Handle up to 1024 individual clients, this can be increased if needed.
Verify HMAC checksums - checksum incorporates the client id, reading and unix timestamp.
Validate client fw version.
Idempotency on client billing updates.
Publishing full json object as received to events topic with routing key _billing_
Logs to stdout but can be changed if needed, on two levels: info and error.
RabbitMQ config located in ingestion.conf, if updating, change the file in src/ as this is copied to build/ at compile time.
HMAC key and expected client fw is in the .env file, although this is not pushed to git. If changing, change src/ingestion.env as this is copied to build/ at compile time.
HMAC key/JSON schema/client fw storage is subject to change once config maps/secret mgmnt systems are ready.

## Project & submission

 GitHub repo with visible contributions from all members; build/run instructions.

 AITS declaration added to repo/appendix.

 Group contract submitted.

 Video ≤30 min; code+arch explained; equal speaking time.

## Gatway Service

Fan-in to single-exit pattern for smart-meter readings Gateway microservice. 
This gateway implements a fan-in → single-exit pattern for smart-meter readings.  
Passes unit tests and integration tests  
If Docker is not available the integration test uses an in-memory fake-broker fallback so tests remain fast and runnable on machines without Docker.
