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

add_executable(paracl_app
    src/main.cpp
    src/lexer.cpp
    src/parser.cpp
)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.tar.gz
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(paracl_tests
    tests/test_node.cpp
    tests/test_leaf_nodes.cpp
    tests/test_operator_nodes.cpp
    tests/test_control_nodes.cpp
)

target_link_libraries(paracl_tests PRIVATE GTest::gtest_main)

add_test(NAME ParaCLUnitTests COMMAND paracl_tests)
