//
// Created by JefungChan on 2018/10/31.
//
#include <fstream>
#include "lexical_analysis.h"
#include "common.h"


Tokens get_tokens_by_regex(std::string content) {
    Tokens tokens;
    std::regex e("\\(([^ ]+?), ([^ ]+?)\\)", std::regex_constants::icase);
    std::smatch m;
    int i = 0;
    while (std::regex_search(content, m, e)) {
        i++;
        // std::cout << m[0] << " " << m[1] << "||" << m[2] << std::endl;
        if (Token::kind_name(Token::Kind::KEY) == m[1])
            tokens.push({Token::Kind::KEY, m[2]});
        if (Token::kind_name(Token::Kind::SYM) == m[1])
            tokens.push({Token::Kind::SYM, m[2]});
        if (Token::kind_name(Token::Kind::ID) == m[1])
            tokens.push({Token::Kind::ID, m[2]});
        if (Token::kind_name(Token::Kind::NUM) == m[1])
            tokens.push({Token::Kind::NUM, m[2]});
        if (Token::kind_name(Token::Kind::STR) == m[1])
            tokens.push({Token::Kind::STR, m[2]});
        content = m.suffix().str();  // 返回末端，作为新的搜索的开始
    }
    return tokens;
};


std::vector<ErrorMsg>
get_thining_tokens_from_file(std::string filename, Tokens &tokens) {
    std::ifstream f(filename);
    if (!f)
        throw "文件不存在: " + filename;
    std::istream is(f.rdbuf());
    LexicalAnalysis a;
    std::vector<ErrorMsg> msgs = a.transfer_token(is, tokens);
    tokens.to_thin();
    return msgs;
}

TreeNode *file_grammar_analysis(const std::string filename, ErrorMsgs &msgs) {
    Tokens tokens;
    std::ifstream f(filename);
    if (!f)
        throw "文件不存在: " + filename;
    std::istream is(f.rdbuf());
    LexicalAnalysis a;
    msgs = a.transfer_token(is, tokens);
    std::cout << "----" << std::endl;
    for (auto msg : msgs) {
        std::cout << msg.to_string() << std::endl;
        
    }
    tokens.to_thin();

    // 语法分析
    GrammarAnalysis grammar(tokens);
    ErrorMsgs grammar_err_msgs;
    TreeNode *root = grammar.program(grammar_err_msgs);
    msgs.insert(msgs.begin(), grammar_err_msgs.begin(), grammar_err_msgs.end());
    return root;
}