#include <fstream>
#include <iostream>
#include "gtest/gtest.h"
#include "data_type_copy.h_copy"
#include "./common.h"
#include "lexical_analysis.h"
#include "grammar_analysis.h"

TEST(grammar_error, test_if_tree_error) {
    ErrorMsgs errs;
    TreeNode *root = file_grammar_analysis("../../sample/err_if_then_assign_else_assign_end.txt", errs);

    ASSERT_EQ(errs.size(), 1);
    ASSERT_EQ(root, nullptr);
}

TEST(grammar_error, test_var_no_declare) {
    ErrorMsgs errs;
    TreeNode *root = file_grammar_analysis("../../sample/err_var_no_declare.txt", errs);

    ASSERT_EQ(errs.size(), 1);
    for (auto msg : errs) {
        std::cout << msg.to_string() << std::endl;
    }
    ASSERT_EQ(root, nullptr);
}

TEST(grammar_error, test_no_assign_symbol) {
    ErrorMsgs errs;
    TreeNode *root = file_grammar_analysis("../../sample/err_no_assign_symbol.txt", errs);

    ASSERT_EQ(errs.size(), 1);
    for (auto msg : errs) {
        std::cout << msg.to_string() << std::endl;
    }
    ASSERT_EQ(root, nullptr);
}

TEST(grammar_error, test_err_diff_var_add) {
    ErrorMsgs errs;
    TreeNode *root = file_grammar_analysis("../../sample/err_diff_var_add.txt", errs);

    ASSERT_EQ(errs.size(), 1);
    for (auto msg : errs) {
        std::cout << msg.to_string() << std::endl;
    }
    ASSERT_EQ(root, nullptr);
}

TEST(grammar_error, test_err_not_int_var_mul) {
    ErrorMsgs errs;
    TreeNode *root = file_grammar_analysis("../../sample/err_not_int_var_mul.txt", errs);

    ASSERT_EQ(errs.size(), 1);
    for (auto msg : errs) {
        std::cout << msg.to_string() << std::endl;
    }
    ASSERT_EQ(root, nullptr);
}

TEST(grammar_error, test_err_diff_type_assign) {
    ErrorMsgs errs;
    TreeNode *root = file_grammar_analysis("../../sample/err_diff_type_assign.txt", errs);
    ASSERT_EQ(errs.size(), 1);
    for (auto msg : errs) {
        std::cout << msg.to_string() << std::endl;
    }
    ASSERT_EQ(root, nullptr);
}