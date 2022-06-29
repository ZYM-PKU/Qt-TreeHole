/**
 * @brief the window showing one item
 * @author 蒋敬
 */
#include "newwindow.h"
#include "ui_newwindow.h"
#include "subcIDs.h"
using namespace THC;


newWindow::newWindow(Item* item,User* u,QWidget *parent) :
    curItem(item),
    user(u),
    QMainWindow(parent),
    ui(new Ui::newWindow)
{
    ui->setupUi(this);
    // 获取屏幕尺寸
    QList<QScreen*> screen_list=QGuiApplication::screens();
    QRect rect=screen_list.at(0)->geometry();
    int screenWidth=rect.width();
    int screenHeight=rect.height();
    int windowWidth=screenWidth/3;
    int windowHeight=screenHeight-100;
    setGeometry(screenWidth-windowWidth,50,windowWidth,windowHeight);
    ui->mac_listWidget->resize(windowWidth,0);

    if(subcIDs.find(curItem->getindex())!=subcIDs.end()) {
        ui->subcBtn->setText("已关注");
    }else {
        ui->subcBtn->setText("关注");
    }
    /** show the msg and comments. */
    if(curItem->getitype()==TEXTITEM) {
        showTextItem(curItem);
    }else if(curItem->getitype()==MULTIITEM) {
        showMultiItem(curItem);
    }
    showCmts(curItem);


}

newWindow::~newWindow()
{
    delete ui;

}

void newWindow::showTextItem(Item* item)
{
    QListWidgetItem* wi=new QListWidgetItem();
    TextItem* ti=reinterpret_cast<TextItem*>(item);

    /** show msg. */
    QString msg=ti->gettext();
    QString info="CommentNum: ";
    info.append(QString::fromStdString(to_string(ti->getcomcnt())));
    info+="    SubcribeNum: ";
    info.append(QString::fromStdString(to_string(ti->getsubcnt())));
    info+="     ";
    info+=MainWindow::tmToStr(ti->gettimestamp());
    info+="\n";

    QTextEdit* text=new QTextEdit();
    text->append(info+msg);
    int oneLine=ui->mac_listWidget->width();
    int totalLen=fontMetrics().width(msg);
    int itemH=MainWindow::itemHeight(oneLine,totalLen)+MainWindow::oneLineHeight;
    QSize msgSize=QSize(oneLine,itemH);
    text->setFixedSize(msgSize);  // 设置每个条目的大小
    text->setFont(textFont);
    text->setReadOnly(true);
    text->show();
    wi->setSizeHint(msgSize);
    ui->mac_listWidget->addItem(wi);
    ui->mac_listWidget->setItemWidget(wi,text);

    /** add a spacer. */
    QListWidgetItem* spacer=new QListWidgetItem();
    spacer->setSizeHint(QSize(ui->mac_listWidget->width(),50));
    ui->mac_listWidget->addItem(spacer);
}

void newWindow::showMultiItem(Item *item) {
    /* 读取数据 */
    MultiItem* mi=reinterpret_cast<MultiItem*>(item);
    QString msg=mi->gettext();
    QString info="";
    info+="CommentNum: ";
    info.append(QString::fromStdString(to_string(mi->getcomcnt())));
    info+="    SubcribeNum: ";
    info.append(QString::fromStdString(to_string(mi->getsubcnt())));
    Datetime time=mi->gettimestamp();
    QString timeStr=MainWindow::tmToStr(time);
    info+="     ";
    info+=timeStr;
    //读取图片
    QImage img;
    img.loadFromData((const uchar*)mi->getmulti(),MAXSIZE);
    QPixmap pix=QPixmap::fromImage(img);
    int itemWidth=ui->mac_listWidget->width();
    QPixmap scaledPix=pix.scaled(itemWidth,MainWindow::picHeight);

    // 计算高度
    int oneline=itemWidth;
    int totalLen=fontMetrics().width(msg);
    int textH=MainWindow::itemHeight(oneline,totalLen);  // 文本内容
    int itemH=MainWindow::oneLineHeight*2+MainWindow::picHeight+textH;
    QSize itemSize=QSize(itemWidth,itemH);

    QGroupBox* box=new QGroupBox();
    box->setFixedSize(itemSize);
    QVBoxLayout* layout=new QVBoxLayout();

    // Info
    QLineEdit* infoLine=new QLineEdit();
    infoLine->setText(info);
    infoLine->setFont(infoFont);
    infoLine->setFixedSize(QSize(itemWidth,MainWindow::oneLineHeight));
    infoLine->setReadOnly(true);
    infoLine->show();
    // 图片
    QLabel* picLabel=new QLabel();
    picLabel->setPixmap(scaledPix);
    picLabel->show();
    // 文本
    QTextEdit* text=new QTextEdit();
    text->append(msg);
    text->setFixedSize(itemWidth,textH);  // 设置每个条目的大小
    text->setFont(textFont);
    text->setReadOnly(true);
    text->show();

    // 合在一起
    layout->addWidget(infoLine);
    layout->addWidget(picLabel);
    layout->addWidget(text);
    box->setLayout(layout);

    QListWidgetItem* wi=new QListWidgetItem();
    wi->setSizeHint(itemSize);
    ui->mac_listWidget->addItem(wi);
    ui->mac_listWidget->setItemWidget(wi,box);

    // 增加间隔
    QListWidgetItem* spacer=new QListWidgetItem(ui->mac_listWidget);
    spacer->setSizeHint(QSize(0,100));
    ui->mac_listWidget->addItem(spacer);
}
void newWindow::showCmts(Item *item)
{
    /** show comments. */

    vector<Comment> cmts=item->getcomment();
    char text[MAXLEN+NAMEBYTE];

    for(Comment j:cmts) {

        memset(text, 0, sizeof(text));
        strncpy(text, j.name, strlen(j.name));
        strcat(text, ": ");
        strcat(text, j.text);

        QString qs = QString(text);

        QListWidgetItem* cmtWI=new QListWidgetItem();
        QTextEdit* cmt=new QTextEdit();

        int oneLine2=ui->mac_listWidget->width();
        int totalLen2=fontMetrics().width(qs);
        int itemH2=MainWindow::itemHeight(oneLine2,totalLen2);
        QSize cmtSize=QSize(oneLine2,itemH2);
        cmt->setText(qs);
        cmt->setFont(textFont);
        cmt->setFixedSize(cmtSize);
        cmt->show();
        cmtWI->setSizeHint(cmtSize);
        ui->mac_listWidget->addItem(cmtWI);
        ui->mac_listWidget->setItemWidget(cmtWI,cmt);

        // 增加间隔
        QListWidgetItem* spacer=new QListWidgetItem(ui->mac_listWidget);
        spacer->setSizeHint(QSize(0,20));
        ui->mac_listWidget->addItem(spacer);
    }
}



// 发送评论
void newWindow::on_sendBtn_clicked()
{
    QString cmt=ui->sendCmt->toPlainText();
    if(cmt.length()!=0) {

        string str = cmt.toStdString();
        const char* cmtbuf = str.c_str();

        user->comment(curItem->getindex(), cmtbuf);

        ui->sendCmt->setText(""); //清空
        ui->refreshBtn->click();  //刷新
        //评论自动关注；
        if(ui->subcBtn->text()=="关注") {
            user->subscribe(curItem->getindex());
            ui->subcBtn->setText("已关注");
            subcIDs.insert(curItem->getindex());
        }
    }
}

void newWindow::on_refreshBtn_clicked()
{
    ui->mac_listWidget->clear(); // 清空
    int idx=curItem->getindex();
    vector<Item*> vec=user->getitems(idx,idx+1);
    Item* refItem=vec.at(0);
    if(refItem->getitype()==TEXTITEM) {
        showTextItem(refItem);
    }else if(refItem->getitype()==MULTIITEM) {
        showMultiItem(refItem);
    }
    showCmts(refItem);
}

void newWindow::on_subcBtn_clicked()
{
    if(ui->subcBtn->text()=="关注") {
        user->subscribe(curItem->getindex());
        ui->subcBtn->setText("已关注");
        subcIDs.insert(curItem->getindex());
    }else if(ui->subcBtn->text()=="已关注") {
        user->desubscribe(curItem->getindex());
        ui->subcBtn->setText("关注");
        subcIDs.erase(subcIDs.find(curItem->getindex()));
    }
}
/**
 * @brief background
 * @author 杨明
 */
void newWindow::on_pushButton_clicked()
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

void newWindow::on_repoBtn_clicked()
{
    QMessageBox::information(this, "提示", "本树洞继承了‘思想自由，兼容并包’的优良传统"
                                         "暂不允许举报🐶🐶🐶");

}
