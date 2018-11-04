//
// Created by JefungChan on 2018/10/31.
//

#ifndef TINY_COMPILER_CPP11_COMMON_H
#define TINY_COMPILER_CPP11_COMMON_H

#include <regex>
#include <vector>
#include "data_type.h"

bool compare_tokens(const std::vector<std::pair<Kind::Type, std::string>> &t1,
                    const std::vector<std::pair<Kind::Type, std::string>> &t2);

std::vector<ErrorMsg>
get_thining_tokens_from_file(std::string filename, std::vector<std::pair<Kind::Type, std::string>>& tokens);

std::vector<std::pair<Kind::Type, std::string>> get_tokens_by_regex(std::string content);

#endif //TINY_COMPILER_CPP11_COMMON_H
