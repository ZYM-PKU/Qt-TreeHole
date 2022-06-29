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
#include <iostream>
#include "login.h"
#include "enroll.h"
#include "subcIDs.h"


int main(int argc, char *argv[])
{
    ifstream fin;
    fin.open("subcData.txt",ios::in);
    if(!fin.is_open()) cerr<<"open file failed"<<endl;
    int tmp;
    // 读入之前的关注id
    while(fin>>tmp) {
        subcIDs.insert(tmp);
    }
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
       a.exec();  // 运行
       // 写入文件
       ofstream fout;
       fout.open("subcData.txt");
       for(auto i=subcIDs.begin();i!=subcIDs.end();i++) {
           fout<<*i<<' ';
       }
       return 0;
    }
    else return 0;

}
