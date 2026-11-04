#ifndef PARACL_OPERATOR_NODES_HPP
#define PARACL_OPERATOR_NODES_HPP

#include "node.hpp"
#include <iostream>
#include <utility>

namespace paracl {

enum class BinaryOpType {
    Add,      // +
    Sub,      // -
    Mul,      // *
    Div,      // /
    Mod,      // %
    Equal,    // ==
    NotEqual, // !=
    Less,     // <
    Greater,  // >
    LessEq,   // <=
    GreaterEq // >=
};

class BinaryOpNode final : public Node {
private:
    BinaryOpType op_;
    NodePtr left_;
    NodePtr right_;

public:
    BinaryOpNode(BinaryOpType op, NodePtr left, NodePtr right)
        : op_(op), left_(std::move(left)), right_(std::move(right)) {}

    int eval(ExecutionContext& ctx) const override {
        int l_val = left_->eval(ctx);
        int r_val = right_->eval(ctx);

        switch (op_) {
            case BinaryOpType::Add:       return l_val + r_val;
            case BinaryOpType::Sub:       return l_val - r_val;
            case BinaryOpType::Mul:       return l_val * r_val;
            case BinaryOpType::Div:
                if (r_val == 0) throw std::runtime_error("Division by zero");
                return l_val / r_val;
            case BinaryOpType::Mod:
                if (r_val == 0) throw std::runtime_error("Modulo by zero");
                return l_val % r_val;
            case BinaryOpType::Equal:     return l_val == r_val;
            case BinaryOpType::NotEqual:  return l_val != r_val;
            case BinaryOpType::Less:      return l_val < r_val;
            case BinaryOpType::Greater:   return l_val > r_val;
            case BinaryOpType::LessEq:    return l_val <= r_val;
            case BinaryOpType::GreaterEq: return l_val >= r_val;
        }
        throw std::runtime_error("Unknown binary operator");
    }

    int dump_dot(std::ostream& out, int& next_id) const override {
        int id = next_id++;
        std::string op_str;
        switch (op_) {
            case BinaryOpType::Add: op_str = "+"; break;
            case BinaryOpType::Sub: op_str = "-"; break;
            case BinaryOpType::Mul: op_str = "*"; break;
            case BinaryOpType::Div: op_str = "/"; break;
            case BinaryOpType::Mod: op_str = "%"; break;
            case BinaryOpType::Equal: op_str = "=="; break;
            case BinaryOpType::NotEqual: op_str = "!="; break;
            case BinaryOpType::Less: op_str = "<"; break;
            case BinaryOpType::Greater: op_str = ">"; break;
            case BinaryOpType::LessEq: op_str = "<="; break;
            case BinaryOpType::GreaterEq: op_str = ">="; break;
        }
        out << "    node" << id << " [label=\"Op: " << op_str << "\", shape=circle, color=orange, style=filled];\n";
        int left_id = left_->dump_dot(out, next_id);
        int right_id = right_->dump_dot(out, next_id);
        out << "    node" << id << " -> node" << left_id << ";\n";
        out << "    node" << id << " -> node" << right_id << ";\n";
        return id;
    }
};

class AssignNode final : public Node {
private:
    std::size_t var_id_;
    NodePtr expr_;

public:
    AssignNode(std::size_t var_id, NodePtr expr)
        : var_id_(var_id), expr_(std::move(expr)) {}

    int eval(ExecutionContext& ctx) const override {
        int val = expr_->eval(ctx);
        ctx.set_variable(var_id_, val);
        return val;
    }

    int dump_dot(std::ostream& out, int& next_id) const override {
        int id = next_id++;
        out << "    node" << id << " [label=\"Assign\\n(Var ID: " << var_id_ << ")\", shape=box, color=yellow, style=filled];\n";
        int expr_id = expr_->dump_dot(out, next_id);
        out << "    node" << id << " -> node" << expr_id << ";\n";
        return id;
    }
};

class PrintNode final : public Node {
private:
    NodePtr expr_;

public:
    explicit PrintNode(NodePtr expr) : expr_(std::move(expr)) {}

    int eval(ExecutionContext& ctx) const override {
        int val = expr_->eval(ctx);
        std::cout << val << std::endl;
        return val;
    }

    int dump_dot(std::ostream& out, int& next_id) const override {
        int id = next_id++;
        out << "    node" << id << " [label=\"Print\", shape=box, color=pink, style=filled];\n";
        int expr_id = expr_->dump_dot(out, next_id);
        out << "    node" << id << " -> node" << expr_id << ";\n";
        return id;
    }
};

class ReadNode final : public Node {
public:
    int eval(ExecutionContext& /*ctx*/) const override {
        int val = 0;
        if (!(std::cin >> val)) {
            throw std::runtime_error("Failed to read integer from stdin");
        }
        return val;
    }

    int dump_dot(std::ostream& out, int& next_id) const override {
        int id = next_id++;
        out << "    node" << id << " [label=\"Read (?)\", shape=box, color=violet, style=filled];\n";
        return id;
    }
};

} // namespace paracl

#endif // PARACL_OPERATOR_NODES_HPP
