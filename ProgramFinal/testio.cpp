#include "testio.h"
#include "interpreter.h"
#include <QString>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QDir>

testio::testio()
{

}
//测试问询-回答
//参数：用户keyWord 输入文件名 输出文件名
void testio::test(std::string keyWord,std::string inName,std::string outName){
    //创建io流
    QString inFilePath = QDir::currentPath() + "/testin/" + QString::fromStdString(inName);
    QString outFilePath = QDir::currentPath() + "/testout/" + QString::fromStdString(outName);
    QFile * infp = new QFile(inFilePath);
    QFile * outfp = new QFile(outFilePath);
    infp->open(QIODevice::ReadOnly|QIODevice::Text);
    outfp->open(QIODevice::WriteOnly|QIODevice::Text);
    QTextStream in(infp);
    QTextStream out(outfp);
    /*!!!根据实际使用的编码设置!!!*/
    in.setCodec("UTF-8");
    out.setCodec("UTF-8");
    interpreter inter;
    inter.init();
    inter.setKeyWord(keyWord);
    bool isEnd = false;
    //输入文件结束或对话结束均停止输出
    for(;!in.atEnd()&&!isEnd;){
        out << QString::fromStdString(inter.answer())+"\n";
        QString str;
        in >> str;
        isEnd = inter.react(str.toStdString());
    }
    infp->close();
    outfp->close();
}
