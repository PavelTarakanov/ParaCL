#include "lexer.hpp"
#include <cctype>
#include <stdexcept>
#include <utility>

namespace paracl {

Lexer::Lexer(std::string source) : src_(std::move(source)), pos_(0) {}

char Lexer::peek() const {
    if (is_eof()) return '\0';
    return src_[pos_];
}

char Lexer::get() {
    if (is_eof()) return '\0';
    return src_[pos_++];
}

bool Lexer::is_eof() const {
    return pos_ >= src_.size();
}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;

    while (!is_eof()) {
        char current = peek();

        // 1. Skip spaces, tabs, and newlines
        if (std::isspace(static_cast<unsigned char>(current))) {
            get();
            continue;
        }

        // 2. Recognition of keywords and identifiers (start with a letter or '_')
        if (std::isalpha(static_cast<unsigned char>(current)) || current == '_') {
            std::string lexeme;
            while (std::isalnum(static_cast<unsigned char>(peek())) || peek() == '_') {
                lexeme += get();
            }

            TokenType type = TokenType::Identifier;
            if (lexeme == "while") type = TokenType::KeywordWhile;
            else if (lexeme == "if") type = TokenType::KeywordIf;
            else if (lexeme == "else") type = TokenType::KeywordElse;
            else if (lexeme == "print") type = TokenType::KeywordPrint;

            tokens.push_back({type, lexeme, 0});
            continue;
        }

        // 3. Recognition of numeric literals
        if (std::isdigit(static_cast<unsigned char>(current))) {
            std::string lexeme;
            while (std::isdigit(static_cast<unsigned char>(peek()))) {
                lexeme += get();
            }
            int val = std::stoi(lexeme);
            tokens.push_back({TokenType::Number, lexeme, val});
            continue;
        }

        // 4. Single-character operators and delimiters
        if (current == '+') { get(); tokens.push_back({TokenType::Plus, "+", 0}); continue; }
        if (current == '-') { get(); tokens.push_back({TokenType::Minus, "-", 0}); continue; }
        if (current == '*') { get(); tokens.push_back({TokenType::Mul, "*", 0}); continue; }
        if (current == '/') { get(); tokens.push_back({TokenType::Div, "/", 0}); continue; }
        if (current == '%') { get(); tokens.push_back({TokenType::Mod, "%", 0}); continue; }
        if (current == ';') { get(); tokens.push_back({TokenType::Semicolon, ";", 0}); continue; }
        if (current == '(') { get(); tokens.push_back({TokenType::OpenParen, "(", 0}); continue; }
        if (current == ')') { get(); tokens.push_back({TokenType::CloseParen, ")", 0}); continue; }
        if (current == '{') { get(); tokens.push_back({TokenType::OpenBrace, "{", 0}); continue; }
        if (current == '}') { get(); tokens.push_back({TokenType::CloseBrace, "}", 0}); continue; }
        if (current == '?') { get(); tokens.push_back({TokenType::Question, "?", 0}); continue; }

        // 5. Compound operators (=, ==, !=, <, <=, >, >=)
        if (current == '=') {
            get();
            if (peek() == '=') {
                get();
                tokens.push_back({TokenType::Equal, "==", 0});
            } else {
                tokens.push_back({TokenType::Assign, "=", 0});
            }
            continue;
        }

        if (current == '!') {
            get();
            if (peek() == '=') {
                get();
                tokens.push_back({TokenType::NotEqual, "!=", 0});
            } else {
                throw std::runtime_error("Lexer error: unexpected character '!'");
            }
            continue;
        }

        if (current == '<') {
            get();
            if (peek() == '=') {
                get();
                tokens.push_back({TokenType::LessEq, "<=", 0});
            } else {
                tokens.push_back({TokenType::Less, "<", 0});
            }
            continue;
        }

        if (current == '>') {
            get();
            if (peek() == '=') {
                get();
                tokens.push_back({TokenType::GreaterEq, ">=", 0});
            } else {
                tokens.push_back({TokenType::Greater, ">", 0});
            }
            continue;
        }

        // If an unknown character is encountered
        throw std::runtime_error(std::string("Lexer error: unexpected character '") + current + "'");
    }

    // Add end-of-file/stream marker at the end
    tokens.push_back({TokenType::EndOfFile, "", 0});
    return tokens;
}

} // namespace paracl
