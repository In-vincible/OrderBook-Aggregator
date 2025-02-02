#include <functional>
#include <map>
#include <iostream>
#include "double_utils.h"

using namespace std;

struct Order {
    double price;
    double quantity;
    Order(double p, double q) : price(p), quantity(q) {}
    
    bool operator<(const Order& other) const {
        return price < other.price;  // Min heap for asks, max heap for bids
    }
    
    bool operator>(const Order& other) const {
        return price > other.price;
    }
};

class OrderBook {
private:
    using BidMap = map<double, double, greater<double>>; // Price -> Quantity (Descending for bids)
    using AskMap = map<double, double>; // Price -> Quantity (Ascending for asks)

public:
    BidMap bids;
    AskMap asks;

    void addBid(double price, double quantity) {
        bids[price] += quantity; // Map handles updates efficiently
    }

    void addAsk(double price, double quantity) {
        asks[price] += quantity; // Map handles updates efficiently
    }

    int getBidDepth() const {
        return bids.size(); // Number of bid levels
    }

    int getAskDepth() const {
        return asks.size(); // Number of ask levels
    }

    double getTotalBidLiquidity(int levels = -1) const {
        double totalLiquidity = 0;
        int count = 0;
        for (const auto& [price, quantity] : bids) {
            totalLiquidity += quantity;
            count++;
            if (levels != -1 && count >= levels) {
                break;
            }
        }
        return totalLiquidity;
    }

    double getTotalAskLiquidity(int levels = -1) const {
        double totalLiquidity = 0;
        int count = 0;
        for (const auto& [price, quantity] : asks) {
            totalLiquidity += quantity;
            count++;
            if (levels != -1 && count >= levels) {
                break;
            }
        }
        return totalLiquidity;
    }

    template <typename MapType>
    double calculatePriceFromMap(MapType& book, double quantity) {
        double totalQty = 0;
        double totalCost = 0;
        double remainingQuantity = quantity;

        for (auto it = book.begin(); it != book.end() && remainingQuantity > 0; ++it) {
            double price = it->first;
            double availableQty = it->second;
            double qtyToTake = min(availableQty, remainingQuantity);

            totalCost += qtyToTake * price;
            remainingQuantity -= qtyToTake;
            totalQty += qtyToTake;
        }

        if (!doubleEquals(totalQty, quantity)) {  
            return -1.0; // Insufficient liquidity
        } else {
            return totalCost / quantity;
        }
    }


    double getBestPrice(double quantity, bool isBuy) {
        return isBuy ? calculatePriceFromMap(asks, quantity) : calculatePriceFromMap(bids, quantity);
    }

    double getSpread() const {
        double bestBid = bids.empty() ? 0 : bids.begin()->first;
        double bestAsk = asks.empty() ? 0 : asks.begin()->first;
        return bestAsk - bestBid;
    }

    void printOrderBook(int levels = 2) const {
        cout << "Bids (Top " << levels << "):" << endl;
        int i = 0;
        for (auto it = bids.begin(); it != bids.end() && i < levels; ++it, ++i) {
            cout << it->first << ": " << it->second << endl;
        }

        cout << "Asks (Top " << levels << "):" << endl;
        i = 0;
        for (auto it = asks.begin(); it != asks.end() && i < levels; ++it, ++i) {
            cout << it->first << ": " << it->second << endl;
        }

        cout << "Bid Depth: " << getBidDepth() << endl;
        cout << "Ask Depth: " << getAskDepth() << endl;
        cout << "Total Bid Liquidity: " << getTotalBidLiquidity() << endl;
        cout << "Total Ask Liquidity: " << getTotalAskLiquidity() << endl << endl;
    }

};