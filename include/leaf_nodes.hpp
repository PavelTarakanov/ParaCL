#ifndef PARACL_LEAF_NODES_HPP
#define PARACL_LEAF_NODES_HPP

#include "node.hpp"

namespace paracl {

// Узел-число (константа)
class NumberNode final : public Node {
private:
    int value_;

public:
    explicit NumberNode(int val) : value_(val) {}

    int eval(ExecutionContext&) const override {
        return value_;
    }
};

// Узел-переменная (хранит индекс в массиве контекста)
class VariableNode final : public Node {
private:
    std::size_t var_id_;

public:
    explicit VariableNode(std::size_t var_id) : var_id_(var_id) {}

    std::size_t get_id() const { return var_id_; }

    int eval(ExecutionContext& ctx) const override {
        return ctx.get_variable(var_id_);
    }
};

} // namespace paracl

#endif // PARACL_LEAF_NODES_HPP
