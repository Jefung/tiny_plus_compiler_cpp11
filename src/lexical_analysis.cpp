//
// Created by JefungChan on 2018/10/26.
//

#include <iostream>
#include "lexical_analysis.h"


std::vector<ErrorMsg>
LexicalAnalysis::transfer_token(std::istream &is, std::vector<std::pair<Kind::Type, std::string>> &tokens) {
    int line_count = 1;
    int column_count = 0;
    std::string line;
    std::vector<ErrorMsg> err_msgs;
    Status status = Status::blank;
    std::vector<std::pair<Kind::Type, std::string>> cur_line_tokens;
    CharType c_type;
    char c;
    std::string word;
    while (is.get(c)) {
        column_count++;
        c_type = get_char_type(c);
        switch (status) {
            case Status::blank:
                switch (c_type) {
                    case CharType::c_newline:
                        line_count++;
                        column_count = 0;
                        tokens.insert(tokens.end(), cur_line_tokens.begin(), cur_line_tokens.end());
                        cur_line_tokens.clear();
                        break;
                    case CharType::c_blank:
                        break;
                    case CharType::c_number:
                        word += c;
                        status = Status::number;
                        break;
                    case CharType::c_letter:
                        word += c;
                        status = Status::letter;
                        break;
                    case CharType::c_single_quote:
                        status = Status::single_quote;
                        break;
                    case CharType::c_special_char:
                        if (c == '{') {
                            status = Status::comment;
                            break;
                        }
                        word = c;
                        status = Status::special_char;
                        break;
                    case CharType::c_illegal:
                        status = Status::error;
                        err_msgs.emplace_back(ErrorMsg(line_count, column_count, "非法字符"));
                        break;
                }
                break;
            case Status::number:
                switch (c_type) {
                    case CharType::c_newline:
                        line_count++;
                        column_count = 0;
                        cur_line_tokens.emplace_back(Kind::Type::NUM, word);
                        tokens.insert(tokens.end(), cur_line_tokens.begin(), cur_line_tokens.end());
                        cur_line_tokens.clear();
                        word = "";
                        status = Status::blank;
                        break;
                    case CharType::c_blank:
                        cur_line_tokens.emplace_back(Kind::Type::NUM, word);
                        word = "";
                        status = Status::blank;
                        break;
                    case CharType::c_number:
                        word += c;
                        break;
                    case CharType::c_letter:
                        err_msgs.emplace_back(ErrorMsg(line_count, column_count - word.size(), "数字加字母是不合法的组合"));
                        word = "";
                        status = Status::error;
                        break;
                    case CharType::c_single_quote:
                        cur_line_tokens.emplace_back(Kind::Type::NUM, word);
                        word = "";
                        status = Status::single_quote;
                        break;
                    case CharType::c_special_char:
                        cur_line_tokens.emplace_back(Kind::Type::NUM, word);
                        word = "";
                        if (c == '{') {
                            status = Status::comment;
                            break;
                        }
                        word = c;
                        status = Status::special_char;
                        break;
                    case CharType::c_illegal:
                        cur_line_tokens.emplace_back(Kind::Type::NUM, word);
                        word = "";
                        status = Status::error;
                        break;
                }
                break;
            case Status::letter:
                switch (c_type) {
                    case CharType::c_newline:
                        if (Kind::is_KEY(word))
                            cur_line_tokens.emplace_back(Kind::Type::KEY, word);
                        else
                            cur_line_tokens.emplace_back(Kind::Type::ID, word);
                        word = "";
                        line_count++;
                        column_count = 0;
                        tokens.insert(tokens.end(), cur_line_tokens.begin(), cur_line_tokens.end());
                        cur_line_tokens.clear();
                        status = Status::blank;
                        break;
                    case CharType::c_blank:
                        if (Kind::is_KEY(word))
                            cur_line_tokens.emplace_back(Kind::Type::KEY, word);
                        else
                            cur_line_tokens.emplace_back(Kind::Type::ID, word);
                        word = "";
                        status = Status::blank;
                        break;
                    case CharType::c_number:
                    case CharType::c_letter:
                        word += c;
                        break;
                    case CharType::c_single_quote:
                        if (Kind::is_KEY(word))
                            cur_line_tokens.emplace_back(Kind::Type::KEY, word);
                        else
                            cur_line_tokens.emplace_back(Kind::Type::ID, word);
                        word = "";
                        status = Status::single_quote;
                        break;
                    case CharType::c_special_char:
                        if (Kind::is_KEY(word))
                            cur_line_tokens.emplace_back(Kind::Type::KEY, word);
                        else
                            cur_line_tokens.emplace_back(Kind::Type::ID, word);
                        word = c;
                        status = Status::special_char;
                        break;
                    case CharType::c_illegal:
                        if (Kind::is_KEY(word))
                            cur_line_tokens.emplace_back(Kind::Type::KEY, word);
                        else
                            cur_line_tokens.emplace_back(Kind::Type::ID, word);
                        word = "";
                        status = Status::error;
                        err_msgs.emplace_back(ErrorMsg(line_count, column_count, "字母后面出现不合法的字符串"));
                        break;
                }
                break;
            case Status::single_quote:
                // std::cout << c << std::endl;
                switch (c_type) {
                    // todo: \n可以包含在字符串里面,当这里遇到字符串该怎么处理
                    case CharType::c_newline:
                        err_msgs.emplace_back(ErrorMsg(line_count, column_count - word.size(), "字符串标识符'未匹配"));
                        line_count++;
                        column_count = 0;
                        cur_line_tokens.clear();
                        status = Status::blank;
                        break;
                    case CharType::c_blank:
                    case CharType::c_number:
                    case CharType::c_letter:
                    case CharType::c_special_char:
                    case CharType::c_illegal:
                        word += c;
                        break;
                    case CharType::c_single_quote:
                        cur_line_tokens.emplace_back(Kind::Type::STR, word);
                        word = "";
                        status = Status::blank;
                        break;
                }
                break;
            case Status::special_char:
                if (word == "}") {
                    status = Status::error;
                    err_msgs.emplace_back(ErrorMsg(line_count, column_count - word.size(), "注释标识符缺少匹配"));
                    word = "";
                    cur_line_tokens.clear();
                    break;
                }
                switch (c_type) {
                    case CharType::c_newline:
                        line_count++;
                        column_count = 0;
                        cur_line_tokens.emplace_back(Kind::Type::SYM, word);
                        tokens.insert(tokens.end(), cur_line_tokens.begin(), cur_line_tokens.end());
                        cur_line_tokens.clear();
                        word = "";
                        status = Status::blank;
                        break;
                    case CharType::c_blank:
                        cur_line_tokens.emplace_back(Kind::Type::SYM, word);
                        word = "";
                        status = Status::blank;
                        break;
                    case CharType::c_number:
                        cur_line_tokens.emplace_back(Kind::Type::SYM, word);
                        word = c;
                        status = Status::number;
                        break;
                    case CharType::c_letter:
                        cur_line_tokens.emplace_back(Kind::Type::SYM, word);
                        word = c;
                        status = Status::letter;
                        break;
                    case CharType::c_single_quote:
                        cur_line_tokens.emplace_back(Kind::Type::SYM, word);
                        word = "";
                        status = Status::single_quote;
                        break;
                    case CharType::c_special_char:
                        if ((word == ":" || word == "<" || word == ">") && c == '=') {
                            word += c;
                            cur_line_tokens.emplace_back(Kind::Type::SYM, word);
                            word = "";
                            status = Status::blank;
                        } else {
                            word = "";
                            status = Status::error;
                            err_msgs.emplace_back(ErrorMsg(line_count, column_count, "非>=,<=,:=的双特殊字符"));
                        }
                        break;
                    case CharType::c_illegal:
                        cur_line_tokens.emplace_back(Kind::Type::SYM, word);
                        status = Status::error;
                        err_msgs.emplace_back(ErrorMsg(line_count, column_count, "特殊字符后面出现非法字符"));
                        break;
                }
                break;
            case Status::error:
                switch (c_type) {
                    case CharType::c_newline:
                        line_count++;
                        column_count = 0;
                        status = Status::blank;
                        break;
                    case CharType::c_blank:
                        status = Status::blank;
                        break;
                    case CharType::c_number:
                    case CharType::c_letter:
                    case CharType::c_single_quote:
                    case CharType::c_special_char:
                    case CharType::c_illegal:
                        break;
                }
                break;
            case Status::comment:
                word += c;
                // todo: 注释需要处理吗?
                if (c == '}') {
                    word = "";
                    status = Status::blank;
                } else {
                    if (is_newline(c)) {
                        err_msgs.emplace_back(ErrorMsg(line_count, column_count - word.size(), "注释标识符缺少匹配"));
                        word = "";
                        cur_line_tokens.clear();
                        line_count++;
                        column_count = 0;
                        status = Status::blank;
                    }
                }
                // todo: 注释中的换行是否算新行
                // if (is_newline(c)) {
                //     line_count++;
                //     column_count = 0;
                // }
                break;
        }
    }
    // 处理在文件尾的字符串
    switch (status) {
        case Status::blank:
            break;
        case Status::error:
        case Status::letter:
            if (Kind::is_KEY(word))
                tokens.emplace_back(Kind::Type::KEY, word);
            else
                tokens.emplace_back(Kind::Type::ID, word);
            break;
        case Status::special_char:
            tokens.emplace_back(Kind::SYM, word);
        case Status::single_quote:
            err_msgs.emplace_back(ErrorMsg(line_count, column_count - word.size(), "字符串缺少单引号匹配"));
            break;
        case Status::comment:
            err_msgs.emplace_back(ErrorMsg(line_count, column_count - word.size(), "注释标识符缺少匹配"));
            break;
        case Status::number:
            cur_line_tokens.emplace_back(Kind::NUM, word);
            break;
    }
    tokens.insert(tokens.end(), cur_line_tokens.begin(), cur_line_tokens.end());
    // for (auto token : tokens)
    //     std::cout << Kind::name(token.first) << " : " << token.second << std::endl;
    // for (auto msg : err_msgs)
    //     msg.print();
    return err_msgs;
}

