
#include "enroll.h"
#include <QMessageBox>
#include <QRegExpValidator>
Enroll::Enroll(QDialog *parent):
    QDialog(parent)
{
    QPalette palette;
    QPixmap enrollpixmap("../Qt-Treehole/img/enrollbackground.jpg");
    palette.setBrush(QPalette::Window, QBrush(enrollpixmap));
    this->setPalette(palette);
    this->setWindowTitle(tr("注册界面"));
    this->resize(500,300);
    //用户名
    inuserNameLbl = new QLabel(this);
    inuserNameLbl->move(80,70);
    inuserNameLbl->setText("用户名:");
    inuserNameLEd = new QLineEdit(this);
    inuserNameLEd->resize(300,30);
    inuserNameLEd->move(160,60);
    inuserNameLEd->setPlaceholderText(tr("只能由数字组成"));//占位符
    inuserNameLEd->setValidator(new QRegExpValidator(QRegExp("[0-9]+$")));
    //密码
    inpwdLbl = new QLabel(this);
    inpwdLbl->move(80,110);
    inpwdLbl->setText("密码:");
    inpwdLEd = new QLineEdit(this);
    inpwdLEd->resize(300,30);
    inpwdLEd->move(160,100);
    inpwdLEd->setPlaceholderText("密码不小于六位不长于十二位");
    inpwdLEd->setMaxLength(12);
    inpwdLEd->setValidator(new QRegExpValidator(QRegExp("[a-zA-Z0-9]+$")));
    inpwdLEd->setEchoMode(QLineEdit::Password);
    setStyleSheet("color:#FF77FF;");
    //退出
    inexitBtn = new QPushButton(this);
    inexitBtn->move(330,170);
    inexitBtn->setText("退出");

    //注册按钮
    inenrollBtn = new QPushButton(this);
    inenrollBtn->move(90,170);
    inenrollBtn->setText("注册");


    //单击退出按钮时 执行 LoginForm::close 槽函数(窗体的关闭函数)
    connect(inenrollBtn,&QPushButton::clicked,this,&Enroll::enroll);
    connect(inexitBtn,&QPushButton::clicked,this,&Enroll::close);
}
void Enroll::enroll()
{
    long long username = inuserNameLEd->text().toLong();
    QString ptext = inpwdLEd->text();
    string str = ptext.toStdString();
    const char *passWord = str.c_str();
    int len=strlen(passWord);

    if(len>=6)
    {
        user->init(username, passWord);

       if(user->signup())
       {
           accept();
       }
       else {
           // 清空输入框内容
              inuserNameLEd->clear();
              inpwdLEd->clear();

              //光标定位
              inuserNameLEd->setFocus();
       }
    }
    else
    {                              //标题             内容                OK按钮
       QMessageBox::warning(this, tr("警告！"),tr("密码不足六位！"),QMessageBox::Yes);

    // 清空输入框内容
       inuserNameLEd->clear();
       inpwdLEd->clear();

       //光标定位
       inuserNameLEd->setFocus();
    }
}
