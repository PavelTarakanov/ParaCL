#include "lexer.hpp"
#include <iostream>

int main() {
    std::string code = "iters = ?; while (iters > 0) { print iters; }";
    paracl::Lexer lexer(code);
    auto tokens = lexer.tokenize();

    for (const auto& token : tokens) {
        std::cout << "Token text: '" << token.text << "'\n";
    }
    return 0;
}
