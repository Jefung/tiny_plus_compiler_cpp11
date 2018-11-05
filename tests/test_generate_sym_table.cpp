//
// Created by JefungChan on 2018/11/4.
//
#include "gtest/gtest.h"
#include "common.h"
#include "grammar_analysis.h"
#include "data_types.h"

TEST(experiment2, test_generate_sym_table) {
    Tokens thining_tokens;
    std::vector<ErrorMsg> msgs = get_thining_tokens_from_file("../../sample/var_declaration.txt", thining_tokens);
    for (auto msg : msgs) {
        std::cout << msg.to_string() << std::endl;
    }
    ASSERT_TRUE(msgs.empty());
    ASSERT_TRUE(thining_tokens.size() != 0);
    GrammarAnalysis grammar(thining_tokens);
    ErrorMsgs err_msgs;
    grammar.program(err_msgs);
    ASSERT_EQ(err_msgs.size(), 0);

    // grammar.print_sym_table();
    ASSERT_EQ(grammar.get_sym_table_size(), 7);
}

TEST(error_throw, test_generate_sym_table) {
    Tokens thining_tokens;
    std::vector<ErrorMsg> msgs = get_thining_tokens_from_file("../../sample/error_var_declaration.txt", thining_tokens);
    for (auto msg : msgs) {
        std::cout << msg.to_string() << std::endl;
    }
    ASSERT_TRUE(msgs.empty());
    ASSERT_TRUE(thining_tokens.size() != 0);
    GrammarAnalysis grammar(thining_tokens);
    ErrorMsgs err_msgs;
    grammar.program(err_msgs);
    ASSERT_EQ(err_msgs.size(), 1);
    // grammar.print_sym_table();
    for (auto msg : err_msgs) {
        msg.print();
    }
    ASSERT_EQ(grammar.get_sym_table_size(), 5);
}