#ifndef PARACL_CONTROL_NODES_HPP
#define PARACL_CONTROL_NODES_HPP

#include "node.hpp"
#include <vector>
#include <utility>

namespace paracl {

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

    int dump_dot(std::ostream& out, int& next_id) const override {
        int id = next_id++;
        out << "    node" << id << " [label=\"Block\", shape=box, color=lightgrey, style=filled];\n";
        for (const auto& stmt : stmts_) {
            if (stmt) {
                int stmt_id = stmt->dump_dot(out, next_id);
                out << "    node" << id << " -> node" << stmt_id << ";\n";
            }
        }
        return id;
    }
};

class IfNode final : public Node {
private:
    NodePtr cond_;
    NodePtr then_branch_;
    NodePtr else_branch_;

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

    int dump_dot(std::ostream& out, int& next_id) const override {
        int id = next_id++;
        out << "    node" << id << " [label=\"If\", shape=diamond, color=coral, style=filled];\n";
        int cond_id = cond_->dump_dot(out, next_id);
        int then_id = then_branch_->dump_dot(out, next_id);
        out << "    node" << id << " -> node" << cond_id << " [label=\"cond\"];\n";
        out << "    node" << id << " -> node" << then_id << " [label=\"then\"];\n";
        if (else_branch_) {
            int else_id = else_branch_->dump_dot(out, next_id);
            out << "    node" << id << " -> node" << else_id << " [label=\"else\"];\n";
        }
        return id;
    }
};

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

    int dump_dot(std::ostream& out, int& next_id) const override {
        int id = next_id++;
        out << "    node" << id << " [label=\"While\", shape=diamond, color=gold, style=filled];\n";
        int cond_id = cond_->dump_dot(out, next_id);
        int body_id = body_->dump_dot(out, next_id);
        out << "    node" << id << " -> node" << cond_id << " [label=\"cond\"];\n";
        out << "    node" << id << " -> node" << body_id << " [label=\"body\"];\n";
        return id;
    }
};

} // namespace paracl

#endif // PARACL_CONTROL_NODES_HPP
