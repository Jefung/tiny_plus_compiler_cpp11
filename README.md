# Tiny+ 编译器部分实现
## todo:
- [x] 词法分析
- [x] 语法分析(递归下降语法分析)
- [x] 语义分析(生成语法树的同时进行语义分析)
- [x] 中间代码生成
- [x] 支持`if-stmt | repeat-stmt | assign-stmt | read-stmt | write-stmt | while-stmt`
- [ ] 支持`function-def`,`function-call`
- [ ] 使用[高效的预测分析技术](https://blog.csdn.net/jzyhywxz/article/details/78404558)生成语法树
- [ ] 中间代码优化
- [ ] 符号表跟随作用域变化(tiny变量没有作用域)
- [ ] 词法检测遇到错误直接结束检测, 增加错误跳过


## 目录结构
```
├─docs : 文档均来自老师的实验资料
│      Doxyfile
│      TINY编译器词法语法规则.doc
│      编译原理实验要求.doc
│      词法分析思路.pptx
│      语法分析思路.pptx
│
├─sample : 测试样本, 其中`experiment*`是实验数据,但是生成中间代码(`experiment2_test*`)的输出并不相同
│
├─src : 源文件
│      CMakeLists.txt
│      data_types.cpp
│      data_types.h : 数据类型定义
│      grammar_analysis.cpp
│      grammar_analysis.h : 语法分析
│      lexical_analysis.cpp
│      lexical_analysis.h : 词法分析
│      main.cpp : 入口(未实现), 所有测试均在单元测试中
│      to_four_element_exp.cpp
│      to_four_element_exp.h : 生成中间代码
│
├─tests
│      CMakeLists.txt
│      common.cpp
│      common.h : 测试共用函数定义
│      test_example.cpp
│      test_generate_four_element_exp.cpp : 测试生成中间代码, 直接输出结果
│      test_generate_grammer_tree.cpp : 测试生成语法树
│      test_generate_sym_table.cpp : 测试生成符号表
│      test_grammar_error.cpp : 测试语法错误
│      test_lexical_analysis.cpp : 测试词法分析
│
└─third_lib : 第三方库`googletest`
```
## 一) 词法分析
* 问题:
    * `{abc er` 缺少右闭合怎么处理? 直接默认一整行错误,并跳过该行分析
    * `abc ef}` 缺少左闭合怎么处理? 直接默认一整行错误,并跳过该行分析
    * 换行符的处理
    ```cpp
    //c++ 字符串包含换行符
        std::string str = "a"
                          "b";
        std::string str2 = "a\n\a";
    ```
    目前我在字符串状态下,匹配到了`\n`会认为是失败的.

## 语法分析
* 问题:
    * read/write语句后面带`;`
    * assign语句后面不带`;`

## 中间代码生成
* 思路:

从语法树根节点开始扫描, 根据节点类型+文法进行递归生成. 对于
`if-stmt | repeat-stmt | assign-stmt | while-stmt` 等需要跳转的,需要先生成
末端标签并传入供跳转

## 参考链接:
ps: 实验所需要的可以直接参考`doc`下的文档
* [【编译原理】语法分析（二）- CSDN博客](https://blog.csdn.net/jzyhywxz/article/details/78404558)
* [FIRST集与FOLLOW集 - CSDN博客](https://blog.csdn.net/IT_DREAM_ER/article/details/53419667)
* [编译原理 消除左递归](https://blog.csdn.net/liyun123gx/article/details/19924993)