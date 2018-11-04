#include <fstream>
#include "gtest/gtest.h"
#include <regex>
#include <vector>
#include "data_type.h"
#include "lexical_analysis.h"
#include "./common.h"

using namespace std;

// bool compare_tokens(const vector<pair<Kind::Type, string>> &t1,
//                     const vector<pair<Kind::Type, string>> &t2) {
//     if (t1.size() != t2.size())
//         return false;
//     for (int i = 0; i < t1.size(); ++i) {
//         // std::cout << Kind::name(t1[i].first) << "<>" << Kind::name(t2[i].first) << "||"
//         //           << t1[i].second << "<>" << t2[i].second << "||" << std::endl;
//         if (t1[i].first == t2[i].first && t1[i].second == t2[i].second)
//             continue;
//         else
//             return false;
//     }
//     return true;
// }
//
//
// vector<pair<Kind::Type, string>> get_tokens_by_regex(std::string content) {
//     vector<pair<Kind::Type, string>> tokens;
//     std::regex e("\\(([^ ]+?), ([^ ]+?)\\)", std::regex_constants::icase);
//     std::smatch m;
//     int i = 0;
//     while (std::regex_search(content, m, e)) {
//         i++;
//         // std::cout << m[0] << " " << m[1] << "||" << m[2] << std::endl;
//         if (Kind::name(Kind::KEY) == m[1])
//             tokens.push_back({Kind::KEY, m[2]});
//         if (Kind::name(Kind::SYM) == m[1])
//             tokens.push_back({Kind::SYM, m[2]});
//         if (Kind::name(Kind::ID) == m[1])
//             tokens.push_back({Kind::ID, m[2]});
//         if (Kind::name(Kind::NUM) == m[1])
//             tokens.push_back({Kind::NUM, m[2]});
//         if (Kind::name(Kind::STR) == m[1])
//             tokens.push_back({Kind::STR, m[2]});
//         content = m.suffix().str();  // 返回末端，作为新的搜索的开始
//     }
//     return tokens;
// };

TEST(experiment1, test2) {
    std::ifstream t("../../sample/experiment1_test2_out.txt");
    std::string content((std::istreambuf_iterator<char>(t)),
                        std::istreambuf_iterator<char>());
    auto tokens = get_tokens_by_regex(content);
    LexicalAnalysis a;
    std::ifstream f("../../sample/experiment1_test2_in.txt");
    istream is(f.rdbuf());
    vector<pair<Kind::Type, string>> analysis_tokens;
    std::vector<ErrorMsg> msgs = a.transfer_token(is, analysis_tokens);
    EXPECT_EQ(tokens.size(), analysis_tokens.size()) << "token数量不一样";
    EXPECT_TRUE(compare_tokens(tokens, analysis_tokens)) << "tokens数量一样,但内容不一样";
    EXPECT_EQ(msgs.size(), 0) << "错误消息行数不相等";
    // 输出错误消息
    // std::cout << msgs.size() << std::endl;
    // for (auto msg : msgs) {
    //     std::cout << msg.to_string() << std::endl;
    // }
}

TEST(experiment1, test1) {
    std::ifstream t("../../sample/experiment1_test1_out.txt");
    std::string content((std::istreambuf_iterator<char>(t)),
                        std::istreambuf_iterator<char>());
    auto tokens = get_tokens_by_regex(content);
    LexicalAnalysis a;
    std::ifstream f("../../sample/experiment1_test1_in.txt");
    istream is(f.rdbuf());
    vector<pair<Kind::Type, string>> analysis_tokens;
    std::vector<ErrorMsg> msgs = a.transfer_token(is, analysis_tokens);
    // 输出错误消息
    // std::cout << msgs.size() << std::endl;
    // for(auto msg : msgs){
    //     std::cout << msg.to_string() << std::endl;
    // }
    EXPECT_EQ(tokens.size(), analysis_tokens.size()) << "tokens数量不一样";
    EXPECT_TRUE(compare_tokens(tokens, analysis_tokens)) << "tokens内容不一样";

    EXPECT_EQ(msgs.size(), 7) << "错误消息行数不相等";
}

