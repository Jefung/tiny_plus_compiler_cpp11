//
// Created by JefungChan on 2018/10/31.
//
#include <fstream>
#include "lexical_analysis.h"
#include "common.h"

bool compare_tokens(const std::vector<std::pair<Kind::Type, std::string>> &t1,
                    const std::vector<std::pair<Kind::Type, std::string>> &t2) {
    if (t1.size() != t2.size())
        return false;
    for (int i = 0; i < t1.size(); ++i) {
        // std::cout << Kind::name(t1[i].first) << "<>" << Kind::name(t2[i].first) << "||"
        //           << t1[i].second << "<>" << t2[i].second << "||" << std::endl;
        if (t1[i].first == t2[i].first && t1[i].second == t2[i].second)
            continue;
        else
            return false;
    }
    return true;
}


std::vector<std::pair<Kind::Type, std::string>> get_tokens_by_regex(std::string content) {
    std::vector<std::pair<Kind::Type, std::string>> tokens;
    std::regex e("\\(([^ ]+?), ([^ ]+?)\\)", std::regex_constants::icase);
    std::smatch m;
    int i = 0;
    while (std::regex_search(content, m, e)) {
        i++;
        // std::cout << m[0] << " " << m[1] << "||" << m[2] << std::endl;
        if (Kind::name(Kind::KEY) == m[1])
            tokens.push_back({Kind::KEY, m[2]});
        if (Kind::name(Kind::SYM) == m[1])
            tokens.push_back({Kind::SYM, m[2]});
        if (Kind::name(Kind::ID) == m[1])
            tokens.push_back({Kind::ID, m[2]});
        if (Kind::name(Kind::NUM) == m[1])
            tokens.push_back({Kind::NUM, m[2]});
        if (Kind::name(Kind::STR) == m[1])
            tokens.push_back({Kind::STR, m[2]});
        content = m.suffix().str();  // 返回末端，作为新的搜索的开始
    }
    return tokens;
};



std::vector<ErrorMsg>
get_thining_tokens_from_file(std::string filename, std::vector<std::pair<Kind::Type, std::string>>& tokens) {
    std::ifstream f(filename);
    std::istream is(f.rdbuf());
    LexicalAnalysis a;
    std::vector<std::pair<Kind::Type, std::string>> analysis_tokens;
    std::vector<ErrorMsg> msgs = a.transfer_token(is, analysis_tokens);
    // 输出错误消息
    // std::cout << msgs.size() << std::endl;
    tokens = Kind::tokens_thining(analysis_tokens);
    return msgs;
}
