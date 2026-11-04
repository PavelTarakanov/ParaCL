#ifndef PARACL_NODE_HPP
#define PARACL_NODE_HPP

#include <vector>
#include <memory>
#include <stdexcept>
#include <cstddef>

namespace paracl {

// Execution context: stores the values of all variables by their numerical index
struct ExecutionContext {
    std::vector<int> variables;

    int get_variable(std::size_t index) const {
        if (index >= variables.size()) {
            throw std::runtime_error("Error: Variable index out of bounds");
        }
        return variables[index];
    }

    void set_variable(std::size_t index, int value) {
        if (index >= variables.size()) {
            variables.resize(index + 1, 0);
        }
        variables[index] = value;
    }
};

// Abstract base class for an AST node
class Node {
public:
    virtual ~Node() = default; // Mandatory virtual destructor (RAII)

    // Pure virtual method to evaluate/execute the node
    virtual int eval(ExecutionContext& ctx) const = 0;
};

using NodePtr = std::unique_ptr<Node>;

} // namespace paracl

#endif // PARACL_NODE_HPP
