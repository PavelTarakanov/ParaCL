#include <gtest/gtest.h>
#include "../include/lexer.hpp"
#include "../include/parser.hpp"
#include <sstream>

using namespace paracl;

TEST(ParserTest, OperatorPrecedence) {
    // a = 2 + 3 * 4; -> 14
    std::string source = "a = 2 + 3 * 4;";
    Lexer lexer(source);
    Parser parser(lexer.tokenize());
    auto ast = parser.parse();

    ExecutionContext ctx;
    ast->eval(ctx);
    EXPECT_EQ(ctx.get_variable(0), 14);
}

TEST(ParserTest, ParenthesesPrecedence) {
    // a = (2 + 3) * 4; -> 20
    std::string source = "a = (2 + 3) * 4;";
    Lexer lexer(source);
    Parser parser(lexer.tokenize());
    auto ast = parser.parse();

    ExecutionContext ctx;
    ast->eval(ctx);
    EXPECT_EQ(ctx.get_variable(0), 20);
}

TEST(ParserTest, WhileAndIfExecution) {
    std::string source = R"(
        i = 0;
        sum = 0;
        while (i < 5) {
            if (i % 2 == 0) {
                sum = sum + i;
            }
            i = i + 1;
        }
    )";
    Lexer lexer(source);
    Parser parser(lexer.tokenize());
    auto ast = parser.parse();

    ExecutionContext ctx;
    ast->eval(ctx);
    // 0 + 2 + 4 = 6
    EXPECT_EQ(ctx.get_variable(1), 6);
}

TEST(ParserTest, SyntaxErrorMissingSemicolon) {
    std::string source = "a = 5";
    Lexer lexer(source);
    Parser parser(lexer.tokenize());
    EXPECT_THROW(parser.parse(), std::runtime_error);
}
