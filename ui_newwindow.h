/********************************************************************************
** Form generated from reading UI file 'newwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NEWWINDOW_H
#define UI_NEWWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_newWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QWidget *widget;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton;
    QPushButton *repoBtn;
    QPushButton *refreshBtn;
    QPushButton *subcBtn;
    QSpacerItem *horizontalSpacer_2;
    QWidget *widget_2;
    QHBoxLayout *horizontalLayout;
    QListWidget *mac_listWidget;
    QWidget *widget_4;
    QHBoxLayout *horizontalLayout_2;
    QTextEdit *sendCmt;
    QPushButton *sendBtn;
    QStatusBar *statusbar;
    QMenuBar *menubar;

    void setupUi(QMainWindow *newWindow)
    {
        if (newWindow->objectName().isEmpty())
            newWindow->setObjectName(QString::fromUtf8("newWindow"));
        newWindow->resize(900, 1200);
        centralwidget = new QWidget(newWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        widget = new QWidget(centralwidget);
        widget->setObjectName(QString::fromUtf8("widget"));
        horizontalLayout_3 = new QHBoxLayout(widget);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalSpacer = new QSpacerItem(277, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);

        pushButton = new QPushButton(widget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setMinimumSize(QSize(100, 50));
        QFont font;
        font.setPointSize(15);
        pushButton->setFont(font);

        horizontalLayout_3->addWidget(pushButton);

        repoBtn = new QPushButton(widget);
        repoBtn->setObjectName(QString::fromUtf8("repoBtn"));
        repoBtn->setMinimumSize(QSize(100, 50));
        repoBtn->setMaximumSize(QSize(100, 50));
        repoBtn->setFont(font);

        horizontalLayout_3->addWidget(repoBtn);

        refreshBtn = new QPushButton(widget);
        refreshBtn->setObjectName(QString::fromUtf8("refreshBtn"));
        refreshBtn->setMinimumSize(QSize(100, 50));
        refreshBtn->setMaximumSize(QSize(100, 50));
        refreshBtn->setFont(font);

        horizontalLayout_3->addWidget(refreshBtn);

        subcBtn = new QPushButton(widget);
        subcBtn->setObjectName(QString::fromUtf8("subcBtn"));
        subcBtn->setMinimumSize(QSize(100, 50));
        subcBtn->setMaximumSize(QSize(100, 50));
        subcBtn->setFont(font);

        horizontalLayout_3->addWidget(subcBtn);

        horizontalSpacer_2 = new QSpacerItem(277, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_2);


        verticalLayout->addWidget(widget);

        widget_2 = new QWidget(centralwidget);
        widget_2->setObjectName(QString::fromUtf8("widget_2"));
        horizontalLayout = new QHBoxLayout(widget_2);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        mac_listWidget = new QListWidget(widget_2);
        mac_listWidget->setObjectName(QString::fromUtf8("mac_listWidget"));

        horizontalLayout->addWidget(mac_listWidget);


        verticalLayout->addWidget(widget_2);

        widget_4 = new QWidget(centralwidget);
        widget_4->setObjectName(QString::fromUtf8("widget_4"));
        horizontalLayout_2 = new QHBoxLayout(widget_4);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        sendCmt = new QTextEdit(widget_4);
        sendCmt->setObjectName(QString::fromUtf8("sendCmt"));

        horizontalLayout_2->addWidget(sendCmt);

        sendBtn = new QPushButton(widget_4);
        sendBtn->setObjectName(QString::fromUtf8("sendBtn"));
        sendBtn->setMinimumSize(QSize(80, 160));
        sendBtn->setMaximumSize(QSize(80, 160));
        sendBtn->setFont(font);

        horizontalLayout_2->addWidget(sendBtn);


        verticalLayout->addWidget(widget_4);

        verticalLayout->setStretch(0, 1);
        verticalLayout->setStretch(1, 22);
        verticalLayout->setStretch(2, 4);
        newWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(newWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        newWindow->setStatusBar(statusbar);
        menubar = new QMenuBar(newWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 900, 21));
        newWindow->setMenuBar(menubar);

        retranslateUi(newWindow);

        QMetaObject::connectSlotsByName(newWindow);
    } // setupUi

    void retranslateUi(QMainWindow *newWindow)
    {
        newWindow->setWindowTitle(QCoreApplication::translate("newWindow", "MainWindow", nullptr));
        pushButton->setText(QCoreApplication::translate("newWindow", "\350\203\214\346\231\257", nullptr));
        repoBtn->setText(QCoreApplication::translate("newWindow", "\344\270\276\346\212\245", nullptr));
        refreshBtn->setText(QCoreApplication::translate("newWindow", "\345\210\267\346\226\260", nullptr));
        subcBtn->setText(QCoreApplication::translate("newWindow", "\345\205\263\346\263\250", nullptr));
        sendBtn->setText(QCoreApplication::translate("newWindow", "send", nullptr));
    } // retranslateUi

};

namespace Ui {
    class newWindow: public Ui_newWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NEWWINDOW_H
