//
// Created by JefungChan on 2018/10/31.
//

#ifndef TINY_COMPILER_CPP11_COMMON_H
#define TINY_COMPILER_CPP11_COMMON_H

#include <regex>
#include <vector>
#include "data_types.h"
#include "grammar_analysis.h"


std::vector<ErrorMsg>
get_thining_tokens_from_file(std::string filename, Tokens &tokens);

Tokens get_tokens_by_regex(std::string content);
TreeNode *file_grammar_analysis(const std::string filename, ErrorMsgs &msgs);
#endif //TINY_COMPILER_CPP11_COMMON_H
