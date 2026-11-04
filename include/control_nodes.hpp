#ifndef PARACL_CONTROL_NODES_HPP
#define PARACL_CONTROL_NODES_HPP

#include "node.hpp"
#include <vector>
#include <utility>

namespace paracl {

// Compound node (block of sequential statements { stmt1; stmt2; })
class BlockNode final : public Node {
private:
    std::vector<NodePtr> stmts_;

public:
    BlockNode() = default;

    void add_statement(NodePtr stmt) {
        stmts_.push_back(std::move(stmt));
    }

    int eval(ExecutionContext& ctx) const override {
        int last_res = 0;
        for (const auto& stmt : stmts_) {
            if (stmt) {
                last_res = stmt->eval(ctx);
            }
        }
        return last_res;
    }
};

// Conditional statement if (cond) { then } else { else }
class IfNode final : public Node {
private:
    NodePtr cond_;
    NodePtr then_branch_;
    NodePtr else_branch_; // Can be nullptr

public:
    IfNode(NodePtr cond, NodePtr then_b, NodePtr else_b = nullptr)
        : cond_(std::move(cond)),
          then_branch_(std::move(then_b)),
          else_branch_(std::move(else_b)) {}

    int eval(ExecutionContext& ctx) const override {
        if (cond_->eval(ctx) != 0) {
            return then_branch_->eval(ctx);
        } else if (else_branch_) {
            return else_branch_->eval(ctx);
        }
        return 0;
    }
};

// While loop while (cond) { body }
class WhileNode final : public Node {
private:
    NodePtr cond_;
    NodePtr body_;

public:
    WhileNode(NodePtr cond, NodePtr body)
        : cond_(std::move(cond)), body_(std::move(body)) {}

    int eval(ExecutionContext& ctx) const override {
        int last_res = 0;
        while (cond_->eval(ctx) != 0) {
            last_res = body_->eval(ctx);
        }
        return last_res;
    }
};

} // namespace paracl

#endif // PARACL_CONTROL_NODES_HPP
