Gateway microservice

This gateway implements a fan-in → single-exit pattern for smart-meter readings.

Quick start (local with docker-compose):

1. Build & run
   docker-compose up --build

2. Send a reading
   curl -X POST http://localhost:5000/readings -H "Content-Type: application/json" -d '{"meter_id":"m1","reading_kwh":12.3}'

Config via env:
- RABBIT_URL, EXCHANGE_NAME, ROUTING_KEY
- HTTP_PORT, UDP_PORT, QUEUE_SIZE, AUTH_TOKEN

## Testing

Unit tests and a lightweight integration fallback are included under `gateway/tests/`.

- Run all gateway tests (unit + integration fallback):

  ```powershell
  Set-Location 'C:\Users\adaiya\Univeristy_Project_Year_3\ddes-smart-meter-app'
  python -m pip install -r gateway/requirements.txt
  python -m pytest gateway -q
  ```

- Run only unit tests:

  ```powershell
  python -m pytest gateway/tests/test_validation.py gateway/tests/test_publisher.py -q
  ```

- Run the real integration test locally (requires Docker):

  ```powershell
  # start a real RabbitMQ via docker-compose (from repo root)
  docker compose -f gateway/test-docker-compose.yml up -d
  python -m pytest gateway/tests/test_integration_rabbit.py -q
  docker compose -f gateway/test-docker-compose.yml down
  ```

Notes
- If Docker is not available the integration test uses an in-memory fake-broker fallback so tests remain fast and runnable on machines without Docker.
- CI: a GitHub Actions workflow (`.github/workflows/integration.yml`) runs the real integration test against a RabbitMQ service container on push/PR to `main`.

