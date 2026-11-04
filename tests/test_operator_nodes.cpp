#include <gtest/gtest.h>
#include <sstream>
#include "../include/node.hpp"
#include "../include/leaf_nodes.hpp"
#include "../include/operator_nodes.hpp"

TEST(OperatorNodesTest, BinaryOpAddition) {
    // Assign
    paracl::ExecutionContext ctx;
    auto left = std::make_unique<paracl::NumberNode>(10);
    auto right = std::make_unique<paracl::NumberNode>(20);
    paracl::BinaryOpNode add(paracl::BinaryOpType::Add, std::move(left), std::move(right));

    // Act
    int result = add.eval(ctx);

    // Assert
    EXPECT_EQ(result, 30);
}

TEST(OperatorNodesTest, BinaryOpDivisionByZeroThrows) {
    // Assign
    paracl::ExecutionContext ctx;
    auto left = std::make_unique<paracl::NumberNode>(10);
    auto right = std::make_unique<paracl::NumberNode>(0);
    paracl::BinaryOpNode div(paracl::BinaryOpType::Div, std::move(left), std::move(right));

    // Act & Assert
    EXPECT_THROW(div.eval(ctx), std::runtime_error);
}

TEST(OperatorNodesTest, AssignNodeEvaluation) {
    // Assign
    paracl::ExecutionContext ctx;
    auto expr = std::make_unique<paracl::NumberNode>(99);
    paracl::AssignNode assign(1, std::move(expr));

    // Act
    int result = assign.eval(ctx);

    // Assert
    EXPECT_EQ(result, 99);
    EXPECT_EQ(ctx.get_variable(1), 99);
}

TEST(OperatorNodesTest, ReadNodeEvaluation) {
    // Assign
    paracl::ExecutionContext ctx;
    std::stringstream input("777");
    std::cin.rdbuf(input.rdbuf());
    paracl::ReadNode read;

    // Act
    int result = read.eval(ctx);

    // Assert
    EXPECT_EQ(result, 777);
}
