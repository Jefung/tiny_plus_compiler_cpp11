//
// Created by JefungChan on 2018/11/4.
//
#include "gtest/gtest.h"
#include "common.h"
#include "grammar_analysis.h"
#include "data_types.h"
#include "to_four_element_exp.h"

TEST(generate_middle_code, test_experiment2_test1_in) {
    Tokens thining_tokens;
    std::vector<ErrorMsg> msgs = get_thining_tokens_from_file("../../sample/experiment2_test1_in.txt",
                                                              thining_tokens);
    for (auto msg : msgs) {
        std::cout << msg.to_string() << std::endl;
    }
    ASSERT_TRUE(msgs.empty());
    ASSERT_TRUE(thining_tokens.size() != 0);
    GrammarAnalysis grammar(thining_tokens);
    ErrorMsgs err_msgs;
    TreeNode *root = grammar.program(err_msgs);
    ASSERT_EQ(err_msgs.size(), 0);

    ToFourElementExp t;
    auto res = t.convert(root);
    std::cout << "" << std::endl;
    for (auto line : res) {
        std::cout << line << std::endl;
    }

    // grammar.print_sym_table();
    // ASSERT_EQ(grammar.get_sym_table_size(), 7);
}

TEST(generate_middle_code, test_experiment2_test2_in) {
    Tokens thining_tokens;
    std::vector<ErrorMsg> msgs = get_thining_tokens_from_file("../../sample/experiment2_test2_in.txt",
                                                              thining_tokens);
    for (auto msg : msgs) {
        std::cout << msg.to_string() << std::endl;
    }
    ASSERT_TRUE(msgs.empty());
    ASSERT_TRUE(thining_tokens.size() != 0);
    GrammarAnalysis grammar(thining_tokens);
    ErrorMsgs err_msgs;
    TreeNode *root = grammar.program(err_msgs);
    ASSERT_EQ(err_msgs.size(), 0);

    ToFourElementExp t;
    auto res = t.convert(root);
    std::cout << "" << std::endl;
    for (auto line : res) {
        std::cout << line << std::endl;
    }
}
