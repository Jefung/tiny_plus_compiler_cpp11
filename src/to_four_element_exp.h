//
// Created by JefungChan on 2018/11/4.
//

#ifndef TINY_COMPILER_CPP11_TO_FOUR_ELEMENT_EXP_H
#define TINY_COMPILER_CPP11_TO_FOUR_ELEMENT_EXP_H

#include "data_types.h"

class ToFourElementExp {
private:
    int label_num = 0;
    int var_num = 1;
    std::vector<std::string> res_;

public:
    std::vector<std::string> convert(TreeNode *root);

    // void preorder_traversal(TreeNode *root, std::vector<std::string> &v);
    void stmt_seq_node_handle(TreeNode *root);

    void stmt_node_handle(TreeNode *root);

    void if_node_handle(TreeNode *root, std::string end_label);

    void while_node_handle(TreeNode *root, std::string end_label);

    void read_node_handle(TreeNode *root);

    void write_node_handle(TreeNode *root);

    void repeat_node_handle(TreeNode *root, std::string end_label);

    void assign_node_handle(TreeNode *root);

    void cond_exp_handle(TreeNode *root, std::string true_label, std::string false_label);

    std::string log_or_handle(TreeNode *root);

    std::string log_and_handle(TreeNode *root);

    std::string comparison_exe_handle(TreeNode *root);

    std::string add_exp_handle(TreeNode *root);

    std::string mul_exe_handle(TreeNode *root);

    std::string factor_handle(TreeNode *root);


    std::string get_new_label();

    std::string get_var_name();
};

#endif //TINY_COMPILER_CPP11_TO_FOUR_ELEMENT_EXP_H
