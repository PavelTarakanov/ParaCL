#include <gtest/gtest.h>
#include "../include/node.hpp"

TEST(NodeTest, ExecutionContextSetAndGet) {
    // Assign
    paracl::ExecutionContext ctx;

    // Act
    ctx.set_variable(0, 42);
    ctx.set_variable(5, 100);

    // Assert
    EXPECT_EQ(ctx.get_variable(0), 42);
    EXPECT_EQ(ctx.get_variable(5), 100);
}

TEST(NodeTest, ExecutionContextOutOfBoundsThrows) {
    // Assign
    paracl::ExecutionContext ctx;

    // Act & Assert
    EXPECT_THROW(ctx.get_variable(0), std::runtime_error);
}
