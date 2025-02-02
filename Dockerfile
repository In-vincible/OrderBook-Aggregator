FROM ubuntu:20.04

# Install dependencies
RUN apt-get update && apt-get install -y \
    g++ \
    cmake \
    make \
    curl \
    libcurl4-openssl-dev \
    libssl-dev \
    nlohmann-json3-dev \
    && rm -rf /var/lib/apt/lists/*

# Set working directory
WORKDIR /app

# Copy source code
COPY . .

# Build the application
RUN mkdir -p build && cd build \
    && cmake .. \
    && make

# Command to run the application
# CMD ["/app/build/OrderBookAggregator"]
ENTRYPOINT ["/app/build/OrderBookAggregator"]
CMD []