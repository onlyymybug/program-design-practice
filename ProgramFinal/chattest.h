#ifndef CHATTEST_H
#define CHATTEST_H

#include <QWidget>

class chatTest : public QWidget
{
    Q_OBJECT
public:
    explicit chatTest(QWidget *parent = nullptr,std::string userKeyWord = "");
    ~chatTest();
signals:

public slots:
};

#endif // CHATTEST_H
