//
// Created by JefungChan on 2018/11/1.
//

#ifndef TINY_COMPILER_CPP11_GRAMMAR_ANALYSIS_H
#define TINY_COMPILER_CPP11_GRAMMAR_ANALYSIS_H

#include "data_type.h"
#include <set>
#include <vector>

class GrammarAnalysis {
private:
    std::vector<std::pair<Kind::Type, std::string>> tokens_;
    Token cur_token_;
    int cur_index_;
    using KindSet = std::set<Kind::Type>;
public:
    explicit GrammarAnalysis(std::vector<std::pair<Kind::Type, std::string>> token);

    bool get_next_token(Token &token);

    void token_forward(std::string no_more_token_msg = "") throw(ErrorMsg);

    bool match(std::set<Kind::Type> kind_set);

    bool match_and_forward(std::set<Kind::Type> kind_set);

    TreeNode *program();

    void declaration();

    void function_def();

    void parameters_list();

    void parameter();

    TreeNode *stmt_sequence();

    TreeNode *if_stmt();

    TreeNode *repeat_stmt();

    TreeNode *assign_stmt();

    TreeNode *read_stmt();

    TreeNode *write_stmt();

    TreeNode *while_stmt();

    TreeNode *log_or_exp();

    TreeNode *log_and_exp();

    TreeNode *comparision_exp();

    TreeNode *add_exp();

    TreeNode *mul_exp();


    TreeNode *factor();

};

#endif //TINY_COMPILER_CPP11_GRAMMER_ANALYSIS_H
