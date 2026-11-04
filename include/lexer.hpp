#ifndef PARACL_LEXER_HPP
#define PARACL_LEXER_HPP

#include <string>
#include <vector>

namespace paracl {

// Types of lexemes (tokens) supported by our ParaCL language
enum class TokenType {
    // Keywords
    KeywordWhile,
    KeywordIf,
    KeywordElse,
    KeywordPrint,

    // Identifiers and literals
    Identifier,     // Variable names (e.g., "fst", "iters")
    Number,         // Numeric constants (e.g., "42")

    // Arithmetic and logical operators
    Plus,           // +
    Minus,          // -
    Mul,            // *
    Div,            // /
    Mod,            // %
    Assign,         // =
    Equal,          // ==
    NotEqual,       // !=
    Less,           // <
    Greater,        // >
    LessEq,         // <=
    GreaterEq,      // >=

    // Delimiters and special signs
    Semicolon,      // ;
    OpenParen,      // (
    CloseParen,     // )
    OpenBrace,      // {
    CloseBrace,     // }
    Question,       // ? (input operator)

    EndOfFile       // End of stream/file
};

// Token structure
struct Token {
    TokenType type;
    std::string text;  // Source text of the token
    int int_value = 0; // Numerical value (if it's a Number)
};

// Lexical analyzer
class Lexer {
private:
    std::string src_;
    std::size_t pos_;

    char peek() const;
    char get();
    bool is_eof() const;

public:
    explicit Lexer(std::string source);

    // Main method: converts the entire source text into a vector of tokens
    std::vector<Token> tokenize();
};

} // namespace paracl

#endif // PARACL_LEXER_HPP
