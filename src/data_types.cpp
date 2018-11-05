//
// Created by JefungChan on 2018/11/4.
//

#include "data_types.h"

std::string Token::kind_names_[] = {
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

std::string Token::kind_name() {

    return kind_names_[static_cast<int>(kind)];
}

bool Token::is_KEY(const std::string &str) {
    return str == "true" || str == "false" || str == "or" ||
           str == "and" || str == "not" || str == "int" ||
           str == "bool" || str == "string" || str == "while" ||
           str == "do" || str == "if" || str == "then" ||
           str == "else" || str == "end" || str == "repeat"
           || str == "until" || str == "read" || str == "write";
}

void Token::to_thin() {
    if (kind == Kind::ID || kind == Kind::STR || kind == Kind::NUM)
        return;
    if (kind == Kind::KEY) {
        if (value == "true")
            kind = Kind::TK_TRUE;
        if (value == "false")
            kind = Kind::TK_FALSE;
        if (value == "or")
            kind = Kind::TK_OR;
        if (value == "and")
            kind = Kind::TK_AND;
        if (value == "not")
            kind = Kind::TK_NOT;
        if (value == "int")
            kind = Kind::TK_INT;
        if (value == "bool")
            kind = Kind::TK_BOOL;
        if (value == "string")
            kind = Kind::TK_STRING;
        if (value == "while")
            kind = Kind::TK_WHILE;
        if (value == "do")
            kind = Kind::TK_DO;
        if (value == "if")
            kind = Kind::TK_IF;
        if (value == "then")
            kind = Kind::TK_THEN;
        if (value == "else")
            kind = Kind::TK_ELSE;
        if (value == "end")
            kind = Kind::TK_END;
        if (value == "repeat")
            kind = Kind::TK_REPEAT;
        if (value == "until")
            kind = Kind::TK_UNTIL;
        if (value == "read")
            kind = Kind::TK_READ;
        if (value == "write")
            kind = Kind::TK_WRITE;
    }
    if (kind == Kind::SYM) {
        if (value == ">")
            kind = Kind::TK_GTR;
        if (value == "<=")
            kind = Kind::TK_LEQ;
        if (value == ">=")
            kind = Kind::TK_GEQ;
        if (value == ",")
            kind = Kind::TK_COMMA;
        if (value == ";")
            kind = Kind::TK_SEMICOLON;
        if (value == ":=")
            kind = Kind::TK_ASSIGN;
        if (value == "+")
            kind = Kind::TK_ADD;
        if (value == "-")
            kind = Kind::TK_SUB;
        if (value == "*")
            kind = Kind::TK_MUL;
        if (value == "/")
            kind = Kind::TK_DIV;
        if (value == "(")
            kind = Kind::TK_LP;
        if (value == ")")
            kind = Kind::TK_RP;
        if (value == "<")
            kind = Kind::TK_LSS;
        if (value == "=")
            kind = Kind::TK_EQU;
    }
}

std::string Token::kind_name(Token::Kind kind) {
    return kind_names_[static_cast<int>(kind)];
}

Token::Token(Token::Kind kind_, std::string value_, int line_, unsigned long column_) {
    kind = kind_;
    value = value_;
    line = line_;
    column = column_;
}

Token::Token() {

}

void Tokens::to_thin() {
    for (auto &token : tokens_) {
        token.to_thin();
    }
}

void Tokens::push(Token token) {
    tokens_.emplace_back(token);
}

void Tokens::clear() {
    tokens_.clear();
}

void Tokens::append(Tokens tokens) {
    tokens_.insert(tokens_.end(), tokens.begin(), tokens.end());
}

std::vector<Token>::iterator Tokens::begin() {
    return tokens_.begin();
}

std::vector<Token>::iterator Tokens::end() {
    return tokens_.end();
}

bool operator==(const Tokens &t1, const Tokens &t2) {
    if (t1.size() != t2.size())
        return false;
    unsigned long size = t1.size();
    for (int i = 0; i < size; i++) {
        if (t1[i].kind == t2[i].kind && t1[i].value == t2[i].value)
            continue;
        else
            return false;
    }
    return true;
}

unsigned long Tokens::size() const {
    return tokens_.size();
}

Token &Tokens::operator[](int index) {
    return tokens_[index];
}

Token Tokens::operator[](int index) const {
    return tokens_[index];
}

void Tokens::push(Token::Kind kind, std::string value, int line, unsigned long column) {
    Token token(kind, value, line, column);
    tokens_.emplace_back(token);
}

Token Tokens::back() {
    return tokens_.back();
}


std::string ErrorMsg::to_string() {
    return "在第" + std::to_string(line) + "行, 第" + std::to_string(column) + "列出错: " + msg;
}

void ErrorMsg::print() {
    std::cout << to_string() << std::endl;
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

std::vector<std::string> TreeNode::type_mapped_names = {
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

std::string TreeNode::get_type_name() {
    return type_mapped_names[type_];
}

std::string TreeNode::type_name(TreeNode::Type type) {
    return type_mapped_names[type];
}

Sym *SymTable::insert(std::string name) {
    if (table_.find(name) != table_.end())
        return nullptr;
    Sym *sym = new Sym();
    table_.insert({name, sym});
    return sym;
}

Sym *SymTable::find(std::string name) {
    if (table_.find(name) != table_.end())
        return table_[name];
    return nullptr;
}

void SymTable::del(std::string name) {
    auto it = table_.find(name);
    if (it != table_.end())
        table_.erase(it);
}

void SymTable::print() {
    for (auto it : table_) {

        std::cout << it.first << " ";
        if (it.second->val_type == ValType::VT_STRING)
            std::cout << "string类型" << std::endl;
        if (it.second->val_type == ValType::VT_INT)
            std::cout << "int类型   " << std::endl;
        if (it.second->val_type == ValType::VT_BOOL)
            std::cout << "bool类型  " << std::endl;
    }
}

int SymTable::size() {
    return table_.size();
}
