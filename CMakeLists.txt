cmake_minimum_required(VERSION 3.29)
project(Assignment2_sentiment)

set(CMAKE_CXX_STANDARD 20)

# Copy data files to build directory
file(COPY data/ DESTINATION data/)

# Show compiler output and enable warnings
set(CMAKE_VERBOSE_MAKEFILE ON)
add_compile_options(-Wall -Wextra -pedantic)

# Create the executables
add_executable(Assignment2_sentiment main.cpp DSString.cpp SentimentClassifier.cpp)
add_executable(test_DSString test_DSString.cpp DSString.cpp)

set(CMAKE_VERBOSE_MAKEFILE OFF)
