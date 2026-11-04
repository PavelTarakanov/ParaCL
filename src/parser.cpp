#include "../include/parser.hpp"
#include <stdexcept>
#include <utility>

namespace paracl {

Parser::Parser(std::vector<Token> tokens)
    : tokens_(std::move(tokens)), pos_(0), next_var_id_(0) {}

const Token& Parser::peek() const {
    if (pos_ >= tokens_.size()) {
        return tokens_.back(); // Return EndOfFile token
    }
    return tokens_[pos_];
}

Token Parser::get() {
    if (pos_ >= tokens_.size()) {
        return tokens_.back();
    }
    return tokens_[pos_++];
}

bool Parser::match(TokenType type) {
    if (peek().type == type) {
        get();
        return true;
    }
    return false;
}

Token Parser::consume(TokenType type, const std::string& error_message) {
    if (peek().type == type) {
        return get();
    }
    throw std::runtime_error("Parser error: " + error_message + ", got '" + peek().text + "'");
}

bool Parser::is_at_end() const {
    return peek().type == TokenType::EndOfFile;
}

std::size_t Parser::get_or_create_variable(const std::string& name) {
    auto it = symbol_table_.find(name);
    if (it != symbol_table_.end()) {
        return it->second;
    }
    std::size_t new_id = next_var_id_++;
    symbol_table_[name] = new_id;
    return new_id;
}

NodePtr Parser::parse() {
    return parse_program();
}

NodePtr Parser::parse_program() {
    auto block = std::make_unique<BlockNode>();
    while (!is_at_end()) {
        block->add_statement(parse_statement());
    }
    return block;
}

NodePtr Parser::parse_statement() {
    // 1. If statement
    if (match(TokenType::KeywordIf)) {
        return parse_if();
    }
    // 2. While loop
    if (match(TokenType::KeywordWhile)) {
        return parse_while();
    }
    // 3. Print statement
    if (match(TokenType::KeywordPrint)) {
        return parse_print();
    }
    // 4. Block of statements { ... }
    if (peek().type == TokenType::OpenBrace) {
        return parse_block();
    }

    // 5. Assignment (identifier = expr;) or expression statement
    if (peek().type == TokenType::Identifier) {
        // Look ahead to check if next token is '='
        if (pos_ + 1 < tokens_.size() && tokens_[pos_ + 1].type == TokenType::Assign) {
            Token name_token = get(); // consume identifier
            get(); // consume '='
            NodePtr expr = parse_equality();
            consume(TokenType::Semicolon, "Expected ';' after assignment");
            std::size_t var_id = get_or_create_variable(name_token.text);
            return std::make_unique<AssignNode>(var_id, std::move(expr));
        }
    }

    // Default: expression followed by a semicolon
    NodePtr expr = parse_equality();
    consume(TokenType::Semicolon, "Expected ';' after expression");
    return expr;
}

NodePtr Parser::parse_block() {
    consume(TokenType::OpenBrace, "Expected '{' to start block");
    auto block = std::make_unique<BlockNode>();
    while (!is_at_end() && peek().type != TokenType::CloseBrace) {
        block->add_statement(parse_statement());
    }
    consume(TokenType::CloseBrace, "Expected '}' to close block");
    return block;
}

NodePtr Parser::parse_if() {
    consume(TokenType::OpenParen, "Expected '(' after 'if'");
    NodePtr cond = parse_equality();
    consume(TokenType::CloseParen, "Expected ')' after condition");

    NodePtr then_branch = parse_statement();
    NodePtr else_branch = nullptr;

    if (match(TokenType::KeywordElse)) {
        else_branch = parse_statement();
    }

    return std::make_unique<IfNode>(std::move(cond), std::move(then_branch), std::move(else_branch));
}

NodePtr Parser::parse_while() {
    consume(TokenType::OpenParen, "Expected '(' after 'while'");
    NodePtr cond = parse_equality();
    consume(TokenType::CloseParen, "Expected ')' after condition");

    NodePtr body = parse_statement();
    return std::make_unique<WhileNode>(std::move(cond), std::move(body));
}

NodePtr Parser::parse_print() {
    NodePtr expr = parse_equality();
    consume(TokenType::Semicolon, "Expected ';' after print");
    return std::make_unique<PrintNode>(std::move(expr));
}

// --- Expression Parsing (Precedence climbing) ---

NodePtr Parser::parse_equality() {
    NodePtr node = parse_comparison();
    while (peek().type == TokenType::Equal || peek().type == TokenType::NotEqual) {
        Token op_token = get();
        NodePtr right = parse_comparison();
        BinaryOpType op_type = (op_token.type == TokenType::Equal) ? BinaryOpType::Equal : BinaryOpType::NotEqual;
        node = std::make_unique<BinaryOpNode>(op_type, std::move(node), std::move(right));
    }
    return node;
}

NodePtr Parser::parse_comparison() {
    NodePtr node = parse_term();
    while (peek().type == TokenType::Less || peek().type == TokenType::Greater ||
           peek().type == TokenType::LessEq || peek().type == TokenType::GreaterEq) {
        Token op_token = get();
        NodePtr right = parse_term();
        BinaryOpType op_type;
        if (op_token.type == TokenType::Less) op_type = BinaryOpType::Less;
        else if (op_token.type == TokenType::Greater) op_type = BinaryOpType::Greater;
        else if (op_token.type == TokenType::LessEq) op_type = BinaryOpType::LessEq;
        else op_type = BinaryOpType::GreaterEq;

        node = std::make_unique<BinaryOpNode>(op_type, std::move(node), std::move(right));
    }
    return node;
}

NodePtr Parser::parse_term() {
    NodePtr node = parse_factor();
    while (peek().type == TokenType::Plus || peek().type == TokenType::Minus) {
        Token op_token = get();
        NodePtr right = parse_factor();
        BinaryOpType op_type = (op_token.type == TokenType::Plus) ? BinaryOpType::Add : BinaryOpType::Sub;
        node = std::make_unique<BinaryOpNode>(op_type, std::move(node), std::move(right));
    }
    return node;
}

NodePtr Parser::parse_factor() {
    NodePtr node = parse_primary();
    while (peek().type == TokenType::Mul || peek().type == TokenType::Div || peek().type == TokenType::Mod) {
        Token op_token = get();
        NodePtr right = parse_primary();
        BinaryOpType op_type;
        if (op_token.type == TokenType::Mul) op_type = BinaryOpType::Mul;
        else if (op_token.type == TokenType::Div) op_type = BinaryOpType::Div;
        else op_type = BinaryOpType::Mod;

        node = std::make_unique<BinaryOpNode>(op_type, std::move(node), std::move(right));
    }
    return node;
}

NodePtr Parser::parse_primary() {
    if (match(TokenType::Number)) {
        Token token = tokens_[pos_ - 1];
        return std::make_unique<NumberNode>(token.int_value);
    }
    if (match(TokenType::Identifier)) {
        Token token = tokens_[pos_ - 1];
        std::size_t var_id = get_or_create_variable(token.text);
        return std::make_unique<VariableNode>(var_id);
    }
    if (match(TokenType::Question)) {
        return std::make_unique<ReadNode>();
    }
    if (match(TokenType::OpenParen)) {
        NodePtr node = parse_equality();
        consume(TokenType::CloseParen, "Expected ')' after expression");
        return node;
    }
    throw std::runtime_error("Parser error: unexpected token '" + peek().text + "'");
}

} // namespace paracl
