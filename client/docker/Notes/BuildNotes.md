# Dockerization notes

Docker files need to be named for the docker compose.

## UI

``` bash
docker build -t client-ui -f docker/Dockerfile.client .
```

## Server

```bash
docker build -t client-server -f docker/Dockerfile.server .
```
