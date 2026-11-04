#include <gtest/gtest.h>
#include "../include/lexer.hpp"
#include <stdexcept>

using namespace paracl;

TEST(LexerTest, KeywordsAndIdentifiers) {
    std::string source = "while if else print my_var _var123";
    Lexer lexer(source);
    auto tokens = lexer.tokenize();

    ASSERT_EQ(tokens.size(), 7); // 6 tokens + EndOfFile
    EXPECT_EQ(tokens[0].type, TokenType::KeywordWhile);
    EXPECT_EQ(tokens[1].type, TokenType::KeywordIf);
    EXPECT_EQ(tokens[2].type, TokenType::KeywordElse);
    EXPECT_EQ(tokens[3].type, TokenType::KeywordPrint);
    EXPECT_EQ(tokens[4].type, TokenType::Identifier);
    EXPECT_EQ(tokens[4].text, "my_var");
    EXPECT_EQ(tokens[5].type, TokenType::Identifier);
    EXPECT_EQ(tokens[5].text, "_var123");
}

TEST(LexerTest, OperatorsAndPunctuation) {
    std::string source = "+ - * / % = == != < <= > >= ; ( ) { } ?";
    Lexer lexer(source);
    auto tokens = lexer.tokenize();

    EXPECT_EQ(tokens[0].type, TokenType::Plus);
    EXPECT_EQ(tokens[1].type, TokenType::Minus);
    EXPECT_EQ(tokens[2].type, TokenType::Mul);
    EXPECT_EQ(tokens[3].type, TokenType::Div);
    EXPECT_EQ(tokens[4].type, TokenType::Mod);
    EXPECT_EQ(tokens[5].type, TokenType::Assign);
    EXPECT_EQ(tokens[6].type, TokenType::Equal);
    EXPECT_EQ(tokens[7].type, TokenType::NotEqual);
    EXPECT_EQ(tokens[8].type, TokenType::Less);
    EXPECT_EQ(tokens[9].type, TokenType::LessEq);
    EXPECT_EQ(tokens[10].type, TokenType::Greater);
    EXPECT_EQ(tokens[11].type, TokenType::GreaterEq);
    EXPECT_EQ(tokens[12].type, TokenType::Semicolon);
    EXPECT_EQ(tokens[13].type, TokenType::OpenParen);
    EXPECT_EQ(tokens[14].type, TokenType::CloseParen);
    EXPECT_EQ(tokens[15].type, TokenType::OpenBrace);
    EXPECT_EQ(tokens[16].type, TokenType::CloseBrace);
    EXPECT_EQ(tokens[17].type, TokenType::Question);
}

TEST(LexerTest, Numbers) {
    std::string source = "0 42 1337";
    Lexer lexer(source);
    auto tokens = lexer.tokenize();

    ASSERT_EQ(tokens.size(), 4);
    EXPECT_EQ(tokens[0].int_value, 0);
    EXPECT_EQ(tokens[1].int_value, 42);
    EXPECT_EQ(tokens[2].int_value, 1337);
}

TEST(LexerTest, UnexpectedCharacter) {
    Lexer lexer("x = @;");
    EXPECT_THROW(lexer.tokenize(), std::runtime_error);
}

TEST(LexerTest, SingleExclamationError) {
    Lexer lexer("x ! y;");
    EXPECT_THROW(lexer.tokenize(), std::runtime_error);
}
