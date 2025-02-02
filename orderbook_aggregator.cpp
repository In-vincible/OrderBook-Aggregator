#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <memory>
#include <stdexcept>
#include "exchanges.cpp"

class OrderBookAggregator {
private:
    vector<shared_ptr<Exchange>> exchanges;

public:
    OrderBookAggregator(vector<shared_ptr<Exchange>> ex) : exchanges(move(ex)) {}

    OrderBook aggregateOrderBooks() {
        OrderBook merged;
        for (const auto& exchange : exchanges) {
            try {
                json data = exchange->fetchOrderBook();
                OrderBook book = exchange->normalizeOrderBook(data);

                // Debug
                cout << "Order book from " << exchange->getExchangeName() << endl;
                book.printOrderBook();

                // Efficient Merging using maps:
                for (const auto& [price, quantity] : book.bids) {
                    merged.addBid(price, quantity); // Uses the map's efficient update
                }

                for (const auto& [price, quantity] : book.asks) {
                    merged.addAsk(price, quantity); // Uses the map's efficient update
                }


            } catch (const exception& e) {
                cerr << "Error aggregating order books: " << e.what() << endl;
            }
        }

        cout << "Merged order book:" << endl;
        merged.printOrderBook();

        return merged;
    }
};

int main(int argc, char* argv[]) {
    try {

        // Default quantity
        double quantity = 10;

        // Check if exactly one argument is provided (quantity) [first argument is the program name]
        if (argc > 2) { 
            throw runtime_error("Error: Exactly one argument is required. (Quantity)");
        } else if (argc == 2) {
            string argument = argv[1];
            quantity = stod(argument);
        }
        

        cout << fixed << setprecision(6);
        vector<shared_ptr<Exchange>> exchanges = {make_shared<CoinbaseExchange>(), 
                                                  make_shared<GeminiExchange>(), 
                                                  make_shared<KrakenExchange>(),
                                                  make_shared<BinanceExchange>()};
        OrderBookAggregator aggregator(exchanges);
        OrderBook mergedBook = aggregator.aggregateOrderBooks();

        double buyPrice = mergedBook.getBestPrice(quantity, true);
        double sellPrice = mergedBook.getBestPrice(quantity, false);        
        
        cout << "Price to buy " << quantity << " BTC: " 
             << (buyPrice > 0 ? to_string(buyPrice) : "insufficient liquidity") << endl;
        cout << "Price to sell " << quantity << " BTC: "
             << (sellPrice > 0 ? to_string(sellPrice) : "insufficient liquidity") << endl;
             
    } catch (const exception& e) {
        cerr << "Error in main: " << e.what() << endl;
        return 1;
    }
    return 0;
}