#include "order_book.cpp"
#include "rest_client.h"

class Exchange {
public:
    virtual ~Exchange() = default;
    virtual json fetchOrderBook() = 0;
    virtual OrderBook normalizeOrderBook(const json& data) = 0;
    virtual string getExchangeName() const { return "Unknown Exchange"; }
};

class CoinbaseExchange : public Exchange {
public:
    string getExchangeName() const override { return "Coinbase"; }

    json fetchOrderBook() override {
        json response = RestClient::Get("https://api.exchange.coinbase.com/products/BTC-USD/book?level=2");
        return response;
    }

    OrderBook normalizeOrderBook(const json& data) override {
        OrderBook book;
        try {
            if (data.contains("bids")) {
                for (const auto& bid : data["bids"]) {
                    if (bid.is_array() && bid.size() >= 2) {
                        book.addBid(
                            bid[0].get<string>().empty() ? 0.0 : stod(bid[0].get<string>()),
                            bid[1].get<string>().empty() ? 0.0 : stod(bid[1].get<string>())
                        );
                    }
                }
            }
            
            if (data.contains("asks")) {
                for (const auto& ask : data["asks"]) {
                    if (ask.is_array() && ask.size() >= 2) {
                        book.addAsk(
                            ask[0].get<string>().empty() ? 0.0 : stod(ask[0].get<string>()),
                            ask[1].get<string>().empty() ? 0.0 : stod(ask[1].get<string>())
                        );
                    }
                }
            }
        } catch (const exception& e) {
            cerr << "Error normalizing order book: " << e.what() << endl;
        }
        return book;
    }
};

class GeminiExchange : public Exchange {
public:
    string getExchangeName() const override { return "Gemini"; }

    json fetchOrderBook() override {
        json response = RestClient::Get("https://api.gemini.com/v1/book/BTCUSD?limit_bids=0&limit_asks=0");
        return response;
    }

    OrderBook normalizeOrderBook(const json& data) override {
        OrderBook book;
        try {
            if (data.contains("bids")) {
                for (const auto& bid : data["bids"]) {
                    if (bid.is_object() && bid.contains("price") && bid.contains("amount")) {
                        book.addBid(
                            bid["price"].get<string>().empty() ? 0.0 : stod(bid["price"].get<string>()),
                            bid["amount"].get<string>().empty() ? 0.0 : stod(bid["amount"].get<string>())
                        );
                    }
                }
            }

            if (data.contains("asks")) {
                for (const auto& ask : data["asks"]) {
                    if (ask.is_object() && ask.contains("price") && ask.contains("amount")) {
                        book.addAsk(
                            ask["price"].get<string>().empty() ? 0.0 : stod(ask["price"].get<string>()),
                            ask["amount"].get<string>().empty() ? 0.0 : stod(ask["amount"].get<string>())
                        );
                    }
                }
            }
        } catch (const exception& e) {
            cerr << "Error normalizing order book: " << e.what() << endl;
        }
        return book;
    }
};

class KrakenExchange : public Exchange {
public:
    string getExchangeName() const override { return "Kraken"; }

    json fetchOrderBook() override {
        json response = RestClient::Get("https://api.kraken.com/0/public/Depth?pair=XBTUSD&count=500");
        return response;
    }

    OrderBook normalizeOrderBook(const json& data) override {
        OrderBook book;
        try {
            if (data.contains("result") && data["result"].contains("XXBTZUSD")) {  // Corrected currency pair key
                auto pair_data = data["result"]["XXBTZUSD"];

                if (pair_data.contains("bids")) {
                    for (const auto& bid : pair_data["bids"]) {
                        if (bid.is_array() && bid.size() >= 2) {
                            book.addBid(
                                bid[0].get<string>().empty() ? 0.0 : stod(bid[0].get<string>()),
                                bid[1].get<string>().empty() ? 0.0 : stod(bid[1].get<string>())
                            );
                        }
                    }
                }

                if (pair_data.contains("asks")) {
                    for (const auto& ask : pair_data["asks"]) {
                        if (ask.is_array() && ask.size() >= 2) {
                            book.addAsk(
                                ask[0].get<string>().empty() ? 0.0 : stod(ask[0].get<string>()),
                                ask[1].get<string>().empty() ? 0.0 : stod(ask[1].get<string>())
                            );
                        }
                    }
                }
            }
        } catch (const exception& e) {
            cerr << "Error normalizing order book: " << e.what() << endl;
        }
        return book;
    }
};


class BinanceExchange : public Exchange {
public:
    string getExchangeName() const override { return "Binance"; }

    json fetchOrderBook() override {
        json response = RestClient::Get("https://api.binance.com/api/v3/depth?limit=500&symbol=BTCUSDT");
        return response;
    }

    OrderBook normalizeOrderBook(const json& data) override {
        OrderBook book;
        try {
            if (data.contains("bids")) {
                for (const auto& bid : data["bids"]) {
                    if (bid.is_array() && bid.size() >= 2) {
                        book.addBid(
                            bid[0].get<string>().empty() ? 0.0 : stod(bid[0].get<string>()),
                            bid[1].get<string>().empty() ? 0.0 : stod(bid[1].get<string>())
                        );
                    }
                }
            }

            if (data.contains("asks")) {
                for (const auto& ask : data["asks"]) {
                    if (ask.is_array() && ask.size() >= 2) {
                        book.addAsk(
                            ask[0].get<string>().empty() ? 0.0 : stod(ask[0].get<string>()),
                            ask[1].get<string>().empty() ? 0.0 : stod(ask[1].get<string>())
                        );
                    }
                }
            }
        } catch (const exception& e) {
            cerr << "Error normalizing order book: " << e.what() << endl;
        }
        return book;
    }
};