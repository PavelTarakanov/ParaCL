cmake_minimum_required(VERSION 3.14)
project(ParaCL CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

include_directories(
    include
    src
    .
)

# --- Основное приложение ---
add_executable(paracl_app
    src/main.cpp
    src/lexer.cpp
    src/parser.cpp
)

# --- Google Test via FetchContent ---
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.tar.gz
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# --- Юнит-тесты ---
add_executable(paracl_tests
    src/lexer.cpp
    src/parser.cpp
    tests/test_lexer.cpp
    tests/test_parser.cpp
    tests/test_nodes.cpp
)

target_link_libraries(paracl_tests PRIVATE GTest::gtest_main)
add_test(NAME ParaCLUnitTests COMMAND paracl_tests)
