#ifndef NEWWINDOW_H
#define NEWWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QListWidgetItem>
#include <set>
#include <QMessageBox>
#include "client.h"
#include "mainwindow.h"
using namespace THC;
namespace Ui {
class newWindow;
}

class newWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit newWindow(Item* item, User* u,QWidget *parent = 0);
    ~newWindow();

    // 显示的字体
    QFont textFont=QFont("SimSun",13);
    QFont infoFont=QFont("SimSun",9);
    // 显示条目的内容
    void showTextItem(Item* item);
    void showMultiItem(Item* item);
    // 显示条目的评论
    void showCmts(Item* item);


private slots:
    void on_sendBtn_clicked();

    void on_refreshBtn_clicked();

    void on_subcBtn_clicked();

    void on_pushButton_clicked();
    void on_repoBtn_clicked();

private:
    Ui::newWindow *ui;
    User* user;
    Item* curItem;

};

#endif // NEWWINDOW_H
