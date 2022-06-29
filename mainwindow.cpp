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
#include "ui_mainwindow.h"
#include <QTextCodec>
#include <QDateTime>
#include <QDebug>
#include <bits/stdc++.h>
#include "client.h"
#include "login.h"
#include "enroll.h"
#include "subcIDs.h"

using namespace std;
using namespace THC;





MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    resize(rect.width(),rect.height()-100);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_sendBtn_clicked()
{
    QString msg = ui->sendMsg->toPlainText();
    ui->sendMsg->setText("");

    string str = msg.toStdString();
    const char* textbuf = str.c_str();
    long long sendID;
    if(!haspic) {
        sendID=user->sendtext(textbuf);
    }else {
        sendID=user->sendmulti(textbuf,picbuf);
        memset(picbuf,0,sizeof(picbuf));
        haspic=false;
    }
    // 评论自动关注
    subcIDs.insert(sendID);

    _sleep(100);
    ui->refreshBtn->click();
}

/**
 * @brief MainWindow::on_refreshBtn_clicked
 * @author 蒋敬
 */
void MainWindow::on_refreshBtn_clicked()
{
    myItems=user->getitems(1,100);
    ui->oldMsg->clear();  // 清空
    ui->searchInfo->setText(""); // 清空搜索框
    ordered=myItems;  // 排序, todo: modify this
    string funcName=ui->orderFunc->currentText().toStdString();
    if(funcName=="最新") {
        sort(ordered.begin(),ordered.end(),orderByTime);
    }else if(funcName=="最多评论") {
        sort(ordered.begin(),ordered.end(),orderByCmtcnt);
    }else if(funcName=="最多关注") {
        sort(ordered.begin(),ordered.end(),orderBySubcnt);
    }
    int itemCnt=ordered.size();
    for(int i=0;i<itemCnt;i++) {
        Item* cur=ordered.at(i);
        if(cur->getitype()==TEXTITEM) {
            showTextItem(reinterpret_cast<TextItem*>(cur),i);
        }else if(cur->getitype()==MULTIITEM) {
            showMultiItem(reinterpret_cast<MultiItem*>(cur),i);
        }
    }
}


/**
 * @brief MainWindow::showTextItem
 * @param ti textItem to show
 * @param i item's index in ordered
 * @author 蒋敬
 */
void MainWindow::showTextItem(TextItem* ti,int i)
{
    QListWidgetItem* item=new QListWidgetItem();
    /* 读取数据 */
    QString msg=ti->gettext();
    QString info="";
    info+="CommentNum: ";
    info.append(QString::fromStdString(to_string(ti->getcomcnt())));
    info+="    SubcribeNum: ";
    info.append(QString::fromStdString(to_string(ti->getsubcnt())));
    Datetime time=ti->gettimestamp();
    QString timeStr=tmToStr(time);
    info+="     ";
    info+=timeStr;
    info+='\n';
    QString index;
    index.append(i+'0');

    // 计算文本的高度
    int oneline=itemWidth;
    int totalLen=fontMetrics().width(msg);
    int itemH=itemHeight(oneline,totalLen)+oneLineHeight;
    QSize itemSize=QSize(itemWidth,itemH);

    // 显示条目
    QTextEdit* text=new QTextEdit();
    text->append(info+msg);
    text->setObjectName(index);  // text的objectname设置为item* 在ordered中的Indx,方便后续从ordered中提取出来
    text->setFixedSize(itemSize);  // 设置每个条目的大小
    text->setFont(textFont);
    text->setReadOnly(true);

    text->show();
    item->setSizeHint(QSize(itemSize));
    ui->oldMsg->addItem(item);
    ui->oldMsg->setItemWidget(item,text);

    // 增加间隔
    QListWidgetItem* spacer=new QListWidgetItem(ui->oldMsg);
    spacer->setSizeHint(QSize(0,100));
    ui->oldMsg->addItem(spacer);
}

void MainWindow::showMultiItem(MultiItem *mi, int i)
{
    /* 读取数据 */
    QString msg=mi->gettext();
    QString info="";
    info+="CommentNum: ";
    info.append(QString::fromStdString(to_string(mi->getcomcnt())));
    info+="    SubcribeNum: ";
    info.append(QString::fromStdString(to_string(mi->getsubcnt())));
    Datetime time=mi->gettimestamp();
    QString timeStr=tmToStr(time);
    info+="     ";
    info+=timeStr;
    QString index;
    index.append(i+'0');
    //读取图片
    QImage img;
    memcpy((void*)picbuf,(void*)mi->getmulti(),MAXSIZE);
    img.loadFromData((const uchar*)picbuf,MAXSIZE);
    QPixmap pix=QPixmap::fromImage(img).scaled(itemWidth,picHeight);

    // 计算高度
    int oneline=itemWidth-20;
    int totalLen=fontMetrics().width(msg);
    int textH=itemHeight(oneline,totalLen);  // 文本内容
    int itemH=oneLineHeight+picHeight+textH+oneLineHeight;  // 额外加一点高度
    QSize itemSize=QSize(oneline,itemH);

    QGroupBox* box=new QGroupBox();
    box->setObjectName(index);
    box->setFixedSize(itemSize);
    QVBoxLayout* layout=new QVBoxLayout();

    // Info
    QLineEdit* infoLine=new QLineEdit();
    infoLine->setText(info);
    infoLine->setFont(infoFont);
    infoLine->setFixedSize(QSize(oneline,oneLineHeight));
    infoLine->setReadOnly(true);
    infoLine->show();
    // 图片
    QLabel* picLabel=new QLabel();
    picLabel->setPixmap(pix);
    picLabel->show();
    // 文本
    QTextEdit* text=new QTextEdit();
    text->append(msg);
    text->setFixedSize(oneline,textH);  // 设置每个条目的大小
    text->setFont(textFont);
    text->setReadOnly(true);
    text->show();

    // 合在一起
    layout->addWidget(infoLine);
    layout->addWidget(picLabel);
    layout->addWidget(text);
    box->setLayout(layout);

    QListWidgetItem* item=new QListWidgetItem();
    item->setSizeHint(itemSize);
    ui->oldMsg->addItem(item);
    ui->oldMsg->setItemWidget(item,box);

    // 增加间隔
    QListWidgetItem* spacer=new QListWidgetItem(ui->oldMsg);
    spacer->setSizeHint(QSize(0,100));
    ui->oldMsg->addItem(spacer);
}



void MainWindow::on_oldMsg_itemClicked(QListWidgetItem *item)
{
    QWidget* w=ui->oldMsg->itemWidget(item);  // 获得item中的widget
    int i=w->objectName().toInt();  // objectname存的是index
    Item* cur=ordered.at(i);  // 从ordered中获取Item*
    newWindow* nw=new newWindow(cur,user);
    nw->show();
    return;
}

/**
 * @brief MainWindow::itemHeight calculate the height of the widget to put the textItem
 * @param oneLine the width of one line
 * @param totalLen the width of the text
 * @return the height we need
 * @author 蒋敬
 */
int MainWindow::itemHeight(int oneLine, int totalLen)
{
    if(totalLen<=oneLine) {
        return oneLineHeight;
    }else {
        return ceil((double)totalLen/oneLine)*oneLineHeight;
    }
}

/**
 * @brief MainWindow::tmToStr
 * @param time
 * @return
 * @author 蒋敬
 */
QString MainWindow::tmToStr(Datetime time)
{
    QString tmp="";
    tmp+=to_string(time.year+1900).c_str();
    tmp.append('-');
    tmp+=to_string(time.tm_mon+1).c_str();
    tmp.append('-');
    tmp+=to_string(time.tm_mday).c_str();
    tmp.append(' ');
    tmp+=to_string(time.tm_hour).c_str();
    tmp.append(':');
    tmp+=to_string(time.tm_min).c_str();
    return tmp;
}

/**
 * @brief MainWindow::on_picBtn_clicked let user select a picture to send
 * @author 蒋敬
 */
void MainWindow::on_picBtn_clicked()
{
    // 弹出explore让用户选择文件
    QString fileName=QFileDialog::getOpenFileName(this,"仅限jpg,png格式","/home",tr("Images (*.png *.jpg)"));
    if(fileName.isEmpty()) return;
    // 二进制流的方式打开文件
    ifstream fin(fileName.toStdString().c_str(),ios::binary);
    if(!fin) {
        cerr<<"open file failed"<<endl;
        return;
    }
    fin.seekg(0,fin.end);
    int len=fin.tellg();
    if(len>MAXSIZE) {
        cerr<<"picture is too big!"<<endl;
        return;
    }
    fin.seekg(0,fin.beg);
    fin.read(picbuf,len);  // 读取到picbuf
    fin.close();
    haspic=true;
}
/**
 * @brief MainWindow::getNext 更新Next数组
 * @param t the string that user want to search
 */
void MainWindow::getNext(string t)
{
    int j,k;
    int m=t.length();
    assert(m>0);
    next[0]=-1;
    j=0;
    k=-1;
    while(j<m-1) {
        while(k>=0&&t[k]!=t[j]) {
            k=next[k];
        }
        j++;
        k++;
        next[j]=k;
    }
}
/**
 * @brief MainWindow::KMP
 * @param s 目标串
 * @param t 模式串
 * @return 返回子串的位置，若失败返回-1
 */
int MainWindow::KMP(string s, string t)
{
    int i,j;
    int tlen=t.length();
    int slen=s.length();
    i=0;
    j=0;
    if(slen<tlen) return (-1);
    while(i<slen&&j<tlen)
   {
      if(j==-1 || s[i]==t[j])
      {
         i++;
         j++;
      }
      else j=next[j];               //j回退。。。
   }
   if(j>=tlen)
       return (i-tlen);         //匹配成功，返回子串的位置
   else
      return (-1);                  //没找到
}

void MainWindow::on_searchBtn_clicked()
{
    string search=ui->searchInfo->text().toStdString();
    memset(next,0,sizeof(next));
    getNext(search);  // 计算next数组
    vector<Item*> tmp=user->getitems(1,100);
    ui->oldMsg->clear();  // 清空显示
    myItems.clear();
    ordered.clear();
    for(Item* i:tmp) {
        if(i->getitype()==TEXTITEM) {
            TextItem* ti=reinterpret_cast<TextItem*>(i);
            string cur=ti->gettext();
            if(KMP(cur,search)!=-1) {
                myItems.push_back(i);
            }
        }else if(i->getitype()==MULTIITEM) {
            MultiItem* mi=reinterpret_cast<MultiItem*>(i);
            string cur=mi->gettext();
            if(KMP(cur,search)!=-1) {
                myItems.push_back(i);
            }
        }
    }
    ordered=myItems;
    string funcName=ui->orderFunc->currentText().toStdString();
    if(funcName=="最新") {
        sort(ordered.begin(),ordered.end(),orderByTime);
    }else if(funcName=="最多评论") {
        sort(ordered.begin(),ordered.end(),orderByCmtcnt);
    }else if(funcName=="最多关注") {
        sort(ordered.begin(),ordered.end(),orderBySubcnt);
    }
    int itemCnt=ordered.size();
    for(int i=0;i<itemCnt;i++) {
        Item* cur=ordered.at(i);
        if(cur->getitype()==TEXTITEM) {
            showTextItem(reinterpret_cast<TextItem*>(cur),i);
        }else if(cur->getitype()==MULTIITEM) {
            showMultiItem(reinterpret_cast<MultiItem*>(cur),i);
        }
    }
}



void MainWindow::on_subsBtn_clicked()
{
    ui->oldMsg->clear();
    myItems.clear();
    ordered.clear();
    set<int>::iterator j=subcIDs.begin();
    set<int>::iterator end=subcIDs.end();
    for(;j!=end;j++) {
        vector<Item*> tmp=user->getitems(*j,(*j)+1);
        myItems.push_back(tmp.at(0));
    }
    ordered=myItems;
    string funcName=ui->orderFunc->currentText().toStdString();
    if(funcName=="最新") {
        sort(ordered.begin(),ordered.end(),orderByTime);
    }else if(funcName=="最多评论") {
        sort(ordered.begin(),ordered.end(),orderByCmtcnt);
    }else if(funcName=="最多关注") {
        sort(ordered.begin(),ordered.end(),orderBySubcnt);
    }
    int itemCnt=ordered.size();
    for(int i=0;i<itemCnt;i++) {
        Item* cur=ordered.at(i);
        if(cur->getitype()==TEXTITEM) {
            showTextItem(reinterpret_cast<TextItem*>(cur),i);
        }else if(cur->getitype()==MULTIITEM) {
            showMultiItem(reinterpret_cast<MultiItem*>(cur),i);
        }
    }
}
/**
 * @brief background
 * @author 杨明
 */
void MainWindow::on_background_clicked()
{
    QString filename=QFileDialog::getOpenFileName(this,tr("Open Image"),QDir::homePath(),tr("(*.jpg)\n(*.bmp)\n(*.png)"));   //打开图片文件，选择图片
    qDebug()<<"filename:"<<filename;
    QPixmap image=QPixmap(filename);   //图片初始化
    qDebug()<<"image:"<<image;
    if(!image.isNull())
    {
        QPalette palette;
        palette.setBrush(QPalette::Window, QBrush(image));
        this->setPalette(palette);
    }
    else
    {
        ui->statusbar->showMessage(tr("Save Image Failed!"),3000);
        return;
    }
}
