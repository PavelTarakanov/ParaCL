#include <gtest/gtest.h>
#include "../include/node.hpp"
#include "../include/leaf_nodes.hpp"
#include "../include/operator_nodes.hpp"
#include "../include/control_nodes.hpp"

TEST(ControlNodesTest, BlockNodeEvaluation) {
    // Assign
    paracl::ExecutionContext ctx;
    paracl::BlockNode block;
    block.add_statement(std::make_unique<paracl::AssignNode>(0, std::make_unique<paracl::NumberNode>(5)));
    block.add_statement(std::make_unique<paracl::AssignNode>(1, std::make_unique<paracl::NumberNode>(10)));

    // Act
    int result = block.eval(ctx);

    // Assert
    EXPECT_EQ(result, 10); // Блок возвращает результат последней инструкции
    EXPECT_EQ(ctx.get_variable(0), 5);
    EXPECT_EQ(ctx.get_variable(1), 10);
}

TEST(ControlNodesTest, IfNodeTrueBranch) {
    // Assign
    paracl::ExecutionContext ctx;
    auto cond = std::make_unique<paracl::NumberNode>(1); // true
    auto then_b = std::make_unique<paracl::NumberNode>(42);
    auto else_b = std::make_unique<paracl::NumberNode>(24);
    paracl::IfNode if_node(std::move(cond), std::move(then_b), std::move(else_b));

    // Act
    int result = if_node.eval(ctx);

    // Assert
    EXPECT_EQ(result, 42);
}

TEST(ControlNodesTest, IfNodeFalseBranchWithoutElse) {
    // Assign
    paracl::ExecutionContext ctx;
    auto cond = std::make_unique<paracl::NumberNode>(0); // false
    auto then_b = std::make_unique<paracl::NumberNode>(42);
    paracl::IfNode if_node(std::move(cond), std::move(then_b));

    // Act
    int result = if_node.eval(ctx);

    // Assert
    EXPECT_EQ(result, 0);
}

TEST(ControlNodesTest, WhileNodeLoop) {
    // Assign
    paracl::ExecutionContext ctx;
    ctx.set_variable(0, 3); // x = 3

    // Условие: x > 0
    auto cond = std::make_unique<paracl::BinaryOpNode>(
        paracl::BinaryOpType::Greater,
        std::make_unique<paracl::VariableNode>(0),
        std::make_unique<paracl::NumberNode>(0)
    );

    // Тело цикла: x = x - 1
    auto body_block = std::make_unique<paracl::BlockNode>();
    body_block->add_statement(std::make_unique<paracl::AssignNode>(
        0,
        std::make_unique<paracl::BinaryOpNode>(
            paracl::BinaryOpType::Sub,
            std::make_unique<paracl::VariableNode>(0),
            std::make_unique<paracl::NumberNode>(1)
        )
    ));

    paracl::WhileNode while_node(std::move(cond), std::move(body_block));

    // Act
    while_node.eval(ctx);

    // Assert
    EXPECT_EQ(ctx.get_variable(0), 0); // Цикл отработает 3 раза, пока x не станет 0
}
