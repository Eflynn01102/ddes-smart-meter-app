# ddes-smart-meter-app
# Client

 Auth over TLS (mTLS or token).

 Random intervals 15–60s (enforced) with realistic kWh deltas.

 Auto-send readings; no manual input.

 Live bill view updates after each send.

 Detect & display comms errors; retry with backoff.

# Server

 Handle ≥12 concurrent clients.

 Receive, verify, and store client info + readings.

 Compute live bills; push to clients.

 Log comms errors.

 Broadcast grid alerts.

 Initialize each client with existing readings on startup.

# Project & submission

 GitHub repo with visible contributions from all members; build/run instructions.

 AITS declaration added to repo/appendix.

 Group contract submitted.

 Video ≤30 min; code+arch explained; equal speaking time.
