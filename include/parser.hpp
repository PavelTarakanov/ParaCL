#ifndef PARACL_PARSER_HPP
#define PARACL_PARSER_HPP

#include "lexer.hpp"
#include "node.hpp"
#include "leaf_nodes.hpp"
#include "operator_nodes.hpp"
#include "control_nodes.hpp"

#include <vector>
#include <string>
#include <unordered_map>
#include <memory>

namespace paracl {

class Parser {
private:
    std::vector<Token> tokens_;
    std::size_t pos_;
    std::unordered_map<std::string, std::size_t> symbol_table_;
    std::size_t next_var_id_;

    // Navigation and helper methods
    const Token& peek() const;
    Token get();
    bool match(TokenType type);
    Token consume(TokenType type, const std::string& error_message);
    bool is_at_end() const;

    // Symbol table management
    std::size_t get_or_create_variable(const std::string& name);

    // Grammar rules
    NodePtr parse_program();
    NodePtr parse_statement();
    NodePtr parse_block();
    NodePtr parse_if();
    NodePtr parse_while();
    NodePtr parse_print();

    // Expression parsing (operator precedence hierarchy)
    NodePtr parse_equality();
    NodePtr parse_comparison();
    NodePtr parse_term();
    NodePtr parse_factor();
    NodePtr parse_primary();

public:
    explicit Parser(std::vector<Token> tokens);

    // Main method: parses tokens into the root AST node
    NodePtr parse();
};

} // namespace paracl

#endif // PARACL_PARSER_HPP
