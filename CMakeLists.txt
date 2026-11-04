cmake_minimum_required(VERSION 3.14)
project(paracl_project CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB SRC_FILES "src/*.cpp")
add_executable(paracl_app ${SRC_FILES})

add_executable(paracl_tests
    tests/test_node.cpp
    tests/test_leaf_nodes.cpp
    tests/test_operator_nodes.cpp
    tests/test_control_nodes.cpp
)

target_link_libraries(paracl_tests
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(paracl_tests)
