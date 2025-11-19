# Stage 1: Builder
FROM ubuntu:22.04 AS builder

ENV DEBIAN_FRONTEND=noninteractive

# Install build tools and libraries
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    librabbitmq-dev \
    libcurl4-openssl-dev \
    libssl-dev \
    libcjson-dev \
    ca-certificates \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY . .

# Build the project

RUN rm -rf build
WORKDIR /app/build
RUN cmake .. -DCMAKE_BUILD_TYPE=Release
RUN make -j$(nproc)

# Stage 2: Minimal runtime
FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

# Install runtime dependencies
RUN apt-get update && apt-get install -y \
    librabbitmq4 \
    libcurl4-openssl-dev \
    libssl3 \
    libcjson1 \
    ca-certificates

RUN rm -rf /var/lib/apt/lists/*
## Copy the built binary
COPY --from=builder /app/build/ingestion /usr/local/bin/ingestion
#
## Copy env files to /app
COPY --from=builder /app/build/ingestion.conf /usr/local/bin/ingestion.conf
#
WORKDIR /usr/local/bin

CMD ["ingestion"]