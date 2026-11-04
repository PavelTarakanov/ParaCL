#ifndef PARACL_LEAF_NODES_HPP
#define PARACL_LEAF_NODES_HPP

#include "node.hpp"

namespace paracl {

class NumberNode final : public Node {
private:
    int value_;

public:
    explicit NumberNode(int val) : value_(val) {}

    int eval(ExecutionContext& /*ctx*/) const override {
        return value_;
    }

    int dump_dot(std::ostream& out, int& next_id) const override {
        int id = next_id++;
        out << "    node" << id << " [label=\"Number: " << value_ << "\", shape=box, color=lightblue, style=filled];\n";
        return id;
    }
};

class VariableNode final : public Node {
private:
    std::size_t var_id_;

public:
    explicit VariableNode(std::size_t var_id) : var_id_(var_id) {}

    std::size_t get_id() const { return var_id_; }

    int eval(ExecutionContext& ctx) const override {
        return ctx.get_variable(var_id_);
    }

    int dump_dot(std::ostream& out, int& next_id) const override {
        int id = next_id++;
        out << "    node" << id << " [label=\"Var ID: " << var_id_ << "\", shape=ellipse, color=lightgreen, style=filled];\n";
        return id;
    }
};

} // namespace paracl

#endif // PARACL_LEAF_NODES_HPP
