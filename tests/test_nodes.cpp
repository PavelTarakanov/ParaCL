#include <gtest/gtest.h>
#include "node.hpp"
#include "leaf_nodes.hpp"
#include "operator_nodes.hpp"
#include "control_nodes.hpp"

using namespace paracl;

TEST(NodesTest, DivisionByZero) {
    ExecutionContext ctx;
    BinaryOpNode div_node(BinaryOpType::Div,
                         std::make_unique<NumberNode>(10),
                         std::make_unique<NumberNode>(0));

    EXPECT_THROW(div_node.eval(ctx), std::runtime_error);
}

TEST(NodesTest, ModuloByZero) {
    ExecutionContext ctx;
    BinaryOpNode mod_node(BinaryOpType::Mod,
                         std::make_unique<NumberNode>(10),
                         std::make_unique<NumberNode>(0));

    EXPECT_THROW(mod_node.eval(ctx), std::runtime_error);
}

TEST(NodesTest, VariableOutOfBounds) {
    ExecutionContext ctx;
    EXPECT_THROW(ctx.get_variable(99), std::runtime_error);
}

TEST(NodesTest, DotDumpGeneration) {
    BlockNode block;
    block.add_statement(std::make_unique<AssignNode>(0, std::make_unique<NumberNode>(42)));

    std::ostringstream out;
    int next_id = 0;
    block.dump_dot(out, next_id);

    std::string dot_output = out.str();
    EXPECT_NE(dot_output.find("Block"), std::string::npos);
    EXPECT_NE(dot_output.find("Assign"), std::string::npos);
    EXPECT_NE(dot_output.find("Number: 42"), std::string::npos);
}
