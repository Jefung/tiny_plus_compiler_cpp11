//
// Created by JefungChan on 2018/10/26.
//

#include <iostream>
#include <vector>
#include "data_type.h"

std::string Kind::name(Type type) {
    std::string arr[] = {
            "KEY", //保留字
            "SYM", //特殊符号
            "ID", //标识符
            "NUM", //数值常数
            "STR", //字符串常数
            "TK_TRUE",// 细化KEY,便于实验二操作
            "TK_FALSE",
            "TK_OR",
            "TK_AND",
            "TK_NOT",
            "TK_INT",
            "TK_BOOL",
            "TK_STRING",
            "TK_WHILE",
            "TK_DO",
            "TK_IF",
            "TK_THEN",
            "TK_ELSE",
            "TK_END",
            "TK_REPEAT",
            "TK_UNTIL",
            "TK_READ",
            "TK_WRITE",
            "TK_GTR",// 细化操作符,便于实验二操作
            "TK_LEQ",
            "TK_GEQ",
            "TK_COMMA",
            "TK_SEMICOLON",
            "TK_ASSIGN",
            "TK_ADD",
            "TK_SUB",
            "TK_MUL",
            "TK_DIV",
            "TK_LP",
            "TK_RP",
            "TK_LSS",
            "TK_EQU",};
    return arr[type];
}

// Type Kind::token_thining(Type type) {
//
// }

// return the string is key or not
bool Kind::is_KEY(const std::string &str) {
    return str == "true" || str == "false" || str == "or" ||
           str == "and" || str == "not" || str == "int" ||
           str == "bool" || str == "string" || str == "while" ||
           str == "do" || str == "if" || str == "then" ||
           str == "else" || str == "end" || str == "repeat"
           || str == "until" || str == "read" || str == "write";
}

std::pair<Kind::Type, std::string> Kind::token_thining(std::pair<Kind::Type, std::string> token) {
    Kind::Type type = token.first;
    std::string value = token.second;
    if (type == Kind::ID || type == Kind::STR || type == Kind::NUM)
        return token;
    if (type == Kind::KEY) {
        if (value == "true")
            return {Kind::TK_TRUE, value};
        if (value == "false")
            return {Kind::TK_FALSE, value};
        if (value == "or")
            return {Kind::TK_OR, value};
        if (value == "and")
            return {Kind::TK_AND, value};
        if (value == "not")
            return {Kind::TK_NOT, value};
        if (value == "int")
            return {Kind::TK_INT, value};
        if (value == "bool")
            return {Kind::TK_BOOL, value};
        if (value == "string")
            return {Kind::TK_STRING, value};
        if (value == "while")
            return {Kind::TK_WHILE, value};
        if (value == "do")
            return {Kind::TK_DO, value};
        if (value == "if")
            return {Kind::TK_IF, value};
        if (value == "then")
            return {Kind::TK_THEN, value};
        if (value == "else")
            return {Kind::TK_ELSE, value};
        if (value == "end")
            return {Kind::TK_END, value};
        if (value == "repeat")
            return {Kind::TK_REPEAT, value};
        if (value == "until")
            return {Kind::TK_UNTIL, value};
        if (value == "read")
            return {Kind::TK_READ, value};
        if (value == "write")
            return {Kind::TK_WRITE, value};
        throw "not match Kind::KEY value: " + value;
    }
    if (type == Kind::SYM) {
        if (value == ">")
            return {Kind::TK_GTR, value};
        if (value == "<=")
            return {Kind::TK_LEQ, value};
        if (value == ">=")
            return {Kind::TK_GEQ, value};
        if (value == ",")
            return {Kind::TK_COMMA, value};
        if (value == ";")
            return {Kind::TK_SEMICOLON, value};
        if (value == ":=")
            return {Kind::TK_ASSIGN, value};
        if (value == "+")
            return {Kind::TK_ADD, value};
        if (value == "-")
            return {Kind::TK_SUB, value};
        if (value == "*")
            return {Kind::TK_MUL, value};
        if (value == "/")
            return {Kind::TK_DIV, value};
        if (value == "(")
            return {Kind::TK_LP, value};
        if (value == ")")
            return {Kind::TK_RP, value};
        if (value == "<")
            return {Kind::TK_LSS, value};
        if (value == "=")
            return {Kind::TK_EQU, value};
        throw "not match Kind::SYM value: " + value;
    }
    return {};
}

std::vector<std::pair<Kind::Type, std::string>>
Kind::tokens_thining(std::vector<std::pair<Kind::Type, std::string>> tokens) {
    std::vector<std::pair<Kind::Type, std::string>> thining_tokens;
    for (auto token : tokens) {
        auto tmp = token_thining(token);
        thining_tokens.emplace_back(tmp);
    }
    return thining_tokens;
}


void ErrorMsg::print() {
    std::cout << to_string() << std::endl;
}

std::string ErrorMsg::to_string() {
    return "在第" + std::to_string(line) + "行, 第" + std::to_string(column) + "列出错: " + msg;
}

TreeNode::TreeNode(TreeNode::Type type, TreeNode *t1, TreeNode *t2, TreeNode *t3) {
    type_ = type;
    child_[0] = t1;
    child_[1] = t2;
    child_[2] = t3;
}

TreeNode::TreeNode(TreeNode::Type type, Token token) {
    type_ = type;
    tk_ = token;
}

std::string TreeNode::get_type_name(TreeNode::Type type) {
    std::vector<std::string> v = {
            "PROGRAM",        // 程序（开始符号）节点
            "STMT_SEQUENCE",  // 语句列表节点
            "IF_STMT",        // 条件语句节点
            "REPEAT_STMT",    // repeat语句节点
            "ASSIGN_STMT",    // 赋值语句节点
            "READ_STMT",      // read语句节点
            "WRITE_STMT",     // write语句节点
            "WHILE_STMT",     // while语句节点
            "GTR_EXP",        // 大于表达式节点
            "GEQ_EXP",        // 大于等于表达式节点
            "LSS_EXP",        // 小于表达式节点
            "LEQ_EXP",        // 小于等于表达式节点
            "EQU_EXP",        // 等于表达式节点
            "LOG_OR_EXP",     // 逻辑或表达式节点
            "LOG_AND_EXP",    // 逻辑与表达式节点
            "LOG_NOT_EXP",    // 逻辑非表达式节点
            "ADD_EXP",        // 加法表达式节点
            "SUB_EXP",        // 减法表达式节点
            "MUL_EXP",        // 乘法表达式节点
            "DIV_EXP",        // 除法表达式节点
            "FACTOR"
    };
    return v[type];
}

std::string TreeNode::type_name() {
    return get_type_name(type_);
}
