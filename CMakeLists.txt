cmake_minimum_required(VERSION 3.15)
project(sentiment)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable all warnings
if (MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Add include directory
include_directories(${PROJECT_SOURCE_DIR}/include)

# Add the executable
add_executable(sentiment 
    src/main.cpp
    src/DSString.cpp
    src/SentimentClassifier.cpp
)

# Copy data directory to build directory
file(COPY ${PROJECT_SOURCE_DIR}/data DESTINATION ${CMAKE_BINARY_DIR})

# Add tests executable
add_executable(tests
    tests/DSStringTest.cpp
    src/DSString.cpp
) 