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

// Вычислительные бинарные операции
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
};

// Оператор присваивания (x = expr)
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
};

// Оператор вывода в поток (print expr)
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
};

// Оператор ввода со стандартного потока (?)
class ReadNode final : public Node {
public:
    int eval(ExecutionContext& /*ctx*/) const override {
        int val = 0;
        if (!(std::cin >> val)) {
            throw std::runtime_error("Failed to read integer from stdin");
        }
        return val;
    }
};

} // namespace paracl

#endif // PARACL_OPERATOR_NODES_HPP
