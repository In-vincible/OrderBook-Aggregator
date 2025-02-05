cmake_minimum_required(VERSION 3.10)

# Set the project name and version
project(OrderBookAggregator VERSION 1.0)

# Set the C++ standard to be used
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find the CURL, nlohmann_json, Threads packages
find_package(CURL REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(Threads REQUIRED)

# Include directories
include_directories(${CURL_INCLUDE_DIRS})

# Add the executable
add_executable(OrderBookAggregator orderbook_aggregator.cpp)

# Link libraries
target_link_libraries(OrderBookAggregator ${CURL_LIBRARIES} nlohmann_json::nlohmann_json Threads::Threads)

