#include "chattest.h"
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QDebug>
#include <iostream>
#include <QString>
#include <QTimer>
#include <QMessageBox>
#include <string>
#include <QFile>
#include <QTextStream>
#include <fstream>

extern QTimer * timeoutTimer;
extern int currentTime;
extern QString currentInput;
extern QString chatContent;
//同chat 用于测试问答界面
chatTest::chatTest(QWidget *parent,std::string userKeyWord) :
    QWidget(parent)
{
    currentTime = 0;
    currentInput = "";
    chatContent = "";
    this->setFixedSize(1400,800);
    this->setFont(QFont("宋体",14));

    QTextEdit * outputBox = new QTextEdit(this);
    outputBox->setGeometry(20,20,1360,460);
    outputBox->setAlignment(Qt::AlignTop|Qt::AlignLeft);
    outputBox->setReadOnly(true);

    QTextEdit * inputBox = new QTextEdit(this);
    inputBox->setGeometry(20,500,1360,240);
    inputBox->setAlignment(Qt::AlignTop|Qt::AlignLeft);

    QPushButton * sendButton = new QPushButton(this);
    sendButton->setGeometry(1260,750,120,40);
    sendButton->setText("发送");

    QMessageBox * timeoutBox = new QMessageBox(this);
    timeoutBox->setWindowTitle("提示");
    timeoutBox->setText("长时间无操作，即将退出!");

    QMessageBox * endBox = new QMessageBox(this);
    endBox->setWindowTitle("提示");
    timeoutBox->setText("对话已结束,即将退出！");

    QString answerStr = "这是bot的回复测试";
    chatContent += (QString)"<div style=\"font-size: 22px; text-align: left; color: red;\">bot</div>"
            + "<div style=\"text-align: left;\">" + answerStr + "</div>";
    outputBox->setText(chatContent);

    connect(sendButton,&QPushButton::clicked,[=,&chatContent](){
        currentTime = 0;
        QString inputStr = inputBox->toPlainText();
        chatContent += "<p style=\"font-size: 22px; text-align: right; color: blue;\">"
                + QString::fromStdString(userKeyWord)
                + "</p>"
                + "<p style=\"text-align: right;\">"
                + inputStr
                + "</p>";
        inputBox->clear();
        outputBox->setText(chatContent);
        outputBox->moveCursor(QTextCursor::End);

        if(inputStr == "结束"){
            timeoutBox->show();
            timeoutBox->exec();
            this->close();
        }else {
            QString answerStr = "这也是bot的回复测试";
            chatContent += (QString)"<div style=\"font-size: 22px; text-align: left; color: red;\">bot</div>"
                    "<div style=\"text-align: left;\">" + answerStr + "</div>";
            QTimer * thinkTimer = new QTimer(this);
            thinkTimer->start(1000);
            connect(thinkTimer,&QTimer::timeout,[=]{
                thinkTimer->stop();
                outputBox->setText(chatContent);
                outputBox->moveCursor(QTextCursor::End);
            });
        }
    });

    timeoutTimer = new QTimer(this);
    timeoutTimer->start(1000);
    connect(timeoutTimer,&QTimer::timeout,[=](){
        timeoutTimer->stop();
        if(inputBox->toPlainText() == currentInput){
            currentTime++;
        } else {
            currentInput = inputBox->toPlainText();
            currentTime = 0;
        }
        if(currentTime == 15){
            currentTime = 0;
            timeoutBox->show();
            timeoutBox->exec();
            this->close();
        }
        timeoutTimer->start(1000);
    });
}

chatTest::~chatTest()
{

}

