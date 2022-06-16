/*-------------------------------------------------
#
# Project created by QtCreator
# Author: 赵一鸣/蒋劲/杨明
# CreateTime: 2022-5-9
# UpdateTime: 2022-5-9
# Info: Qt树洞
# Github:https://github.com/ZYM-PKU/Qt-TreeHole
#
#-------------------------------------------------*/
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTextCodec>
#include <QDateTime>
#include <QDebug>
#include <bits/stdc++.h>
#include "client.h"

using namespace std;
using namespace THC;


#define IP "162.105.101.249"
#define PORT 9999

char password[] = "1919810";
User me(114514ll, password);

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    resize(1200, 900);


    //初始化套接字对象
    TSocket = new QTcpSocket(this);
    //链接服务器
    TSocket->connectToHost(QHostAddress(IP), PORT);
    if(!TSocket->waitForConnected(30000))
    {
         qDebug() << "Connection failed";
         return;
     }
     qDebug() << "Connect successfully";

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString msg = ui->textEdit->toPlainText();
    ui->textEdit->setText("");

    string str = msg.toStdString();
    const char* textbuf = str.c_str();


    if(me.gettoken() == 0){
        me.bindsocket(TSocket);
        me.signup();
        me.login();
    }

    me.sendtext(textbuf);
    auto v = me.getitems(1,100);
    for(auto it : v){
        if(it->getitype() == TEXTITEM){
            TextItem* tit = reinterpret_cast<TextItem*>(it);
            cout<<"user "<<tit->getuid()<<" says "<<tit->gettext()<<endl;
        }
    }
    cout<<"success"<<endl;

}

void MainWindow::dealMessage(QNChatMessage *messageW, QListWidgetItem *item, QString text, QString time,  QNChatMessage::User_Type type)
{
    messageW->setFixedWidth(ui->listWidget->width());
    QSize size = messageW->fontRect(text);
    item->setSizeHint(size);
    messageW->setText(text, time, size, type);
    ui->listWidget->setItemWidget(item, messageW);
}

void MainWindow::dealMessageTime(QString curMsgTime)
{
    bool isShowTime = false;
    if(ui->listWidget->count() > 0) {
        QListWidgetItem* lastItem = ui->listWidget->item(ui->listWidget->count() - 1);
        QNChatMessage* messageW = (QNChatMessage*)ui->listWidget->itemWidget(lastItem);
        int lastTime = messageW->time().toInt();
        int curTime = curMsgTime.toInt();
        qDebug() << "curTime lastTime:" << curTime - lastTime;
        isShowTime = ((curTime - lastTime) > 60); // 两个消息相差一分钟
//        isShowTime = true;
    } else {
        isShowTime = true;
    }
    if(isShowTime) {
        QNChatMessage* messageTime = new QNChatMessage(ui->listWidget->parentWidget());
        QListWidgetItem* itemTime = new QListWidgetItem(ui->listWidget);

        QSize size = QSize(this->width(), 40);
        messageTime->resize(size);
        itemTime->setSizeHint(size);
        messageTime->setText(curMsgTime, curMsgTime, size, QNChatMessage::User_Time);
        ui->listWidget->setItemWidget(itemTime, messageTime);
    }
}


