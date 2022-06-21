#include "login.h"
#include <QMessageBox>
#include "enroll.h"
#include "client.h"
#include <QByteArray>
#include <QTcpSocket>


LoginForm::LoginForm(QDialog *parent) :
    QDialog(parent)
{
    QPalette palette;
    QPixmap loginpixmap("../Qt-Treehole/img/loginbackground.jpg");
    palette.setBrush(QPalette::Window, QBrush(loginpixmap));
    this->setPalette(palette);
    QFont ft;
    //设置窗体标题
    this->setWindowTitle(tr("登录界面"));
    this->resize(500,300);
    //设置窗口标题
    title = new QLabel(this);
    title->setText("PKU HOLE");
    title->move(140,50);
    ft.setPointSize(14);
    title->move(180,20);
    title->resize(300,100);
    title->setFont(ft);
    setStyleSheet("color:#00BBff;");
    //用户名Label
    userNameLbl = new QLabel(this);   //new一个标签对象
    userNameLbl->move(120,105);
    userNameLbl->setText("用户名:");  //设置标签文本

    //用户名输入框
    userNameLEd = new QLineEdit(this);
    userNameLEd->resize(180,30);
    userNameLEd->move(185,100);
    userNameLEd->setPlaceholderText(tr("请输入用户名..."));//占位符

    //密码Label
    pwdLbl = new QLabel(this);
    pwdLbl->move(120,145);
    pwdLbl->setText("密码:");

    //密码输入框
    pwdLEd = new QLineEdit(this);
    pwdLEd->resize(180,30);
    pwdLEd->move(185,140);
    pwdLEd->setPlaceholderText("请输入密码...");
    pwdLEd->setEchoMode(QLineEdit::Password);//输入的密码以圆点显示

    //登录按钮
    loginBtn = new QPushButton(this);
    loginBtn->move(200,200);
    loginBtn->setText("登录");

    //退出按钮
    exitBtn = new QPushButton(this);
    exitBtn->move(330,200);
    exitBtn->setText("退出");

    //注册按钮
    enrollBtn = new QPushButton(this);
    enrollBtn->move(70,200);
    enrollBtn->setText("注册");


    //单击登录按钮时 执行 LoginForm::login 槽函数(自定义)；单击退出按钮时 执行 LoginForm::close 槽函数(窗体的关闭函数)
    connect(loginBtn,&QPushButton::clicked,this,&LoginForm::Login);
    connect(exitBtn,&QPushButton::clicked,this,&LoginForm::close);
    connect(enrollBtn,&QPushButton::clicked,this,&LoginForm::enr);

}
void LoginForm::enr()
{
    enroLL.show();
}
void LoginForm::Login()
{
    long long username = userNameLEd->text().toLong();

    QString ptext = pwdLEd->text();
    string str = ptext.toStdString();
    const char *passWord = str.c_str();

    user->init(username, passWord);

    if(user->login())
    {
       accept();
    }
    else
    {                              //标题              内容                OK按钮
       QMessageBox::warning(this, tr("警告！"),tr("用户名或密码错误！"),QMessageBox::Yes);

    // 清空输入框内容
       //userNameLEd->clear();
       pwdLEd->clear();

       //光标定位
       userNameLEd->setFocus();
    }
}
