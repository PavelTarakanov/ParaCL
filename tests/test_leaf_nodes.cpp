#include <gtest/gtest.h>
#include "../include/node.hpp"
#include "../include/leaf_nodes.hpp"

TEST(LeafNodesTest, NumberNodeEvaluation) {
    // Assign
    paracl::ExecutionContext ctx;
    paracl::NumberNode num(123);

    // Act
    int result = num.eval(ctx);

    // Assert
    EXPECT_EQ(result, 123);
}

TEST(LeafNodesTest, VariableNodeEvaluation) {
    // Assign
    paracl::ExecutionContext ctx;
    ctx.set_variable(2, 555);
    paracl::VariableNode var(2);

    // Act
    int result = var.eval(ctx);

    // Assert
    EXPECT_EQ(result, 555);
}
