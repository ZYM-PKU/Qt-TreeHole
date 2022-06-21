#ifndef LOGIN_H
#define LOGIN_H


#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include "enroll.h"
#include "client.h"
using namespace std;
using namespace THC;

class LoginForm : public QDialog
{
    Q_OBJECT //使用信号与槽需要的宏
public:
    explicit LoginForm(QDialog *parent = 0);  //explicit 防止歧义

signals:

public slots:
    void Login();//点击登录按钮是执行的槽函数
    void enr(); //点击注册按钮执行的槽函数
public:
    QLineEdit *userNameLEd;      //用户名编辑行
    QLineEdit *pwdLEd;           //密码编辑行
    void bind(User* u){
        user=u;
        enroLL.bind(u);
    }
private:
    QLabel *title;               //"p大树洞"标签
    QLabel *userNameLbl;         //"用户名"标签
    QLabel *pwdLbl;              //"密码"标签
    QPushButton *loginBtn;       //登录按钮
    QPushButton *exitBtn;        //退出按钮
    QPushButton *enrollBtn;      //注册按钮:
    Enroll enroLL;

    User* user;

};


#endif // LOGIN_H
