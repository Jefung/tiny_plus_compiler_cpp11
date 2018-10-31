//
// Created by JefungChan on 2018/10/26.
//

#include <iostream>
#include "data_type.h"

std::string Kind::name(Type type) {
    std::string arr[] = {"KEY", "SYM", "ID", "NUM", "STR"};
    return arr[type];
}

// return the string is key or not
bool Kind::is_KEY(const std::string &str) {
    return str == "true" || str == "false" || str == "or" ||
           str == "and" || str == "not" || str == "int" ||
           str == "bool" || str == "string" || str == "while" ||
           str == "do" || str == "if" || str == "then" ||
           str == "else" || str == "end" || str == "repeat"
           || str == "until" || str == "read" || str == "write";
}

void ErrorMsg::print() {
    std::cout << to_string() << std::endl;
}

std::string ErrorMsg::to_string() {
    return "在第" + std::to_string(line) + "行, 第" + std::to_string(column) + "列出错: " + msg;
}