//
// Created by JefungChan on 2018/11/1.
//

#include <iostream>
#include "grammar_analysis.h"

GrammarAnalysis::GrammarAnalysis(Tokens tokens) {
    tokens_ = tokens;
    token_it_ = tokens_.begin();
}


bool GrammarAnalysis::match(KindSet kind_set, std::string eof_error_msg)  throw(ErrorMsg) {
    if (eof_error_msg != "") {
        if (token_it_ == tokens_.end()) {
            throw ErrorMsg(tokens_.back().line, tokens_.back().column, eof_error_msg);
        }
        if (kind_set.find((*token_it_).kind) == kind_set.end())
            throw ErrorMsg((*token_it_).line, (*token_it_).column, eof_error_msg);
        return true;
    } else {
        if (token_it_ == tokens_.end()) {
            return false;
        }

        return kind_set.find((*token_it_).kind) != kind_set.end();
    }
}

TreeNode *GrammarAnalysis::program(ErrorMsgs &error_msgs) {
    if (tokens_.size() == 0)
        return nullptr;
    try {
        declaration();
        return stmt_sequence();
    } catch (ErrorMsg &msg) {
        std::cout << msg.to_string() << std::endl;
        error_msgs.emplace_back(msg);
        return nullptr;
    }
    // function_def();


    return nullptr;
}

void GrammarAnalysis::declaration() {
    while (match({Token::Kind::TK_INT, Token::Kind::TK_BOOL, Token::Kind::TK_STRING})) {
        // todo: store var declaration into sym table
        // 函数声明前两位和标识符声明一样,第三位是'('
        if (std::distance(token_it_, tokens_.end()) > 2 && (*(token_it_ + 2)).kind == Token::Kind::TK_LP)
            return;
        ValType val_type;
        if (token_it_->kind == Token::Kind::TK_INT)
            val_type = ValType::VT_INT;
        if (token_it_->kind == Token::Kind::TK_BOOL)
            val_type = ValType::VT_BOOL;
        if (token_it_->kind == Token::Kind::TK_STRING)
            val_type = ValType::VT_STRING;

        do {
            token_it_++;
            match({Token::Kind::ID}, "标识符后面需要跟变量名");
            Sym *sym = sym_table_.insert(token_it_->value);
            if (!sym)
                throw ErrorMsg(token_it_->line, token_it_->column, "标识符已经存在");
            sym->obj_type = ObjType::OT_VAR;
            sym->val_type = val_type;
            sym->tk = *token_it_;
            token_it_++;
            // todo: add ID to symbol table
        } while (match({Token::Kind::TK_COMMA})); //匹配逗号
        match_and_forward({Token::Kind::TK_SEMICOLON}, "变量声明最后应该加';'"); //匹配分号
    }
}

bool GrammarAnalysis::match_and_forward(KindSet kind_set, std::string eof_error_msg) throw(ErrorMsg) {
    bool res = match(kind_set, eof_error_msg);

    if (token_it_ != tokens_.end()) {
        token_it_++;
    }
    return res;
}

void GrammarAnalysis::print_sym_table() {
    sym_table_.print();
}

int GrammarAnalysis::get_sym_table_size() {
    return sym_table_.size();
}

TreeNode *GrammarAnalysis::stmt_sequence() {
    TreeNode *t1 = nullptr, *t2 = nullptr;
    KindSet first_statement = {Token::Kind::TK_IF, Token::Kind::TK_WHILE, Token::Kind::TK_REPEAT,
                               Token::Kind::TK_READ, Token::Kind::TK_WRITE, Token::Kind::ID};

    while (match(first_statement)) {
        switch ((*token_it_).kind) {
            case Token::Kind::TK_IF:
                t2 = if_stmt();
                break;
            case Token::Kind::TK_WHILE:
                t2 = while_stmt();
                break;
            case Token::Kind::TK_REPEAT:
                t2 = repeat_stmt();
                break;
            case Token::Kind::TK_READ:
                t2 = read_stmt();
                break;
            case Token::Kind::TK_WRITE:
                t2 = write_stmt();
                break;
            case Token::Kind::ID:
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
    match_and_forward({Token::Kind::TK_IF}, "期望是if关键词");
    TreeNode *cond_exp = nullptr, *then_stmt = nullptr, *else_stmt = nullptr;
    cond_exp = log_or_exp();
    match_and_forward({Token::Kind::TK_THEN}, "期望是then关键词");
    then_stmt = stmt_sequence();
    if (match({Token::Kind::TK_ELSE})) {
        token_it_++;
        else_stmt = stmt_sequence();
    }
    match_and_forward({Token::Kind::TK_END}, "if控制块缺少end结束符");
    return new TreeNode(TreeNode::Type::IF_STMT, cond_exp, then_stmt, else_stmt);
}

TreeNode *GrammarAnalysis::log_or_exp() {
    TreeNode *log_and_exp_node = nullptr, *log_or_exp_node = nullptr;
    log_and_exp_node = log_and_exp();
    if (match({Token::Kind::TK_OR})) {
        token_it_++;
        log_or_exp_node = log_or_exp();
    }
    if (log_or_exp_node) {
        TreeNode *tmp = new TreeNode(TreeNode::Type::LOG_OR_EXP, log_and_exp_node, log_or_exp_node);
        tmp->val_type_ = ValType::VT_BOOL;
        return tmp;
    } else
        return log_and_exp_node;
}

TreeNode *GrammarAnalysis::log_and_exp() {
    TreeNode *log_and_exp_node = nullptr;
    TreeNode *another_log_and_exp_node = nullptr;
    log_and_exp_node = comparision_exp();
    if (match({Token::Kind::TK_AND})) {
        token_it_++;
        another_log_and_exp_node = log_and_exp();
    }
    if (another_log_and_exp_node) {
        TreeNode *tmp = new TreeNode(TreeNode::Type::LOG_AND_EXP, log_and_exp_node, another_log_and_exp_node);
        tmp->val_type_ = ValType::VT_BOOL;
        return tmp;
    } else
        return log_and_exp_node;
}

TreeNode *GrammarAnalysis::comparision_exp() {
    TreeNode *add_exp_node = nullptr;
    add_exp_node = add_exp();
    KindSet comparison_op_set = {Token::Kind::TK_LEQ, Token::Kind::TK_GEQ, Token::Kind::TK_LSS,
                                 Token::Kind::TK_EQU, Token::Kind::TK_GTR};
    if (match(comparison_op_set)) {
        Token prev_token = *token_it_;
        token_it_++;
        TreeNode *comparison_node;
        if (prev_token.kind == Token::Kind::TK_LEQ)
            comparison_node = new TreeNode(TreeNode::Type::LEQ_EXP, add_exp_node, comparision_exp());
        if (prev_token.kind == Token::Kind::TK_GEQ)
            comparison_node = new TreeNode(TreeNode::Type::GEQ_EXP, add_exp_node, comparision_exp());
        if (prev_token.kind == Token::Kind::TK_LSS)
            comparison_node = new TreeNode(TreeNode::Type::LSS_EXP, add_exp_node, comparision_exp());
        if (prev_token.kind == Token::Kind::TK_EQU)
            comparison_node = new TreeNode(TreeNode::Type::EQU_EXP, add_exp_node, comparision_exp());
        if (prev_token.kind == Token::Kind::TK_GTR)
            comparison_node = new TreeNode(TreeNode::Type::GTR_EXP, add_exp_node, comparision_exp());
        comparison_node->val_type_ = ValType::VT_BOOL;
        return comparison_node;
    }
    return add_exp_node;
}

TreeNode *GrammarAnalysis::add_exp() {
    TreeNode *add_exp_node = nullptr;
    add_exp_node = mul_exp();
    if (add_exp_node->val_type_ != ValType::VT_STRING and add_exp_node->val_type_ != ValType::VT_INT)
        throw ErrorMsg(token_it_->line, token_it_->column, "非数字或字符串不能进行加减乘除");
    KindSet add_op_set = {Token::Kind::TK_ADD, Token::Kind::TK_SUB};
    if (match(add_op_set)) {
        Token prev_token = *token_it_;
        token_it_++;
        TreeNode *another_add_exp_node = add_exp();
        if (add_exp_node->val_type_ != ValType::VT_STRING and add_exp_node->val_type_ != ValType::VT_INT)
            throw ErrorMsg(token_it_->line, token_it_->column, "非数字或字符串不能进行加减乘除");
        if (add_exp_node->val_type_ != another_add_exp_node->val_type_)
            throw ErrorMsg(token_it_->line, token_it_->column, "不同类型的不能进行相加");

        if (prev_token.kind == Token::Kind::TK_ADD) {
            TreeNode *tmp = new TreeNode(TreeNode::Type::ADD_EXP, add_exp_node, another_add_exp_node);
            tmp->val_type_ = add_exp_node->val_type_;
            return tmp;
        }
        if (prev_token.kind == Token::Kind::TK_SUB) {
            TreeNode *tmp = new TreeNode(TreeNode::Type::SUB_EXP, add_exp_node, another_add_exp_node);
            tmp->val_type_ = add_exp_node->val_type_;
            return tmp;
        }
    }
    return add_exp_node;
}

TreeNode *GrammarAnalysis::mul_exp() {
    TreeNode *node1 = factor();
    if (token_it_->kind == Token::Kind::TK_MUL) {
        token_it_++;
        TreeNode *node2 = mul_exp();
        if (node1->val_type_ != ValType::VT_INT or node2->val_type_ != ValType::VT_INT)
            throw ErrorMsg(token_it_->line, token_it_->column, "非int不能相乘");
        TreeNode *node3 = new TreeNode(TreeNode::Type::MUL_EXP, node1, node2);
        node3->val_type_ = ValType::VT_INT;
        return node3;
    } else {
        if (token_it_->kind == Token::Kind::TK_DIV) {
            token_it_++;
            TreeNode *node2 = mul_exp();
            if (node1->val_type_ != ValType::VT_INT or node2->val_type_ != ValType::VT_INT)
                throw ErrorMsg(token_it_->line, token_it_->column, "非int不能相除");
            TreeNode *node3 = new TreeNode(TreeNode::Type::DIV_EXP, node1, node2);
            node3->val_type_ = ValType::VT_INT;
            return node3;
        } else {
            return node1;
        }
    }
}

TreeNode *GrammarAnalysis::assign_stmt() {
    TreeNode *node = factor();

    if (node->tk_.kind != Token::Kind::ID)
        throw ErrorMsg(token_it_->line, token_it_->column, "赋值语句左边应该是变量");
    if (match({Token::Kind::TK_ASSIGN}, "赋值语句应该有:=")) {
        token_it_++;
        TreeNode *log_or_exp_node = log_or_exp();
        if (node->val_type_ != log_or_exp_node->val_type_)
            throw ErrorMsg(token_it_->line, token_it_->column, "赋值语句左右两边应该是同种类型");
        return new TreeNode(TreeNode::Type::ASSIGN_STMT, node, log_or_exp_node);
    }
    return nullptr;
}

TreeNode *GrammarAnalysis::factor() {
    KindSet factor_set = {Token::Kind::NUM, Token::Kind::STR,
                          Token::Kind::ID, Token::Kind::TK_TRUE,
                          Token::Kind::TK_FALSE};
    TreeNode *mul_exp_node = nullptr;
    if (match({Token::Kind::TK_LP})) {
        token_it_++;
        TreeNode *log_or_exp_node = log_or_exp();
        match_and_forward({Token::Kind::TK_RP}, "'{'缺少'}'的匹配");
        return log_or_exp_node;
    }
    if (match(factor_set, "匹配数字,字符串,变量,true,false失败")) {
        TreeNode *factor_node = new TreeNode(TreeNode::Type::FACTOR, *token_it_);
        if (match({Token::Kind::ID})) {
            if (!sym_table_.find((*token_it_).value))
                throw ErrorMsg((*token_it_).line, (*token_it_).column, "使用了未声明的变量");
            else {
                Sym *sym = sym_table_.find((*token_it_).value);
                factor_node->val_type_ = sym->val_type;
            }
        }
        if (match({Token::Kind::NUM}))
            factor_node->val_type_ = ValType::VT_INT;

        if (match({Token::Kind::STR}))
            factor_node->val_type_ = ValType::VT_STRING;

        if (match({Token::Kind::TK_TRUE, Token::Kind::TK_FALSE}))
            factor_node->val_type_ = ValType::VT_BOOL;
        token_it_++;
        return factor_node;
    }
    // else {
    //     std::cout << "factor 没匹配错误处理" << std::endl;
    //     return nullptr;
    // }
    return nullptr;
}


TreeNode *GrammarAnalysis::repeat_stmt() {
    match_and_forward({Token::Kind::TK_REPEAT}, "Repeat块缺少repeat关键词");
    TreeNode *stmt_seq_node = stmt_sequence();
    match_and_forward({Token::Kind::TK_UNTIL}, "Repeat块缺少until关键词");
    TreeNode *log_or_exp_node = log_or_exp();
    return new TreeNode(TreeNode::Type::REPEAT_STMT, stmt_seq_node, log_or_exp_node);
}

TreeNode *GrammarAnalysis::read_stmt() {
    match_and_forward({Token::Kind::TK_READ}, "read块缺少read关键词");
    TreeNode *read_node = nullptr;
    if (match({Token::Kind::ID}, "read 后面跟变量")) {
        read_node = new TreeNode(TreeNode::Type::READ_STMT, *token_it_);
        token_it_++;
        match_and_forward({Token::Kind::TK_SEMICOLON}, "read语句最后以';'结尾");
        return read_node;
        // todo: how to handle ID ? as a child or store it to node.tk_
    }
    return nullptr;
}

TreeNode *GrammarAnalysis::write_stmt() {
    match_and_forward({Token::Kind::TK_WRITE}, "write块缺少write关键词");
    TreeNode *log_or_exp_node = log_or_exp();
    // todo: the end char of write stmt is ';'?
    match_and_forward({Token::Kind::TK_SEMICOLON}, "write语句最后以';'结尾");
    return new TreeNode(TreeNode::Type::WRITE_STMT, log_or_exp_node);
}

TreeNode *GrammarAnalysis::while_stmt() {
    match_and_forward({Token::Kind::TK_WHILE}, "缺少while关键词");
    TreeNode *log_or_exp_node = log_or_exp();
    match_and_forward({Token::Kind::TK_DO}, "while块缺少do关键词");
    TreeNode *stmt_seq_node = stmt_sequence();
    match_and_forward({Token::Kind::TK_END}, "while块缺少end关键词");
    return new TreeNode(TreeNode::Type::WHILE_STMT, log_or_exp_node, stmt_seq_node);
}
//
// void GrammarAnalysis::function_def() {
//     while (match({Token::Kind::TK_INT, Token::Kind::TK_BOOL, Token::Kind::TK_STRING})) {
//         token_forward();
//         // todo: store function_def to sym
//         match_and_forward({Token::Kind::ID});
//         match_and_forward({Token::Kind::TK_LP});
//         while (!match({Token::Kind::TK_RP})) {
//             parameters_list();
//         }
//         match_and_forward({Token::Kind::TK_RP});
//         declaration();
//         // todo: handle stmt_sequence
//         stmt_sequence();
//         match_and_forward({Token::Kind::TK_END});
//     }
//
// }
//
// void GrammarAnalysis::parameters_list() {
//     while (!match({Token::Kind::TK_RP})) {
//         while (true) {
//             parameter();
//             if (match({Token::Kind::TK_COMMA})) {
//                 token_forward();
//                 continue;
//             } else {
//                 if (match({Token::Kind::TK_RP}))
//                     break;
//                 else
//                     std::cout << "Error: please handle paramters list handle" << std::endl;
//             }
//         }
//     }
// }
//
// void GrammarAnalysis::parameter() {
//     match_and_forward({Token::Kind::TK_INT, Token::Kind::TK_BOOL, Token::Kind::TK_STRING});
//     std::cout << "parameter:" << cur_token_.second << std::endl;
//     match_and_forward({Token::Kind::ID});
// }
