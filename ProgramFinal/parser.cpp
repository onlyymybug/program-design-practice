#include "parser.h"

//解析脚本
//参数：脚本文件路径
void parser::parse_file(std::string filename) {
    std::fstream f;
    f.open(filename);
    while (!f.eof()) {
        std::string line;
        std::cin.unsetf(std::ios::skipws);   //读取时保留空白
        std::getline(f, line);
        trim(line);                          //消除首尾的空格
        if (line != "" && line[0] != '#' && line != "\n" && line != "\t") {  //出现空行或注释则跳过
            parse_line(line);
        }
    }
    if (old_stepid != "") {
        Script.steps[old_stepid] = currentstep;
    }
    f.close();
    //test_parser();
}

//删除首尾空白字符
//参数：要处理的字符串
void parser::trim(std::string & s)
{
     if (!s.empty())
     {
         s.erase(0, s.find_first_not_of(" "));
         s.erase(s.find_last_not_of(" ") + 1);
     }
}

//处理一行
//参数：某一行字符串
void parser::parse_line(std::string line) {
    std::vector<std::string> tokens;
    std::string temp="";
    for (int i = 0; i < line.size(); i++) {
        if (line[i] == '#') {   //如果识别到行尾注释则直接退出
            break;
        }
        if (line[i] !=  ' ') {  //空格分割token
            temp += line[i];
        }
        else {
            if (temp != "") {   //合成一个token
                tokens.push_back(temp);
            }
            temp = "";
        }
    }
    tokens.push_back(temp);
    process_tokens(tokens);
}

//处理一行中的token情况
//参数：一行中的所有token
void parser::process_tokens(std::vector<std::string>& tokens) {
    std::string token_0 = tokens[0];
    if (token_0 == "Step") {
        process_step(tokens[1]);
    }
    else if (token_0 == "Speak") {
        std::vector<std::string> var(++tokens.begin(), tokens.end());
        process_speak(var);
    }
    else if (token_0 == "Listen") {
        process_listen(std::stoi(tokens[1]));
    }
    else if (token_0 == "Branch") {
        process_branch(tokens[1], tokens[2]);
    }
    else if (token_0 == "Silence") {
        process_silence(tokens[1]);
    }
    else if (token_0 == "Default") {
        process_default(tokens[1]);
    }
    else if (token_0 == "Exit") {
        process_exit();
    }
    else {
        std::cout << "Wrong string!" << token_0 << std::endl;
    }
}

//step创建过程
//参数：步骤名
void parser::process_step(std::string stepid) {
    if (old_stepid != "") {
        Script.steps[old_stepid] = currentstep;
    }
    step newstep;
    //如果此时是第一个step，则设置其为起点
    currentstep = newstep;
    old_stepid = stepid;
    if (Script.steps.size() == 0) {
        Script.entry = stepid;
    }
}

//语音输出过程
//参数：需要语音翻译的token
void parser::process_speak(std::vector<std::string>& tokens) {
    for (int i = 0; i < tokens.size(); i++) {
        //如果此时是变量
        if (tokens[i][0] == '$') {
            Script.vars.push_back(tokens[i].substr(1));
            currentstep.expression.push_back(tokens[i]);

        }
        //如果此时是“”的字符串
        else if (tokens[i][0] == '"') {
            currentstep.expression.push_back(tokens[i]);
        }
        //忽略掉+号
        else if (tokens[i][0] == '+') {
            continue;
        }
        else {
            std::cout << "Wrong expression!\n";
        }
    }
}

//听取语音过程
//参数：最长等待时间
void parser::process_listen(int wait_time) {
    currentstep.listen = wait_time;
}

//分支过程
//参数：回答，跳转步骤名
void parser::process_branch(std::string answer, std::string next_stepid) {
    std::string var(++answer.begin(), --answer.end());   //去掉前后引号
    currentstep.ans_step[var] = next_stepid;
}

//安静过程
//参数：安静时跳转步骤名
void parser::process_silence(std::string next_stepid) {
    currentstep.silence_to = next_stepid;
}

//默认过程
//参数：默认时跳转步骤名
void parser::process_default(std::string next_stepid) {
    currentstep.default_to = next_stepid;
}

//退出过程
void parser::process_exit() {
    currentstep.exit = 1;
}

