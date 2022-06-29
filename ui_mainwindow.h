/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout_4;
    QVBoxLayout *mainLayout;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *refreshBtn;
    QPushButton *background;
    QSpacerItem *horizontalSpacer_3;
    QLineEdit *searchInfo;
    QSpacerItem *horizontalSpacer_4;
    QPushButton *searchBtn;
    QSpacerItem *horizontalSpacer_5;
    QPushButton *subsBtn;
    QHBoxLayout *horizontalLayout;
    QListWidget *oldMsg;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QComboBox *orderFunc;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *picBtn;
    QPushButton *sendBtn;
    QWidget *widget;
    QTextEdit *sendMsg;
    QMenuBar *menubar;
    QMenu *menuQt_TreeHole;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(955, 600);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        QFont font;
        font.setFamily(QString::fromUtf8("Arial"));
        font.setPointSize(9);
        MainWindow->setFont(font);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        horizontalLayout_4 = new QHBoxLayout(centralwidget);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        mainLayout = new QVBoxLayout();
        mainLayout->setObjectName(QString::fromUtf8("mainLayout"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        refreshBtn = new QPushButton(centralwidget);
        refreshBtn->setObjectName(QString::fromUtf8("refreshBtn"));
        refreshBtn->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 85, 0);\n"
"color: white;\n"
"font: 18px \"Comic Sans MS\";\n"
"border: 2px solid rgb(255, 170, 0);\n"
"border-radius: 10px;\n"
"width: 100px;\n"
"height: 25px;\n"
""));

        horizontalLayout_3->addWidget(refreshBtn);

        background = new QPushButton(centralwidget);
        background->setObjectName(QString::fromUtf8("background"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(background->sizePolicy().hasHeightForWidth());
        background->setSizePolicy(sizePolicy1);
        background->setMinimumSize(QSize(104, 29));
        background->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 85, 0);\n"
"color: white;\n"
"font: 18px \"Comic Sans MS\";\n"
"border: 2px solid rgb(255, 170, 0);\n"
"border-radius: 10px;\n"
"width: 100px;\n"
"height: 25px;\n"
""));

        horizontalLayout_3->addWidget(background);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_3);

        searchInfo = new QLineEdit(centralwidget);
        searchInfo->setObjectName(QString::fromUtf8("searchInfo"));
        searchInfo->setStyleSheet(QString::fromUtf8("height: 30px;\n"
"background-color: rgb(255, 255, 255);\n"
"border: 2px solid rgb(67, 67, 67);\n"
"border-radius: 10px;"));

        horizontalLayout_3->addWidget(searchInfo);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_4);

        searchBtn = new QPushButton(centralwidget);
        searchBtn->setObjectName(QString::fromUtf8("searchBtn"));
        searchBtn->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 85, 0);\n"
"color: white;\n"
"font: 18px \"Comic Sans MS\";\n"
"border: 2px solid rgb(255, 170, 0);\n"
"border-radius: 10px;\n"
"width: 100px;\n"
"height: 25px;\n"
""));

        horizontalLayout_3->addWidget(searchBtn);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_5);

        subsBtn = new QPushButton(centralwidget);
        subsBtn->setObjectName(QString::fromUtf8("subsBtn"));
        subsBtn->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 85, 0);\n"
"color: white;\n"
"font: 18px \"Comic Sans MS\";\n"
"border: 2px solid rgb(255, 170, 0);\n"
"border-radius: 10px;\n"
"width: 100px;\n"
"height: 25px;\n"
""));

        horizontalLayout_3->addWidget(subsBtn);

        horizontalLayout_3->setStretch(2, 1);
        horizontalLayout_3->setStretch(3, 50);
        horizontalLayout_3->setStretch(4, 1);
        horizontalLayout_3->setStretch(5, 10);
        horizontalLayout_3->setStretch(6, 1);
        horizontalLayout_3->setStretch(7, 10);

        mainLayout->addLayout(horizontalLayout_3);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        oldMsg = new QListWidget(centralwidget);
        oldMsg->setObjectName(QString::fromUtf8("oldMsg"));
        oldMsg->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"border: 2px solid rgb(67, 67, 67);\n"
"border-radius: 10px;"));
        oldMsg->setLineWidth(0);
        oldMsg->setAutoScroll(true);

        horizontalLayout->addWidget(oldMsg);

        horizontalLayout->setStretch(0, 6);

        mainLayout->addLayout(horizontalLayout);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        orderFunc = new QComboBox(centralwidget);
        orderFunc->addItem(QString());
        orderFunc->addItem(QString());
        orderFunc->addItem(QString());
        orderFunc->setObjectName(QString::fromUtf8("orderFunc"));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Comic Sans MS"));
        font1.setBold(false);
        font1.setItalic(false);
        font1.setWeight(50);
        orderFunc->setFont(font1);
        orderFunc->setStyleSheet(QString::fromUtf8("background-color: rgb(50,50,50);\n"
"color: white;\n"
"font: 18px \"Comic Sans MS\";\n"
"border: 2px solid rgb(150,150,150);\n"
"border-radius: 10px;\n"
"width: 100px;\n"
"height: 25px;"));

        horizontalLayout_2->addWidget(orderFunc);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        picBtn = new QPushButton(centralwidget);
        picBtn->setObjectName(QString::fromUtf8("picBtn"));
        picBtn->setStyleSheet(QString::fromUtf8("background-color: rgb(14, 118, 255);\n"
"color: white;\n"
"font: 18px \"Comic Sans MS\";\n"
"border: 2px solid rgb(0, 200, 255);\n"
"border-radius: 10px;\n"
"width: 100px;\n"
"height: 25px;\n"
""));

        horizontalLayout_2->addWidget(picBtn);

        sendBtn = new QPushButton(centralwidget);
        sendBtn->setObjectName(QString::fromUtf8("sendBtn"));
        sendBtn->setStyleSheet(QString::fromUtf8("background-color: rgb(14, 118, 255);\n"
"color: white;\n"
"font: 18px \"Comic Sans MS\";\n"
"border: 2px solid rgb(0, 200, 255);\n"
"border-radius: 10px;\n"
"width: 100px;\n"
"height: 25px;\n"
""));

        horizontalLayout_2->addWidget(sendBtn);


        verticalLayout->addLayout(horizontalLayout_2);

        widget = new QWidget(centralwidget);
        widget->setObjectName(QString::fromUtf8("widget"));

        verticalLayout->addWidget(widget);

        sendMsg = new QTextEdit(centralwidget);
        sendMsg->setObjectName(QString::fromUtf8("sendMsg"));
        sendMsg->setStyleSheet(QString::fromUtf8("font: 10pt \"\345\215\216\346\226\207\347\273\206\351\273\221\";\n"
"background-color: rgba(255, 255, 255, 0.8);\n"
"border: 2px solid rgb(67, 67, 67);\n"
"border-radius: 18px;"));

        verticalLayout->addWidget(sendMsg);


        mainLayout->addLayout(verticalLayout);

        mainLayout->setStretch(1, 9);
        mainLayout->setStretch(2, 3);

        horizontalLayout_4->addLayout(mainLayout);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 955, 26));
        menuQt_TreeHole = new QMenu(menubar);
        menuQt_TreeHole->setObjectName(QString::fromUtf8("menuQt_TreeHole"));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuQt_TreeHole->menuAction());

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        refreshBtn->setText(QCoreApplication::translate("MainWindow", "Refresh", nullptr));
        background->setText(QCoreApplication::translate("MainWindow", "Background", nullptr));
        searchBtn->setText(QCoreApplication::translate("MainWindow", "Search", nullptr));
        subsBtn->setText(QCoreApplication::translate("MainWindow", "Subscirption", nullptr));
        orderFunc->setItemText(0, QCoreApplication::translate("MainWindow", "\346\234\200\346\226\260", nullptr));
        orderFunc->setItemText(1, QCoreApplication::translate("MainWindow", "\346\234\200\345\244\232\350\257\204\350\256\272", nullptr));
        orderFunc->setItemText(2, QCoreApplication::translate("MainWindow", "\346\234\200\345\244\232\345\205\263\346\263\250", nullptr));

        picBtn->setText(QCoreApplication::translate("MainWindow", "selectPic", nullptr));
        sendBtn->setText(QCoreApplication::translate("MainWindow", "Send", nullptr));
        menuQt_TreeHole->setTitle(QCoreApplication::translate("MainWindow", "Qt TreeHole", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
