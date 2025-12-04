# Build Instructions - DDES Smart Meter Application

Simple guide to build and deploy the complete smart meter application using Docker.

## Prerequisites

- **Docker** (version 20.10 or higher)
- **Docker Compose** (version 2.0 or higher)

That's it! Everything else runs inside containers.

---

## Quick Start

### 1. Clone the Repository

```bash
git clone <repository-url>
cd ddes-smart-meter-app
```

### 2. Build All Docker Images

Run these commands from the project root:

```bash
# Build Billing Service
cd billing-service
docker build -t billing:latest .
cd ..

# Build Client UI
cd client
docker build -f Dockerfile.client -t client-ui:latest .
cd ..

# Build Client Server
cd client
docker build -f Dockerfile.server -t client-server:latest .
cd ..

# Build Ingestion Service
cd ingestion
docker build -t ingestion:latest .
cd ..

# Build Gateway Service
cd gateway
docker build -t gateway:latest .
cd ..
```

### 3. Deploy the Application

```bash
cd deployment
docker compose up -d
```

### 4. Verify Services are Running

```bash
docker compose ps
```

All services should show as "running".

---

## Access the Application

Once deployed, access the following services:

- **UI**: http://localhost:80
- **Billing API**: http://localhost:8080
- **Gateway HTTP**: http://localhost:5000
- **Gateway UDP**: localhost:9002
- **RabbitMQ Management**: http://localhost:15672 (username: `guest`, password: `guest`)

---

## Managing the Application

### View Logs

```bash
cd deployment

# View all logs
docker compose logs -f

# View specific service logs
docker compose logs -f billing
docker compose logs -f gateway
docker compose logs -f ingestion
docker compose logs -f client
docker compose logs -f ui
```

### Stop the Application

```bash
cd deployment
docker compose down
```

### Stop and Remove All Data

```bash
cd deployment
docker compose down -v
```

### Restart a Single Service

```bash
cd deployment
docker compose restart billing
```

---

## Rebuilding Services

If you make changes to any service, rebuild and restart it:

```bash
# Example: Rebuilding the billing service
cd billing-service
docker build -t billing:latest .
cd ../deployment
docker compose up -d billing
```

---

## Troubleshooting

### Services Won't Start

**Check if ports are already in use:**
```bash
lsof -i :80    # UI
lsof -i :8080  # Billing
lsof -i :5000  # Gateway
lsof -i :5672  # RabbitMQ
```

**Solution:** Stop conflicting services or modify ports in `deployment/docker-compose.yml`

### Build Failures

**Clean Docker cache and rebuild:**
```bash
docker system prune -a
# Then rebuild the failing service
```

### RabbitMQ Connection Issues

**Check RabbitMQ is running:**
```bash
cd deployment
docker compose logs rabbitmq
```

**Restart RabbitMQ:**
```bash
docker compose restart rabbitmq
```

### View Service Health

```bash
cd deployment
docker compose ps
docker compose logs <service-name>
```

---

## Configuration

Default environment variables are configured in `deployment/docker-compose.yml`:

- **HMAC/HMACKEY**: `00deb39aaf33c17977e0c858148dca5483703f554da146637b35914772cae5b2`
- **CLIENTFW**: `1.0.0`
- **RabbitMQ Host**: `rabbitmq-service`

Modify these values in the docker-compose.yml file if needed.

---

## Complete Deployment Script

Copy and paste this entire script to build and deploy everything:

```bash
#!/bin/bash

# Navigate to project root
cd ddes-smart-meter-app

# Build all images
echo "Building Billing Service..."
cd billing-service && docker build -t billing:latest . && cd ..

echo "Building Client UI..."
cd client && docker build -f Dockerfile.client -t client-ui:latest . && cd ..

echo "Building Client Server..."
cd client && docker build -f Dockerfile.server -t client-server:latest . && cd ..

echo "Building Ingestion Service..."
cd ingestion && docker build -t ingestion:latest . && cd ..

echo "Building Gateway Service..."
cd gateway && docker build -t gateway:latest . && cd ..

# Deploy
echo "Deploying application..."
cd deployment
docker compose up -d

echo "Deployment complete!"
echo "Access the UI at: http://localhost:80"
echo "Access the API at: http://localhost:8080"
echo "Access RabbitMQ Management at: http://localhost:15672"
```
