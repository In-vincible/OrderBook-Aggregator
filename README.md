# Order Book Aggregator

## Build and Run Commands

### **Build the Docker Image**

```sh
docker build -t orderbook-aggregator .
```

### **Run the Container**

(Without quantity arg - default: 10)
```sh
docker run --rm orderbook-aggregator
```

(With quantity arg)
```sh
docker run --rm orderbook-aggregator 100
```

## Exchanges Supported

1. Coinbase
2. Gemini
3. Kraken (bonus)
4. Binance (bonus)

## File Structure

- `orderbook_aggregator.cpp` - The main C++ source file implementing the order book aggregator.
- `exchanges.cpp` - Exchange implementations for fetching orderbook.
- `order_book.cpp` - Order, Orderbook implementations.
- `rest_client.h` - REST interface for handling get/post requests.
- `double_utils.h` - To handle double precision issues while comparison.
- `CMakeLists.txt` - CMake file for building the project.
- `Dockerfile` - Container setup to run project across platforms.

## Dependencies

Tested on `Docker version 27.4.0` (Should work fine in any recent docker installation)

The program uses:

- `nlohmann/json` for parsing JSON data.
- `libcurl` for making API requests to cryptocurrency exchanges.
- `CMake` for building the project.

Ensure all dependencies are installed if running outside Docker.

## Project assumptions and limitations

1. Double precision has been handled with a shortcut (check double_utils.h) keeping in mind the scope of the problem. (We need better precision handling in a potential Trading System)
2. Orderbook merging - Orderbook merging is done simply by adding liquidity across exchanges, this is a complex problem as liquidity available across exchanges aren't sequentially available in real-life.
3. Marketdata delays - Due marketdata delays in public endpoint, bid/ask crosses b/w exchanges for eg. Gemini orderbook was observerd during testing to cross others, which basically leads to asks at the top lower than top bids sometimes.
4. Logging - Haven't solved logging appropriatly in order to keep the implementation time constrained in few hours. (Once could use boost and other libs for that)

