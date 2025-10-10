# Stage 1: Builder
FROM ubuntu:22.04 AS builder

ENV DEBIAN_FRONTEND=noninteractive

# Install build tools and libraries
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    librabbitmq-dev \
    libssl-dev \
    libcjson-dev \
    ca-certificates \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY . .

# Build the project
RUN rm -rf build && mkdir build && cd build && \
    cmake .. -DCMAKE_BUILD_TYPE=Release && \
    make -j$(nproc)

# Stage 2: Minimal runtime
FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

# Install runtime dependencies
RUN apt-get update && apt-get install -y \
    librabbitmq4 \
    libssl3 \
    libcjson1 \
    ca-certificates \
    && rm -rf /var/lib/apt/lists/*

# Copy the built binary
COPY --from=builder /app/build/ingestion /usr/local/bin/ingestion

# Copy config files to /app
COPY --from=builder /app/build/ingestion.conf /app/ingestion.conf
COPY --from=builder /app/build/ingestion.env /app/ingestion.env

WORKDIR /app
ENTRYPOINT ["/usr/local/bin/ingestion"]