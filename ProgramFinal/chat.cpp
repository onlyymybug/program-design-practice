#include "chat.h"
#include "ui_chat.h"
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QDebug>
#include <iostream>
#include <QString>
#include "interpreter.h"
#include <QTimer>
#include <QMessageBox>
#include <string>
#include <QFile>
#include <QTextStream>
#include <fstream>

QTimer * timeoutTimer;
int currentTime = 0;
QString currentInput = "";
QString chatContent = "";
interpreter inter;

Chat::Chat(QWidget *parent,std::string userKeyWord) :
    QWidget(parent),
    ui(new Ui::Chat)
{
    inter.init();
    inter.setKeyWord(userKeyWord);
    ui->setupUi(this);

    this->setFixedSize(1400,800);
    this->setFont(QFont("宋体",14));
    //对话框
    QTextEdit * outputBox = new QTextEdit(this);
    outputBox->setGeometry(20,20,1360,460);
    outputBox->setAlignment(Qt::AlignTop|Qt::AlignLeft);
    outputBox->setReadOnly(true);
    //输入框
    QTextEdit * inputBox = new QTextEdit(this);
    inputBox->setGeometry(20,500,1360,240);
    inputBox->setAlignment(Qt::AlignTop|Qt::AlignLeft);
    //发送按键
    QPushButton * sendButton = new QPushButton(this);
    sendButton->setGeometry(1260,750,120,40);
    sendButton->setText("发送");
    //退出提示消息框
    QMessageBox * timeoutBox = new QMessageBox(this);
    timeoutBox->setWindowTitle("提示");
    //初始step输出
    std::string s2 = inter.answer();
    QString answerStr = QString::fromStdString(s2);
    chatContent += (QString)"<div style=\"font-size: 22px; text-align: left; color: red;\">bot</div>"
            + "<div style=\"text-align: left;\">" + answerStr + "</div>";
    outputBox->setText(chatContent);
    //sginal:发送 slot:清空输入框 消息框同步 产生回答/退出 重置定时器
    connect(sendButton,&QPushButton::clicked,[=,&inter,&chatContent](){
        currentTime = 0;
        QString inputStr = inputBox->toPlainText();
        chatContent += "<p style=\"font-size: 22px; text-align: right; color: blue;\">"
                + QString::fromStdString(inter.getKeyWords())
                + "</p>"
                + "<p style=\"text-align: right;\">"
                + inputStr
                + "</p>";
        inputBox->clear();
        outputBox->setText(chatContent);
        outputBox->moveCursor(QTextCursor::End);
        std::string s1 = inputStr.toStdString();
        bool isEnd = inter.react(s1);
        if(isEnd){
            timeoutBox->setText("对话已结束,即将退出！");
            timeoutBox->show();
            timeoutBox->exec();
            this->close();
        }else {
            std::string s2 = inter.answer();
            QString answerStr = QString::fromStdString(s2);
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
    //signal:长时间输入无变化 slot:退出提示框->退出
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
        if(currentTime == inter.getListenTime()){
            timeoutBox->setText("您长时间未提问,对话已结束！");
            timeoutBox->show();
            timeoutBox->exec();
            this->close();
        }
        timeoutTimer->start(1000);
    });
}

Chat::~Chat()
{
    delete ui;
}

