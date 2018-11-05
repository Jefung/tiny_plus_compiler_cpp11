//
// Created by JefungChan on 2018/10/26.
//

#ifndef TINY_PLUS_COMPILER_LEXICAL_ANALYSIS_H
#define TINY_PLUS_COMPILER_LEXICAL_ANALYSIS_H

#include <fstream>
#include <vector>
#include <map>
#include "data_types.h"


class LexicalAnalysis {
private:
    enum Status {
        blank,
        number,
        letter,
        single_quote,
        special_char,
        error,
        comment,
    };
    enum CharType {
        c_blank,
        c_number,
        c_letter,
        c_single_quote,
        c_special_char,
        c_newline,
        c_illegal,
    };

    // std::vector<std::string> get_token_from_line(std::string line, std::map);
    bool is_blank(char c) {
        return c == 0x20 || c == 0x09 || c == 0x0B || c == 0x0C || c == 0x0D;
    }

    bool is_newline(char c) {
        return c == 0x0A;
    }

    bool is_number(char c) {
        return c >= '0' && c <= '9';
    }

    bool is_letter(char c) {
        return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
    }

    CharType get_char_type(char c) {
        if (is_newline(c))
            return CharType::c_newline;
        if (is_number(c))
            return CharType::c_number;
        if (is_blank(c))
            return CharType::c_blank;
        if (is_special_symbol(c))
            return CharType::c_special_char;
        if (is_single_quote(c))
            return CharType::c_single_quote;
        if (is_letter(c))
            return CharType::c_letter;
        return CharType::c_illegal;
    }

    bool is_special_symbol(char c) {
        return c == ':' || c == ',' || c == ';' ||
               c == '<' || c == '>' || c == '=' ||
               c == '+' || c == '-' || c == '*' ||
               c == '/' || c == '(' || c == ')' ||
               c == '{' || c == '}';
    }

    bool is_single_quote(char c) {
        return c == '\'';
    }

    bool is_illegal_character(char c) {
        return is_number(c) || is_letter(c) || is_special_symbol(c) || is_single_quote(c);
    }

public:
    // return 错误信息 行号,列号 -> 错误信息
    std::vector<ErrorMsg> transfer_token(std::istream &is, Tokens &tokens);

};

#endif //TINY_PLUS_COMPILER_LEXICAL_ANALYSIS_H
