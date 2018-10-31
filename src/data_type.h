//
// Created by JefungChan on 2018/10/26.
//

#ifndef TINY_PLUS_COMPILER_DATA_TYPE_H
#define TINY_PLUS_COMPILER_DATA_TYPE_H

#include <string>

class Kind {
public:
    enum Type {
        KEY, //保留字
        SYM, //特殊符号
        ID, //标识符
        NUM, //数值常数
        STR, //字符串常数
    };

    // print enum name
    static std::string name(Type type);
    // return the string is key or not
    static bool is_KEY(const std::string &str) ;
};

struct ErrorMsg {
    int line;
    int column;
    std::string msg;

    ErrorMsg(int line_, int column_, std::string msg_) : line(line_), column(column_), msg(msg_) {}
    void print();
    std::string to_string();
};

#endif //TINY_PLUS_COMPILER_DATA_TYPE_H
