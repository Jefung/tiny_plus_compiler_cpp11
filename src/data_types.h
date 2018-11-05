//
// Created by JefungChan on 2018/11/4.
//

#ifndef TINY_COMPILER_CPP11_DATA_TYPES_H
#define TINY_COMPILER_CPP11_DATA_TYPES_H


#include <string>
#include <vector>
#include <iostream>
#include <map>

class Token {
private:
    static std::string kind_names_[];
public:
    enum class Kind : char {
        KEY, //保留字
        SYM, //特殊符号
        ID, //标识符
        NUM, //数值常数
        STR, //字符串常数
        TK_TRUE,// 细化KEY,便于实验二操作
        TK_FALSE,
        TK_OR,
        TK_AND,
        TK_NOT,
        TK_INT,
        TK_BOOL,
        TK_STRING,
        TK_WHILE,
        TK_DO,
        TK_IF,
        TK_THEN,
        TK_ELSE,
        TK_END,
        TK_REPEAT,
        TK_UNTIL,
        TK_READ,
        TK_WRITE,
        TK_GTR,// 细化操作符,便于实验二操作
        TK_LEQ,
        TK_GEQ,
        TK_COMMA,
        TK_SEMICOLON,
        TK_ASSIGN,
        TK_ADD,
        TK_SUB,
        TK_MUL,
        TK_DIV,
        TK_LP,
        TK_RP,
        TK_LSS,
        TK_EQU,
        END_OF_FILE,
    };
    Kind kind;
    std::string value;
    int line;
    unsigned long column;

    Token(Kind kind, std::string value, int line = 0, unsigned long column = 0);

    Token();

    std::string kind_name();

    static std::string kind_name(Kind kind);

    static bool is_KEY(const std::string &str);

    void to_thin();
};

class Tokens {
private:
    std::vector<Token> tokens_;
public:
    void to_thin();

    void push(Token token);

    void push(Token::Kind kind, std::string value, int line = 0, unsigned long column = 0);

    void clear();

    void append(Tokens tokens);

    std::vector<Token>::iterator begin();

    std::vector<Token>::iterator end();

    Token back();

    friend bool operator==(const Tokens &t1, const Tokens &t2);

    unsigned long size() const;

    Token &operator[](int index);

    Token operator[](int index) const;

};

struct ErrorMsg {
    int line;
    int column;
    std::string msg;

    ErrorMsg(int line_, int column_, std::string msg_) : line(line_), column(column_), msg(msg_) {}

    void print();

    std::string to_string();
};

using ErrorMsgs = std::vector<ErrorMsg>;
enum ValType {
    VT_INT,         // 整型数类型
    VT_BOOL,        // 布尔类型
    VT_STRING,      // 字符串类型
};
enum ObjType {
    OT_FUN,         // 函数
    OT_VAR,         // 变量
    OT_CONST,       // 常量
};

struct Sym {
    Token tk;          // 符号(词法分析器的返回值)
    ObjType obj_type;   // 符号对象类型：函数/变量/常量
    ValType val_type;   // 值类型: int/bool/string
    // int addr;     // 地址。
    // int size;     // 字节数
    // int level;     // 层次
};

class SymTable {
private:
    std::map<std::string, Sym *> table_;
public:
    Sym *insert(std::string name);

    Sym *find(std::string name);

    void del(std::string name);

    int size();

    void print();

};


class TreeNode {
private:
    static std::vector<std::string> type_mapped_names;
public:
    enum Type {
        PROGRAM,        // 程序（开始符号）节点
        STMT_SEQUENCE,  // 语句列表节点
        IF_STMT,        // 条件语句节点
        REPEAT_STMT,    // repeat语句节点
        ASSIGN_STMT,    // 赋值语句节点
        READ_STMT,      // read语句节点
        WRITE_STMT,     // write语句节点
        WHILE_STMT,     // while语句节点
        GTR_EXP,        // 大于表达式节点
        GEQ_EXP,        // 大于等于表达式节点
        LSS_EXP,        // 小于表达式节点
        LEQ_EXP,        // 小于等于表达式节点
        EQU_EXP,        // 等于表达式节点
        LOG_OR_EXP,     // 逻辑或表达式节点
        LOG_AND_EXP,    // 逻辑与表达式节点
        LOG_NOT_EXP,    // 逻辑非表达式节点
        ADD_EXP,        // 加法表达式节点
        SUB_EXP,        // 减法表达式节点
        MUL_EXP,        // 乘法表达式节点
        DIV_EXP,        // 除法表达式节点
        FACTOR          // 原子节点
    };
    Type type_;
    ValType val_type_;
    TreeNode *child_[3] = {nullptr, nullptr, nullptr};
    Token tk_;

    TreeNode(Type type, TreeNode *t1 = nullptr, TreeNode *t2 = nullptr, TreeNode *t3 = nullptr);

    TreeNode(Type type, Token token);

    std::string get_type_name();

    static std::string type_name(Type type);
};

#endif //TINY_COMPILER_CPP11_DATA_TYPES_H
