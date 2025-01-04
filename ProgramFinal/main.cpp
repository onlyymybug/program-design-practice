#include "chat.h"
#include <QApplication>
#include "testio.h"
#include "chattest.h"
#include <iostream>

int main(int argc, char *argv[])
{
    std::string mode = std::string(argv[1]);
    if(mode == "-t"){           //io测试模式 参数：用户KeyWord 输入文件名 输出文件名
        if(argc == 5){
            std::string keyWord = std::string(argv[2]);
            std::string inPath = std::string(argv[3]);
            std::string outPath = std::string(argv[4]);
            testio tio;
            tio.test(keyWord,inPath,outPath);
        }else {
            std::cout << "lack of parameter!" << std::endl;
        }
        return 0;

    }else if(mode == "-r"){     //正常运行模式 参数：用户KeyWord
        if(argc == 3){
            QApplication a(argc, argv);
            Chat * w = new Chat(0,std::string(argv[2]));
            w->show();
            return a.exec();
        }else {
            std::cout << "lack of parameter!" << std::endl;
            return 0;
        }
    }else if (mode == "-rt") {  //ui界面测试 参数：用户KeyWord
        if(argc == 3){
            QApplication a(argc, argv);
            chatTest * w = new chatTest(0,std::string(argv[2]));
            w->show();
            return a.exec();
        }else {
            std::cout << "lack of parameter!" << std::endl;
            return 0;
        }

    }

}
