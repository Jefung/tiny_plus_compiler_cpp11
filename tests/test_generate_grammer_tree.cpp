//
// Created by JefungChan on 2018/10/31.
//
#include <fstream>
#include <iostream>
#include "gtest/gtest.h"
#include "data_type.h"
#include "./common.h"
#include "lexical_analysis.h"
#include "grammar_analysis.h"

TEST(experiment2, test_token_thining) {
    // std::ifstream t("../../sample/experiment1_test2_out.txt");
    // std::string content((std::istreambuf_iterator<char>(t)),
    //                     std::istreambuf_iterator<char>());
    // auto tokens = get_tokens_by_regex(content);
    Token token({Kind::NUM, "111"});
    EXPECT_EQ(Kind::token_thining(token), token);

    Token token2({Kind::KEY, "string"});
    EXPECT_EQ(Kind::token_thining(token2), Token({Kind::TK_STRING, "string"}));

    Token token3({Kind::SYM, "<"});
    EXPECT_EQ(Kind::token_thining(token3), Token({Kind::TK_LSS, "<"}));

    // todo: add more token test
}

TEST(experiment2, test_tokens_thining) {
    std::ifstream t("../../sample/experiment1_test2_out.txt");
    std::string content((std::istreambuf_iterator<char>(t)),
                        std::istreambuf_iterator<char>());
    auto tokens = get_tokens_by_regex(content);
    auto thining_tokens = Kind::tokens_thining(tokens);
    EXPECT_EQ(thining_tokens.size(), tokens.size());
    EXPECT_FALSE(compare_tokens(tokens, thining_tokens));
    // for (auto t  : thining_tokens) {
    //     std::cout << t.first << " " << t.second << std::endl;
    // }
}

void postorder_traversal(TreeNode *root) {
    if (!root)
        return;
    postorder_traversal(root->child_[0]);
    postorder_traversal(root->child_[1]);
    postorder_traversal(root->child_[2]);
    std::cout << root->type_name() << std::endl;
}

TEST(experiment2, test_if_tree) {
    std::ifstream f("../../sample/experiment2_test3_in.txt");
    std::istream is(f.rdbuf());
    LexicalAnalysis a;
    std::vector<std::pair<Kind::Type, std::string>> analysis_tokens;
    std::vector<ErrorMsg> msgs = a.transfer_token(is, analysis_tokens);
    // 输出错误消息
    // std::cout << msgs.size() << std::endl;
    for (auto msg : msgs) {
        std::cout << msg.to_string() << std::endl;
    }
    std::cout << analysis_tokens.size() << std::endl;
    auto tokens = Kind::tokens_thining(analysis_tokens);
    std::cout << tokens.size() << std::endl;
    GrammarAnalysis grammar(tokens);
    try {
        auto tree = grammar.program();
        postorder_traversal(tree);
    } catch (ErrorMsg e) {
        std::cout << "---------------" << std::endl;
        std::cout << e.to_string() << std::endl;
    }

}

TEST(experiment2, test_if_if_tree) {
    std::ifstream f("../../sample/experiment2_test4_in.txt");
    std::istream is(f.rdbuf());
    LexicalAnalysis a;
    std::vector<std::pair<Kind::Type, std::string>> analysis_tokens;
    std::vector<ErrorMsg> msgs = a.transfer_token(is, analysis_tokens);
    // 输出错误消息
    // std::cout << msgs.size() << std::endl;

    auto tokens = Kind::tokens_thining(analysis_tokens);

    ASSERT_EQ(tokens.size(), analysis_tokens.size());
    GrammarAnalysis grammar(tokens);
    try {
        auto tree = grammar.program();
        postorder_traversal(tree);
    } catch (ErrorMsg e) {
        std::cout << e.to_string() << std::endl;
    }

}

TEST(generate_grammar_tree, if_else_tree) {
    std::ifstream f("../../sample/if_else.txt");
    std::istream is(f.rdbuf());
    LexicalAnalysis a;
    std::vector<std::pair<Kind::Type, std::string>> analysis_tokens;
    std::vector<ErrorMsg> msgs = a.transfer_token(is, analysis_tokens);
    // 输出错误消息
    // std::cout << msgs.size() << std::endl;

    auto tokens = Kind::tokens_thining(analysis_tokens);

    ASSERT_EQ(tokens.size(), analysis_tokens.size());
    GrammarAnalysis grammar(tokens);
    try {
        auto tree = grammar.program();
        ASSERT_TRUE(tree->child_[0] != nullptr);
        ASSERT_TRUE(tree->child_[1] != nullptr);
        ASSERT_TRUE(tree->child_[2] != nullptr);
        postorder_traversal(tree->child_[2]);
    } catch (ErrorMsg e) {
        std::cout << e.to_string() << std::endl;
    }
}

TEST(generate_grammar_tree, while_then_end_tree) {
    std::vector<std::pair<Kind::Type, std::string>> thining_tokens;
    std::vector<ErrorMsg> msgs = get_thining_tokens_from_file("../../sample/while_do_end.txt", thining_tokens);
    // for (auto msg : msgs) {
    //     std::cout << msg.to_string() << std::endl;
    // }
    ASSERT_TRUE(msgs.empty());
    // for (auto token : thining_tokens) {
    //     std::cout << Kind::name(token.first) << token.second << std::endl;
    // }
    GrammarAnalysis grammar(thining_tokens);
    try {
        auto tree = grammar.program();
        ASSERT_TRUE(tree->child_[0] != nullptr);
        ASSERT_TRUE(tree->child_[1] != nullptr);
        // postorder_traversal(tree->child_[1]);
    } catch (ErrorMsg e) {
        std::cout << e.to_string() << std::endl;
    }
}

TEST(generate_grammar_tree, while_do_if_end_end_tree) {
    std::vector<std::pair<Kind::Type, std::string>> thining_tokens;
    std::vector<ErrorMsg> msgs = get_thining_tokens_from_file("../../sample/while_do_if_end_end.txt", thining_tokens);
    // for (auto msg : msgs) {
    //     std::cout << msg.to_string() << std::endl;
    // }
    ASSERT_TRUE(msgs.empty());
    // for (auto token : thining_tokens) {
    //     std::cout << Kind::name(token.first) << token.second << std::endl;
    // }
    GrammarAnalysis grammar(thining_tokens);
    try {
        auto tree = grammar.program();
        ASSERT_TRUE(tree->type_ == TreeNode::Type::WHILE_STMT);
        ASSERT_TRUE(tree->child_[1]->type_ == TreeNode::Type::IF_STMT);
        ASSERT_TRUE(tree->child_[1]->child_[0]->type_ == TreeNode::Type::EQU_EXP);
        ASSERT_TRUE(tree->child_[1]->child_[1]->type_ == TreeNode::Type::ASSIGN_STMT);
    } catch (ErrorMsg e) {
        std::cout << e.to_string() << std::endl;
    }
}

TEST(generate_grammar_tree, repeat_until_tree) {
    std::vector<std::pair<Kind::Type, std::string>> thining_tokens;
    std::vector<ErrorMsg> msgs = get_thining_tokens_from_file("../../sample/repeat_until.txt", thining_tokens);
    for (auto msg : msgs) {
        std::cout << msg.to_string() << std::endl;
    }
    ASSERT_TRUE(msgs.empty());
    for (auto token : thining_tokens) {
        std::cout << Kind::name(token.first) << "  " << token.second << std::endl;
    }
    GrammarAnalysis grammar(thining_tokens);
    try {
        auto tree = grammar.program();
        // ASSERT_TRUE(tree->type_ == TreeNode::Type::REPEAT_STMT);
        // ASSERT_TRUE(tree->child_[0]->type_ == TreeNode::Type::ASSIGN_STMT);
        // std::cout <<  << std::endl;
        // ASSERT_TRUE(tree->child_[1]->type_ == TreeNode::Type::LEQ_EXP);
        postorder_traversal(tree);

    } catch (ErrorMsg e) {
        std::cout << e.to_string() << std::endl;
    }
}

TEST(generate_grammar_tree, read_tree) {
    std::vector<std::pair<Kind::Type, std::string>> thining_tokens;
    std::vector<ErrorMsg> msgs = get_thining_tokens_from_file("../../sample/read.txt", thining_tokens);
    for (auto msg : msgs) {
        std::cout << msg.to_string() << std::endl;
    }
    ASSERT_TRUE(msgs.empty());
    // for (auto token : thining_tokens) {
    //     std::cout << Kind::name(token.first) << "  " << token.second << std::endl;
    // }

    GrammarAnalysis grammar(thining_tokens);
    try {
        auto tree = grammar.program();
        ASSERT_EQ(tree->type_, TreeNode::Type::STMT_SEQUENCE);
        ASSERT_EQ(tree->child_[0]->type_, TreeNode::Type::READ_STMT);
        ASSERT_EQ(tree->child_[1]->type_, TreeNode::Type::READ_STMT);
        // postorder_traversal(tree);
    } catch (ErrorMsg e) {
        std::cout << e.to_string() << std::endl;
    }
}

TEST(generate_grammar_tree, write_tree) {
    std::vector<std::pair<Kind::Type, std::string>> thining_tokens;
    std::vector<ErrorMsg> msgs = get_thining_tokens_from_file("../../sample/write.txt", thining_tokens);
    for (auto msg : msgs) {
        std::cout << msg.to_string() << std::endl;
    }
    ASSERT_TRUE(msgs.empty());
    // for (auto token : thining_tokens) {
    //     std::cout << Kind::name(token.first) << "  " << token.second << std::endl;
    // }

    GrammarAnalysis grammar(thining_tokens);
    try {
        auto tree = grammar.program();
        ASSERT_EQ(tree->type_, TreeNode::Type::STMT_SEQUENCE);
        ASSERT_EQ(tree->child_[0]->type_, TreeNode::Type::WRITE_STMT);
        ASSERT_EQ(tree->child_[1]->type_, TreeNode::Type::WRITE_STMT);
        // postorder_traversal(tree);
    } catch (ErrorMsg e) {
        std::cout << e.to_string() << std::endl;
    }
}

TEST(generate_grammar_tree, function_def) {
    std::vector<std::pair<Kind::Type, std::string>> thining_tokens;
    std::vector<ErrorMsg> msgs = get_thining_tokens_from_file("../../sample/function_def_list.txt", thining_tokens);
    for (auto msg : msgs) {
        std::cout << msg.to_string() << std::endl;
    }
    ASSERT_TRUE(msgs.empty());
    ASSERT_FALSE(thining_tokens.empty());
    // for (auto token : thining_tokens) {
    //     std::cout << Kind::name(token.first) << "  " << token.second << std::endl;
    // }

    GrammarAnalysis grammar(thining_tokens);
    try {
        auto tree = grammar.program();
        // ASSERT_EQ(tree->type_, TreeNode::Type::STMT_SEQUENCE);
        // ASSERT_EQ(tree->child_[0]->type_, TreeNode::Type::WRITE_STMT);
        // ASSERT_EQ(tree->child_[1]->type_, TreeNode::Type::WRITE_STMT);
        // postorder_traversal(tree);
    } catch (ErrorMsg e) {
        std::cout << e.to_string() << std::endl;
    }
}

TEST(experiment2, test1) {
    std::vector<std::pair<Kind::Type, std::string>> thining_tokens;
    std::vector<ErrorMsg> msgs = get_thining_tokens_from_file("../../sample/experiment2_test1_in.txt", thining_tokens);
    for (auto msg : msgs) {
        std::cout << msg.to_string() << std::endl;
    }
    ASSERT_TRUE(msgs.empty());
    ASSERT_FALSE(thining_tokens.empty());
    // for (auto token : thining_tokens) {
    //     std::cout << Kind::name(token.first) << "  " << token.second << std::endl;
    // }

    GrammarAnalysis grammar(thining_tokens);
    try {
        auto tree = grammar.program();
        // ASSERT_EQ(tree->type_, TreeNode::Type::STMT_SEQUENCE);
        // ASSERT_EQ(tree->child_[0]->type_, TreeNode::Type::WRITE_STMT);
        // ASSERT_EQ(tree->child_[1]->type_, TreeNode::Type::WRITE_STMT);
        postorder_traversal(tree);
    } catch (ErrorMsg e) {
        std::cout << e.to_string() << std::endl;
    }
}