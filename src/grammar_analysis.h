//
// Created by JefungChan on 2018/11/1.
//

#ifndef TINY_COMPILER_CPP11_GRAMMAR_ANALYSIS_H
#define TINY_COMPILER_CPP11_GRAMMAR_ANALYSIS_H

#include <set>
#include <vector>
#include "data_types.h"

class GrammarAnalysis {
private:
    Tokens tokens_;
    std::vector<Token>::iterator token_it_;
    SymTable sym_table_;
    using KindSet = std::set<Token::Kind>;

public:
    explicit GrammarAnalysis(Tokens token);

    bool match(KindSet kind_set, std::string eof_error_msg = "") throw(ErrorMsg);

    bool match_and_forward(KindSet kind_set, std::string eof_error_msg = "") throw(ErrorMsg);

    void print_sym_table();

    int get_sym_table_size();

    TreeNode *program(ErrorMsgs &error_msg);

    void declaration();

    // void function_def();
    //
    // void parameters_list();
    //
    // void parameter();
    //
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
