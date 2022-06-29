/*-------------------------------------------------
#
# Project created by QtCreator
# Author: 赵一鸣
# CreateTime: 2022-5-9
# UpdateTime: 2022-5-9
# Info: Qt树洞
# Github:https://github.com/ZYM-PKU/Qt-TreeHole
#
#-------------------------------------------------*/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QTcpSocket>
#include <QUdpSocket>
#include <QHostAddress>
#include <QDebug>
#include <QTimer>
#include <QString>
#include <vector>
#include <QScreen>
#include <QRect>
#include <QGuiApplication>
#include <math.h>
#include <QGroupBox>
#include <QLayout>
#include <QFileDialog>
#include <set>
#include "client.h"
#include "newwindow.h"
#include "login.h"
#include "enroll.h"
using namespace THC;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void bind(User* u) {user=u;}

    // 存储Items
    vector<Item*> myItems;
    // 排序后的Items
    vector<Item*> ordered;
    // 显示的字体
    QFont textFont=QFont("SimSun",13);
    QFont infoFont=QFont("SimSun",10);
    // 获取屏幕尺寸
    QList<QScreen*> screen_list=QGuiApplication::screens();
    QRect rect=screen_list.at(0)->geometry();
    int screenWidth=rect.width();
    // 显示的每一条条目宽度
    int itemWidth=screenWidth/2;

    static const int oneLineHeight=45;
    static const int picHeight=600;

    // multiItem
    char picbuf[MAXSIZE];
    bool haspic=false;

    // 显示文本条目
    void showTextItem(TextItem* ti,int i);
    // 显示图片条目
    void showMultiItem(MultiItem* mi,int i);
    // 计算每个条目的自适应高度
    static int itemHeight(int oneLine, int totalLen);

    // 根据tm返回字符串
    static QString tmToStr(Datetime time);

    // 搜索功能
    int next[MAXLEN];
    void getNext(string t);
    int KMP(string s,string t);





private slots:
    void on_sendBtn_clicked();

    void on_refreshBtn_clicked();

    void on_oldMsg_itemClicked(QListWidgetItem *item);

    void on_picBtn_clicked();

    void on_searchBtn_clicked();

    void on_subsBtn_clicked();

    void on_background_clicked();
private:
    Ui::MainWindow *ui;
    User* user;
};

#endif // MAINWINDOW_H
