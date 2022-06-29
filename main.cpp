/*-------------------------------------------------
#
# Project created by QtCreator
# Author: 赵一鸣/蒋敬/杨明
# CreateTime: 2022-5-9
# UpdateTime: 2022-5-9
# Info: Qt树洞
# Github:https://github.com/ZYM-PKU/Qt-TreeHole
#
#-------------------------------------------------*/
#include "mainwindow.h"
#include <QApplication>
#include <QDialog>
#include "login.h"
#include "enroll.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    LoginForm Login;
    MainWindow w;

    User* user = new User;

    Login.bind(user);
    w.bind(user);


    Login.show();
    if (Login.exec() == QDialog::Accepted)//调用login.exec()，阻塞主控制流，直到完成返回，继续执行主控制流
    {
       w.show();
       return a.exec();
    }
    else return 0;

    return a.exec();
}
