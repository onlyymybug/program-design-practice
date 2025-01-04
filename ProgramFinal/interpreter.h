#ifndef INTERPRETER_H
#define INTERPRETER_H

#include"parser.h"

class interpreter
{
private:
    std::string key_words;      //主键
    parser p;                   //语法树
    //变量表 内容依次为 用户id 属性名 属性内容
    std::unordered_map<std::string,std::unordered_map<std::string, std::string>> var_table;
    step tstep;                 //当前step

public:
    void setKeyWord(std::string);
    std::string getKeyWords();
    int getListenTime();
    void init();               //初始化，读取脚本和用户数据
    bool react(std::string inputStr);         //根据用户输入转移状态
    std::string answer();      //根据当前状态返回回答内容
};



#endif // INTERPRETER_H
