#include "../include/lexer.hpp"
#include "../include/parser.hpp"
#include "../include/node.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <exception>
#include <cstdlib>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <source_file.pcl>\n";
        return 1;
    }

    std::string filename = argv[1];
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file '" << filename << "'\n";
        return 1;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string source_code = buffer.str();

    try {
        // 1. Lexical analysis
        paracl::Lexer lexer(source_code);
        auto tokens = lexer.tokenize();

        // 2. Parsing (AST generation)
        paracl::Parser parser(tokens);
        auto ast_root = parser.parse();

        // 3. Generate DOT file for tree visualization
        std::ofstream dot_file("ast.dot");
        dot_file << "digraph AST {\n";
        dot_file << "    node [fontname=\"Arial\"];\n";
        int next_id = 0;
        ast_root->dump_dot(dot_file, next_id);
        dot_file << "}\n";
        dot_file.close();

        // 4. Automatically generate PNG image using Graphviz dot utility
        int ret = std::system("dot -Tpng ast.dot -o ast.png");
        if (ret == 0) {
            std::cout << "[Info] AST image successfully generated: 'ast.png'\n";
        } else {
            std::cout << "[Warning] Failed to generate 'ast.png'. Make sure Graphviz is installed ('brew install graphviz').\n";
        }

        // 5. Program execution
        std::cout << "\n=== Program Execution Start ===\n";
        paracl::ExecutionContext ctx;
        ast_root->eval(ctx);
        std::cout << "=== Program Execution End ===\n";

    } catch (const std::exception& e) {
        std::cerr << "Compilation/Runtime Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
