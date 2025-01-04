#include "interpreter.h"
#include<Windows.h>
#include <conio.h>
#include <thread>
#include<process.h>
#include<fstream>
#include<QDebug>
#include<QString>

void interpreter::setKeyWord(std::string key_word){
    this->key_words = key_word;
}

std::string interpreter::getKeyWords(){
    return this->key_words;
}

int interpreter::getListenTime(){
    return (this->tstep.listen == 0?15: this->tstep.listen);
}

//初始化，读取脚本和用户数据
void interpreter::init() {
    this->p.parse_file("F:\\QtFile\\ProgramFinal\\resource\\test3.rsl");//读取脚本
    std::fstream fp;
    fp.open("F:\\QtFile\\ProgramFinal\\resource\\database3.txt");
    std::string pn1, na;
    std::vector<std::string> varName;
    std::string line;
    std::getline(fp,line);
    std::string temp = "";
    //读取属性名
    for (int i = 0; i < line.size(); i++) {
        if (line[i] !=  ' ') {
            temp += line[i];
        }
        else {
            if (temp != "") {
                varName.push_back(temp);
            }
            temp = "";
        }
    }
    varName.push_back(temp);
    //读取用户对应属性
    int var_size = varName.size();
    while (!fp.eof()) {
        fp >> pn1;
        for (int i = 0; i < var_size && !fp.eof(); i++) {
            fp >> na;
            var_table[pn1][varName[i]] = na;
        }
    }
    tstep = p.Script.steps[p.Script.entry];
}

//推进步骤
//返回true则说明已到结束步骤
bool interpreter::react(std::string ans) {
    if (this->var_table.find(key_words) == this->var_table.end()) {
        std::cout << "用户不存在\n";
        return true;
    }
    if (this->tstep.exit == 1) {                //如果是结束步骤直接返回
        return true;
    }
    if (ans == "") {                                            //输入为空跳转silence
        this->tstep = p.Script.steps[tstep.silence_to];
    }
    else if (tstep.ans_step.find(ans) != tstep.ans_step.end()) {//如果在branch中能找到
        this->tstep = p.Script.steps[tstep.ans_step[ans]];
    }
    else {                                                      //找不到则跳转defalt
        this->tstep = p.Script.steps[tstep.default_to];
    }
    return false;
}

//返回这一步骤回答
std::string interpreter::answer() {
    std::string outputStr = "";
    for (int i = 0; i < this->tstep.expression.size(); i++) {
        if (this->tstep.expression[i][0] == '"') {         //如果是字符串直接拼接字符串内容
            std::string var(++this->tstep.expression[i].begin(), --this->tstep.expression[i].end());
            outputStr+=var;
        }
        else if (this->tstep.expression[i][0] == '$') {   //如果是变量则去符号表中找对应的量
            std::string var(++this->tstep.expression[i].begin(), this->tstep.expression[i].end());
            if (var_table[key_words].find(var) != var_table[key_words].end()) {
                outputStr += var_table[key_words][var];
            }
            else {
                qDebug() << "Can't find variable!\n";
            }
        }
        else {//否则则出错
            qDebug() << "Wrong!\n";
        }
    }
    return outputStr;
}


