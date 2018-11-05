//
// Created by JefungChan on 2018/10/31.
//
#include <fstream>
#include <iostream>
#include "gtest/gtest.h"
#include "data_type_copy.h_copy"
#include "./common.h"
#include "lexical_analysis.h"
#include "grammar_analysis.h"

// TEST(experiment2, test_token_thining) {
//     // std::ifstream t("../../sample/experiment1_test2_out.txt");
//     // std::string content((std::istreambuf_iterator<char>(t)),
//     //                     std::istreambuf_iterator<char>());
//     // auto tokens = get_tokens_by_regex(content);
//     Token token({Kind::NUM, "111"});
//     EXPECT_EQ(Kind::token_thining(token), token);
//
//     Token token2({Kind::KEY, "string"});
//     EXPECT_EQ(Kind::token_thining(token2), Token({Kind::TK_STRING, "string"}));
//
//     Token token3({Kind::SYM, "<"});
//     EXPECT_EQ(Kind::token_thining(token3), Token({Kind::TK_LSS, "<"}));
//
//     // todo: add more token test
// }

// TEST(experiment2, test_tokens_thining) {
//     std::ifstream t("../../sample/experiment1_test2_out.txt");
//     // std::string content((std::istreambuf_iterator<char>(t)),
//     //                     std::istreambuf_iterator<char>());
//     // auto tokens = get_tokens_by_regex(content);
//     // auto thining_tokens = Kind::tokens_thining(tokens);
//
//     EXPECT_EQ(thining_tokens.size(), tokens.size());
//     EXPECT_FALSE(compare_tokens(tokens, thining_tokens));
//     // for (auto t  : thining_tokens) {
//     //     std::cout << t.first << " " << t.second << std::endl;
//     // }
// }

void postorder_traversal(TreeNode *root) {
    if (!root)
        return;
    postorder_traversal(root->child_[0]);
    postorder_traversal(root->child_[1]);
    postorder_traversal(root->child_[2]);
    std::cout << root->get_type_name() << std::endl;
}

void preorder_traversal(TreeNode *root) {
    if (!root)
        return;
    // if(root->child_[0])
    std::cout << root->get_type_name() << std::endl;
    preorder_traversal(root->child_[0]);
    preorder_traversal(root->child_[1]);
    preorder_traversal(root->child_[2]);
}

TEST(experiment2, test_if_tree) {
    ErrorMsgs errs;
    TreeNode *root = file_grammar_analysis("../../sample/if_then_assign_else_assign_end.txt", errs);

    ASSERT_TRUE(errs.empty());
    ASSERT_EQ(root->child_[1]->type_, TreeNode::Type::ASSIGN_STMT);
    ASSERT_EQ(root->child_[2]->type_, TreeNode::Type::ASSIGN_STMT);
    // postorder_traversal(root);
    preorder_traversal(root);
}

TEST(experiment2, test_if_if_tree) {
    ErrorMsgs errs;
    TreeNode *root = file_grammar_analysis("../../sample/if_if_end_end.txt", errs);
    ASSERT_EQ(errs.size(), 0);
    ASSERT_EQ(root->child_[1]->type_, TreeNode::Type::IF_STMT);
}

TEST(generate_grammar_tree, while_do_end) {
    ErrorMsgs errs;
    TreeNode *root = file_grammar_analysis("../../sample/while_do_end.txt", errs);
    for (auto msg : errs) {
        std::cout << msg.to_string() << std::endl;
    }
    ASSERT_EQ(errs.size(), 0);
    ASSERT_EQ(root->type_, TreeNode::Type::WHILE_STMT);
}
//
TEST(generate_grammar_tree, while_do_if_end_end_tree) {
    ErrorMsgs errs;
    TreeNode *root = file_grammar_analysis("../../sample/while_do_if_end_end.txt", errs);
    for (auto msg : errs) {
        std::cout << msg.to_string() << std::endl;
    }
    ASSERT_EQ(errs.size(), 0);
    ASSERT_EQ(root->child_[1]->type_, TreeNode::Type::IF_STMT);
}

TEST(generate_grammar_tree, repeat_until_tree) {
    ErrorMsgs errs;
    TreeNode *root = file_grammar_analysis("../../sample/repeat_until.txt", errs);
    for (auto msg : errs) {
        std::cout << msg.to_string() << std::endl;
    }
    ASSERT_EQ(errs.size(), 0);
    ASSERT_EQ(root->type_, TreeNode::Type::REPEAT_STMT);
}

TEST(generate_grammar_tree, read_tree) {
    ErrorMsgs errs;
    TreeNode *root = file_grammar_analysis("../../sample/read.txt", errs);
    for (auto msg : errs) {
        std::cout << msg.to_string() << std::endl;
    }
    ASSERT_EQ(errs.size(), 0);
    ASSERT_EQ(root->child_[0]->type_, TreeNode::Type::READ_STMT);
}

TEST(generate_grammar_tree, write_tree) {
    ErrorMsgs errs;
    TreeNode *root = file_grammar_analysis("../../sample/write.txt", errs);
    for (auto msg : errs) {
        std::cout << msg.to_string() << std::endl;
    }
    ASSERT_EQ(errs.size(), 0);
    ASSERT_EQ(root->child_[0]->type_, TreeNode::Type::WRITE_STMT);
}

TEST(generate_grammar_tree, experiment2_test1_in) {
    ErrorMsgs errs;
    TreeNode *root = file_grammar_analysis("../../sample/experiment2_test1_in.txt", errs);
    for (auto msg : errs) {
        std::cout << msg.to_string() << std::endl;
    }
    ASSERT_EQ(errs.size(), 0);
    ASSERT_EQ(root->type_, TreeNode::Type::WHILE_STMT);
    ASSERT_EQ(root->child_[0]->type_, TreeNode::Type::LOG_AND_EXP);
    ASSERT_EQ(root->child_[1]->type_, TreeNode::Type::IF_STMT);
}

TEST(generate_grammar_tree, experiment2_test2_in) {
    ErrorMsgs errs;
    TreeNode *root = file_grammar_analysis("../../sample/experiment2_test2_in.txt", errs);
    for (auto msg : errs) {
        std::cout << msg.to_string() << std::endl;
    }
    std::cout << std::endl;
    ASSERT_EQ(errs.size(), 0);
    ASSERT_EQ(root->type_, TreeNode::Type::STMT_SEQUENCE);
    ASSERT_EQ(root->child_[0]->type_, TreeNode::Type::READ_STMT);
    ASSERT_EQ(root->child_[1]->type_, TreeNode::Type::IF_STMT);
}
