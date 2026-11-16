cmake_minimum_required(VERSION 3.20)
project(ParaCL CXX)

# Require C++23 standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

enable_testing()

include_directories(
    include
    src
    .
)

# --- Main Application Executable ---
add_executable(paracl_app
    src/main.cpp
    src/lexer.cpp
    src/parser.cpp
)

# --- Google Test Framework via FetchContent ---
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.tar.gz
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# --- Unit Tests Executable ---
add_executable(paracl_tests
    src/lexer.cpp
    src/parser.cpp
    tests/test_lexer.cpp
    tests/test_parser.cpp
    tests/test_nodes.cpp
)

target_link_libraries(paracl_tests PRIVATE GTest::gtest_main)
add_test(NAME ParaCLUnitTests COMMAND paracl_tests)

# --- E2E Integration Tests (Python) ---
find_package(Python3 REQUIRED)

add_test(
    NAME ParaCLE2ETests
    COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/tests/full_test.py
)
# Ensure unit tests pass before running E2E integration tests
set_tests_properties(ParaCLE2ETests PROPERTIES DEPENDS ParaCLUnitTests)
