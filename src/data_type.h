//
// Created by JefungChan on 2018/10/26.
//

#ifndef TINY_PLUS_COMPILER_DATA_TYPE_H
#define TINY_PLUS_COMPILER_DATA_TYPE_H

#include <string>
#include <vector>

class Kind {
public:
    enum Type {
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

    // print enum name
    static std::string name(Type type);

    // return the string is key or not
    static bool is_KEY(const std::string &str);

    static std::pair<Kind::Type, std::string> token_thining(std::pair<Kind::Type, std::string> type);

    using Token  = std::pair<Type, std::string>;

    static std::vector<Token>
    tokens_thining(std::vector<std::pair<Type, std::string>>);
};


struct ErrorMsg {
    int line;
    int column;
    std::string msg;

    ErrorMsg(int line_, int column_, std::string msg_) : line(line_), column(column_), msg(msg_) {}

    void print();

    std::string to_string();
};

typedef std::pair<Kind::Type, std::string> Token;
using Tokens = std::vector<std::pair<Kind::Type, std::string>>;

class Sym {

};

enum ValType {
    VT_INT,         // 整型数类型
    VT_BOOL,        // 布尔类型
    VT_STRING,      // 字符串类型
};

class TreeNode {
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

    std::string get_type_name(Type type);

    std::string type_name();
};

#endif //TINY_PLUS_COMPILER_DATA_TYPE_H
