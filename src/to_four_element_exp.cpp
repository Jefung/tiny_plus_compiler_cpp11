//
// Created by JefungChan on 2018/11/4.
//

#include "to_four_element_exp.h"


std::vector<std::string> ToFourElementExp::convert(TreeNode *root) {
    if (!root)
        return {};

    stmt_seq_node_handle(root);
    return res_;
}

std::string ToFourElementExp::get_new_label() {
    return "L" + std::to_string(label_num++);
}

void ToFourElementExp::stmt_node_handle(TreeNode *root) {
    if (!root)
        return;
    std::string after_stmt_label = get_new_label();

    switch (root->type_) {
        case TreeNode::Type::IF_STMT:
            if_node_handle(root, after_stmt_label);
            break;
        case TreeNode::Type::ASSIGN_STMT:
            assign_node_handle(root);
            break;
        case TreeNode::Type::WHILE_STMT:
            while_node_handle(root, after_stmt_label);
            break;
        case TreeNode::Type::REPEAT_STMT:
            repeat_node_handle(root, after_stmt_label);
            break;
        case TreeNode::Type::READ_STMT:
            read_node_handle(root);
            break;
        case TreeNode::Type::WRITE_STMT:
            write_node_handle(root);
            break;
        default:
            break;
    }
    res_.push_back("Label " + after_stmt_label);

}

void ToFourElementExp::if_node_handle(TreeNode *root, std::string end_label) {
    std::string before_else_label = end_label;
    std::string before_then_label = get_new_label();
    if (root->child_[2]) {
        before_else_label = get_new_label();
        cond_exp_handle(root->child_[0], before_then_label, before_else_label);
        res_.push_back("Label " + before_then_label);
        stmt_seq_node_handle(root->child_[1]);
        res_.push_back("goto " + end_label);
        res_.push_back("Label " + before_else_label);
        stmt_seq_node_handle(root->child_[2]);
    } else {
        cond_exp_handle(root->child_[0], before_then_label, end_label);
        res_.push_back("Label " + before_then_label);
        stmt_seq_node_handle(root->child_[1]);
        res_.push_back("Label " + end_label);
    }
}

std::string ToFourElementExp::log_or_handle(TreeNode *root) {

    switch (root->type_) {
        case TreeNode::Type::LOG_OR_EXP: {
            std::string log_and_res = log_and_handle(root->child_[0]);
            std::string another_log_or_res = log_or_handle(root->child_[1]);
            std::string log_or_res = get_var_name();
            res_.push_back(log_or_res + ":=" + log_and_res + "or" + another_log_or_res);
            return log_or_res;
        }
        default:
            return log_and_handle(root);
    }
}

std::string ToFourElementExp::log_and_handle(TreeNode *root) {
    switch (root->type_) {
        case TreeNode::Type::LOG_AND_EXP: {
            std::string comp_exp_res = comparison_exe_handle(root->child_[0]);
            std::string second_log_and_res = log_and_handle(root->child_[1]);
            std::string log_and_res = get_var_name();
            res_.push_back(log_and_res + ":=" + comp_exp_res + "and" + second_log_and_res);
            return log_and_res;
        }
        default:
            return comparison_exe_handle(root);
    }
}

std::string ToFourElementExp::comparison_exe_handle(TreeNode *root) {
    switch (root->type_) {
        case TreeNode::Type::LEQ_EXP:
        case TreeNode::Type::LSS_EXP:
        case TreeNode::Type::GTR_EXP:
        case TreeNode::Type::GEQ_EXP:
        case TreeNode::Type::EQU_EXP: {
            std::string comp_left = add_exp_handle(root->child_[0]);
            std::string comp_right = comparison_exe_handle(root->child_[1]);
            std::string op;
            switch (root->type_) {
                case TreeNode::Type::LEQ_EXP:
                    op = "<=";
                    break;
                case TreeNode::Type::LSS_EXP:
                    op = "<";
                    break;
                case TreeNode::Type::GTR_EXP:
                    op = ">";
                    break;
                case TreeNode::Type::GEQ_EXP:
                    op = ">=";
                    break;
                case TreeNode::Type::EQU_EXP:
                    op = "=";
                    break;
                default:
                    break;
            }
            return comp_left + op + comp_right;
        }
        default:
            return add_exp_handle(root);

    }
}

std::string ToFourElementExp::add_exp_handle(TreeNode *root) {
    switch (root->type_) {
        case TreeNode::Type::ADD_EXP:
        case TreeNode::Type::SUB_EXP: {
            std::string add_left = mul_exe_handle(root->child_[0]);
            std::string add_right = add_exp_handle(root->child_[1]);
            std::string add_res = get_var_name();
            std::string op;
            if (root->type_ == TreeNode::Type::ADD_EXP)
                op = "+";
            if (root->type_ == TreeNode::Type::SUB_EXP)
                op = "-";
            res_.push_back(add_res + ":=" + add_left + op + add_right);
            return add_res;
        }
        default:
            return mul_exe_handle(root);
    }
}

std::string ToFourElementExp::mul_exe_handle(TreeNode *root) {
    switch (root->type_) {
        case TreeNode::Type::MUL_EXP:
        case TreeNode::Type::DIV_EXP: {
            std::string mul_left = mul_exe_handle(root->child_[0]);
            std::string mul_right = add_exp_handle(root->child_[1]);
            std::string mul_res = get_var_name();
            std::string op;
            if (root->type_ == TreeNode::Type::MUL_EXP)
                op = "*";
            if (root->type_ == TreeNode::Type::DIV_EXP)
                op = "/";
            res_.push_back(mul_res + " := " + mul_left + op + mul_right);
            return mul_res;
        }
        default:
            return factor_handle(root);
    }
}

std::string ToFourElementExp::get_var_name() {
    return "t" + std::to_string(var_num++);
}

std::string ToFourElementExp::factor_handle(TreeNode *root) {
    switch (root->tk_.kind) {
        case Token::Kind::ID:
            return root->tk_.value;
        case Token::Kind::NUM:
            return root->tk_.value;
        case Token::Kind::STR:
            return "'" + root->tk_.value + "'";
        case Token::Kind::TK_FALSE:
            return "false";
        case Token::Kind::TK_TRUE:
            return "true";
        default:
            return log_or_handle(root);
    }
}

void ToFourElementExp::cond_exp_handle(TreeNode *root, std::string true_label, std::string false_label) {

    switch (root->type_) {
        case TreeNode::Type::LOG_OR_EXP: {
            std::string before_second_log_or_label = get_new_label();
            cond_exp_handle(root->child_[0], true_label, before_second_log_or_label);
            res_.push_back("Label " + before_second_log_or_label);
            cond_exp_handle(root->child_[1], true_label, false_label);
            break;
        }
        case TreeNode::Type::LOG_AND_EXP: {
            std::string before_second_log_and_label = get_new_label();
            cond_exp_handle(root->child_[0], before_second_log_and_label, false_label);
            res_.push_back("Label " + before_second_log_and_label);
            cond_exp_handle(root->child_[1], true_label, false_label);
            break;
        }
        default: {
            std::string comp_res = comparison_exe_handle(root);
            res_.push_back("if " + comp_res + " goto " + true_label);
            res_.push_back("goto " + false_label);
            break;
        }
    }
}

void ToFourElementExp::assign_node_handle(TreeNode *root) {
    std::string log_or_res = log_or_handle(root->child_[1]);
    std::string id_res = factor_handle(root->child_[0]);
    res_.push_back(id_res + ":=" + log_or_res);
}

void ToFourElementExp::while_node_handle(TreeNode *root, std::string end_label) {
    std::string before_cond_label = get_new_label();
    std::string before_then_label = get_new_label();
    res_.push_back("Label " + before_cond_label);
    cond_exp_handle(root->child_[0], before_then_label, end_label);
    res_.push_back("Label " + before_then_label);
    stmt_seq_node_handle(root->child_[1]);
    res_.push_back("goto " + before_cond_label);

}

void ToFourElementExp::read_node_handle(TreeNode *root) {
    res_.push_back("read " + root->tk_.value);
}

void ToFourElementExp::write_node_handle(TreeNode *root) {
    res_.push_back("write " + root->child_[0]->tk_.value);
}

void ToFourElementExp::repeat_node_handle(TreeNode *root, std::string end_label) {
    std::string before_stmt_label = get_new_label();
    res_.push_back("Label " + before_stmt_label);
    stmt_seq_node_handle(root->child_[0]);
    cond_exp_handle(root->child_[1], end_label, before_stmt_label);
}

void ToFourElementExp::stmt_seq_node_handle(TreeNode *root) {
    switch (root->type_) {
        case TreeNode::Type::STMT_SEQUENCE:
            stmt_seq_node_handle(root->child_[0]);
            stmt_node_handle(root->child_[1]);
            break;
        default:
            stmt_node_handle(root);
    }
}
