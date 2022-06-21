#ifndef ENROLL_H
#define ENROLL_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <client.h>
using namespace std;
using namespace THC;


class Enroll : public QDialog
{
    Q_OBJECT //使用信号与槽需要的宏
public:
    explicit Enroll(QDialog *parent = 0);  //explicit 防止歧义

signals:

public slots:
    void enroll();//点击登录按钮是执行的槽函数
public:
    QLineEdit *inuserNameLEd;      //用户名编辑行
    QLineEdit *inpwdLEd;           //密码编辑行
    void bind(User* u){user=u;}
private:
    QLabel *inuserNameLbl;         //"用户名"标签
    QLabel *inpwdLbl;              //"密码"标签
    QPushButton *inexitBtn;        //退出按钮
    QPushButton *inenrollBtn;      //注册按钮

    User* user;
};

#endif // ENROLL_H
