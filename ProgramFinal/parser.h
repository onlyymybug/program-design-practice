#ifndef PARSER_H
#define PARSER_H

#include<cstdio>
#include<string>
#include<iostream>
#include<vector>
#include<unordered_map>
#include<fstream>

class step {
public:
    std::vector<std::string> expression;                    //存储speak中用到的的变量
    int listen;                                             //存储listen的时间
    std::unordered_map<std::string, std::string>  ans_step; //前者为回答，后者为步骤名
    std::string silence_to;									//silence情况的步骤名
    std::string default_to;								    //default情况的步骤名
    int exit = 0;                                           //为0表示此时不存在退出程序，为1则表示此时需要退出
};

class script {
public:
    std::unordered_map<std::string, step> steps;           //存储所有step，前者为step名，后者为step
    std::string entry;                                     //存放入口stepid
    std::vector<std::string> vars;                         //存放所有变量
};

class parser
{
public:
    void parse_file(std::string filename);  //读取脚本文件
    script Script;                          //Script实例
private:
    std::string old_stepid="";              //上一个step_id名

    step currentstep;                       //当前step
    void trim(std::string& s);              //删除首尾空白字符
    void parse_line(std::string line);      //处理一行
    void process_tokens(std::vector<std::string>& tokens);                 //处理一行中的token情况
    void process_step(std::string stepid);                                 //step创建过程
    void process_speak(std::vector<std::string>& tokens);                  //对话输出过程
    void process_listen(int wait_time);                                    //听取输入过程
    void process_branch(std::string answer, std::string next_stepid);      //分支过程
    void process_silence(std::string next_stepid);                         //安静过程
    void process_default(std::string next_stepid);                         //默认过程
    void process_exit();                                                   //退出过程
};



#endif // PARSER_H
