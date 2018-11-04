//
// Created by JefungChan on 2018/11/1.
//

#include <iostream>
#include "grammar_analysis.h"

GrammarAnalysis::GrammarAnalysis(std::vector<std::pair<Kind::Type, std::string>> tokens) {
    tokens_ = tokens;
    std::cout << tokens_.size() << std::endl;
    cur_index_ = 0;
    // todo: handle tokens_ empty
    cur_token_ = tokens_[0];
}

bool GrammarAnalysis::get_next_token(Token &token) {
    cur_index_++;
    if (cur_index_ < tokens_.size()) {
        token = tokens_[cur_index_];
        return true;
    }
    // token = NULL;
    token = {Kind::Type::END_OF_FILE, ""};
    return false;
}

void GrammarAnalysis::token_forward(std::string no_more_token_msg)throw(ErrorMsg) {
    if (!get_next_token(cur_token_) && no_more_token_msg != "") {
        std::cout << "no_more_token_msg:" << no_more_token_msg << "" << std::endl;
        // todo: custom error msg, add row and col information
        // throw ErrorMsg(0, 0, no_more_token_msg);
    }
}

bool GrammarAnalysis::match(std::set<Kind::Type> kind_set) {
    if (kind_set.find(cur_token_.first) != kind_set.end()) {
        return true;
    } else {
        // std::cout << "Token.kind类型不匹配" << std::endl;
        return false;
    }
}

TreeNode *GrammarAnalysis::program() {
    declaration();
    function_def();
    return stmt_sequence();
}

void GrammarAnalysis::declaration() {
    while (match({Kind::Type::TK_INT, Kind::Type::TK_BOOL, Kind::Type::TK_STRING})) {
        // todo: store var declaration into sym table
        if (cur_index_ + 2 < tokens_.size() && tokens_[cur_index_ + 2].first == Kind::Type::TK_LP)
            return;
        do {
            token_forward("声明变量缺少变量名");
            match_and_forward({Kind::Type::ID});
            // todo: add ID to symbol table
        } while (match({Kind::Type::TK_COMMA})); //匹配逗号
        match_and_forward({Kind::Type::TK_SEMICOLON}); //匹配分号
    }
}

bool GrammarAnalysis::match_and_forward(std::set<Kind::Type> kind_set) {
    if (match(kind_set)) {
        std::cout << "match_and_forward" << std::endl;
        token_forward();
        return true;
    }
    return false;
}

TreeNode *GrammarAnalysis::stmt_sequence() {
    TreeNode *t1 = nullptr, *t2 = nullptr;
    KindSet first_statement = {Kind::Type::TK_IF, Kind::Type::TK_WHILE, Kind::Type::TK_REPEAT,
                               Kind::Type::TK_READ, Kind::Type::TK_WRITE, Kind::Type::ID};

    while (match(first_statement)) {
        Token prev_token = cur_token_;
        switch (prev_token.first) {
            case Kind::Type::TK_IF:
                t2 = if_stmt();
                break;
            case Kind::Type::TK_WHILE:
                t2 = while_stmt();
                break;
            case Kind::Type::TK_REPEAT:
                t2 = repeat_stmt();
                break;
            case Kind::Type::TK_READ:
                t2 = read_stmt();
                break;
            case Kind::Type::TK_WRITE:
                t2 = write_stmt();
                break;
            case Kind::Type::ID:
                t2 = assign_stmt();
                break;
            default:
                break;
        }
        if (t1 == nullptr)
            t1 = t2;
        else {
            t1 = new TreeNode(TreeNode::Type::STMT_SEQUENCE, t1, t2);
        }
    }
    return t1;
}

TreeNode *GrammarAnalysis::if_stmt() {
    token_forward();
    TreeNode *cond_exp = nullptr, *then_stmt = nullptr, *else_stmt = nullptr;
    cond_exp = log_or_exp();
    match_and_forward({Kind::Type::TK_THEN});
    then_stmt = stmt_sequence();
    if (match({Kind::Type::TK_ELSE})) {
        token_forward();
        else_stmt = stmt_sequence();
    }
    match_and_forward({Kind::Type::TK_END});
    return new TreeNode(TreeNode::Type::IF_STMT, cond_exp, then_stmt, else_stmt);
}

TreeNode *GrammarAnalysis::log_or_exp() {
    TreeNode *log_and_exp_node = nullptr, *log_or_exp_node = nullptr;
    log_and_exp_node = log_and_exp();
    if (match({Kind::Type::TK_OR})) {
        token_forward();
        log_or_exp_node = log_or_exp();
    }
    if (log_or_exp_node)
        return new TreeNode(TreeNode::Type::LOG_OR_EXP, log_and_exp_node, log_or_exp_node);
    else
        return log_and_exp_node;
}

TreeNode *GrammarAnalysis::log_and_exp() {
    TreeNode *log_and_exp_node = nullptr;
    TreeNode *another_log_and_exp_node = nullptr;
    log_and_exp_node = comparision_exp();
    if (match({Kind::Type::TK_AND})) {
        token_forward();
        another_log_and_exp_node = log_and_exp();
    }
    if (another_log_and_exp_node)
        return new TreeNode(TreeNode::Type::LOG_AND_EXP, log_and_exp_node, another_log_and_exp_node);
    else
        return log_and_exp_node;
}

TreeNode *GrammarAnalysis::comparision_exp() {
    TreeNode *add_exp_node = nullptr;
    TreeNode *comparison_exp_node = nullptr;
    add_exp_node = add_exp();
    KindSet comparison_op_set = {Kind::Type::TK_LEQ, Kind::Type::TK_GEQ, Kind::Type::TK_LSS,
                                 Kind::Type::TK_EQU, Kind::Type::TK_GTR};
    if (match(comparison_op_set)) {
        Token prev_token = cur_token_;
        token_forward();
        if (prev_token.first == Kind::Type::TK_LEQ)
            return new TreeNode(TreeNode::Type::LEQ_EXP, add_exp_node, comparision_exp());
        if (prev_token.first == Kind::Type::TK_GEQ)
            return new TreeNode(TreeNode::Type::GEQ_EXP, add_exp_node, comparision_exp());
        if (prev_token.first == Kind::Type::TK_LSS)
            return new TreeNode(TreeNode::Type::LSS_EXP, add_exp_node, comparision_exp());
        if (prev_token.first == Kind::Type::TK_EQU)
            return new TreeNode(TreeNode::Type::EQU_EXP, add_exp_node, comparision_exp());
        if (prev_token.first == Kind::Type::TK_GTR)
            return new TreeNode(TreeNode::Type::GTR_EXP, add_exp_node, comparision_exp());
    }
    return add_exp_node;
}

TreeNode *GrammarAnalysis::add_exp() {
    TreeNode *add_exp_node = nullptr;
    add_exp_node = mul_exp();
    KindSet add_op_set = {Kind::Type::TK_ADD, Kind::Type::TK_SUB};
    if (match(add_op_set)) {
        Token prev_token = cur_token_;
        token_forward();
        if (prev_token.first == Kind::Type::TK_ADD) {
            return new TreeNode(TreeNode::Type::ADD_EXP, add_exp_node, add_exp());
        }
        if (prev_token.first == Kind::Type::TK_SUB)
            return new TreeNode(TreeNode::Type::SUB_EXP, add_exp_node, add_exp());
    }
    return add_exp_node;
}

TreeNode *GrammarAnalysis::mul_exp() {
    TreeNode *factor_node = factor();
    if (cur_token_.first == Kind::Type::TK_MUL) {
        token_forward();
        return new TreeNode(TreeNode::Type::MUL_EXP, factor_node, mul_exp());
    } else {
        if (cur_token_.first == Kind::Type::TK_DIV) {
            token_forward();
            return new TreeNode(TreeNode::Type::DIV_EXP, factor_node, mul_exp());
        } else {
            // token_forward();
            return factor_node;
            // std::cout << "乘除匹配失败" << std::endl;
        }

    }
}

TreeNode *GrammarAnalysis::assign_stmt() {
    TreeNode *node = factor();
    // token_forward();
    if (match({Kind::Type::TK_ASSIGN})) {
        token_forward();
        return new TreeNode(TreeNode::Type::ASSIGN_STMT, node, log_or_exp());
    } else {
        std::cout << ":= 匹配失败" << std::endl;
        return nullptr;
    }
}

TreeNode *GrammarAnalysis::factor() {
    KindSet factor_set = {Kind::Type::NUM, Kind::Type::STR,
                          Kind::Type::ID, Kind::Type::TK_TRUE,
                          Kind::Type::TK_FALSE};
    TreeNode *mul_exp_node = nullptr;
    if (match({Kind::Type::TK_LP})) {
        token_forward();
        TreeNode *log_or_exp_node = log_or_exp();
        std::cout << Kind::name(cur_token_.first) << "------" << std::endl;
        match({Kind::Type::TK_RP});
        match_and_forward({Kind::Type::TK_RP});
        return log_or_exp_node;
    }
    if (match(factor_set)) {
        auto prev_token = cur_token_;
        token_forward();
        return new TreeNode(TreeNode::Type::FACTOR, prev_token);
    } else {
        std::cout << "factor 没匹配错误处理" << std::endl;
        return nullptr;
    }
    return nullptr;
}

TreeNode *GrammarAnalysis::repeat_stmt() {
    match_and_forward({Kind::Type::TK_REPEAT});
    TreeNode *stmt_seq_node = stmt_sequence();
    match_and_forward({Kind::Type::TK_UNTIL});
    TreeNode *log_or_exp_node = log_or_exp();
    return new TreeNode(TreeNode::Type::REPEAT_STMT, stmt_seq_node, log_or_exp_node);
}

TreeNode *GrammarAnalysis::read_stmt() {
    match_and_forward({Kind::Type::TK_READ});
    TreeNode *read_node = nullptr;
    if (match({Kind::Type::ID})) {
        read_node = new TreeNode(TreeNode::Type::READ_STMT, cur_token_);
        token_forward();
        match_and_forward({Kind::Type::TK_SEMICOLON});
        return read_node;
        // todo: how to handle ID ? as a child or store it to node.tk_
    } else {
        std::cout << "read error!!!!!" << std::endl;
        return nullptr;
    }

}

TreeNode *GrammarAnalysis::write_stmt() {
    match_and_forward({Kind::Type::TK_WRITE});
    TreeNode *log_or_exp_node = log_or_exp();
    // todo: the end char of write stmt is ';'?
    match_and_forward({Kind::Type::TK_SEMICOLON});
    return new TreeNode(TreeNode::Type::WRITE_STMT, log_or_exp_node);
}

TreeNode *GrammarAnalysis::while_stmt() {
    match_and_forward({Kind::Type::TK_WHILE});
    TreeNode *log_or_exp_node = log_or_exp();
    match_and_forward({Kind::Type::TK_DO});
    TreeNode *stmt_seq_node = stmt_sequence();
    match_and_forward({Kind::Type::TK_END});
    return new TreeNode(TreeNode::Type::WHILE_STMT, log_or_exp_node, stmt_seq_node);
}

void GrammarAnalysis::function_def() {
    while (match({Kind::Type::TK_INT, Kind::Type::TK_BOOL, Kind::Type::TK_STRING})) {
        token_forward();
        // todo: store function_def to sym
        match_and_forward({Kind::Type::ID});
        match_and_forward({Kind::Type::TK_LP});
        while (!match({Kind::Type::TK_RP})) {
            parameters_list();
        }
        match_and_forward({Kind::Type::TK_RP});
        declaration();
        // todo: handle stmt_sequence
        stmt_sequence();
        match_and_forward({Kind::Type::TK_END});
    }

}

void GrammarAnalysis::parameters_list() {
    while (!match({Kind::Type::TK_RP})) {
        while (true) {
            parameter();
            if (match({Kind::Type::TK_COMMA})) {
                token_forward();
                continue;
            } else {
                if (match({Kind::Type::TK_RP}))
                    break;
                else
                    std::cout << "Error: please handle paramters list handle" << std::endl;
            }
        }
    }
}

void GrammarAnalysis::parameter() {
    match_and_forward({Kind::Type::TK_INT, Kind::Type::TK_BOOL, Kind::Type::TK_STRING});
    std::cout << "parameter:" << cur_token_.second << std::endl;
    match_and_forward({Kind::Type::ID});
}
