/********************************************************************************
** Form generated from reading UI file 'MainUI.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINUI_H
#define UI_MAINUI_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>
#include "../../Components/DesignWidget/DesignWidget.h"

QT_BEGIN_NAMESPACE

class Ui_MainUI
{
public:
    DesignWidget *M_DesignWidget;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;

    void setupUi(QWidget *MainUI)
    {
        if (MainUI->objectName().isEmpty())
            MainUI->setObjectName(QString::fromUtf8("MainUI"));
        MainUI->resize(600, 400);
        M_DesignWidget = new DesignWidget(MainUI);
        M_DesignWidget->setObjectName(QString::fromUtf8("M_DesignWidget"));
        M_DesignWidget->setGeometry(QRect(50, 30, 451, 331));
        pushButton = new QPushButton(M_DesignWidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(10, 10, 75, 23));
        pushButton_2 = new QPushButton(M_DesignWidget);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(370, 300, 75, 23));
        pushButton_3 = new QPushButton(MainUI);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        pushButton_3->setGeometry(QRect(270, 370, 75, 23));

        retranslateUi(MainUI);

        QMetaObject::connectSlotsByName(MainUI);
    } // setupUi

    void retranslateUi(QWidget *MainUI)
    {
        MainUI->setWindowTitle(QCoreApplication::translate("MainUI", "MainUI", nullptr));
        pushButton->setText(QCoreApplication::translate("MainUI", "PushButton", nullptr));
        pushButton_2->setText(QCoreApplication::translate("MainUI", "PushButton", nullptr));
        pushButton_3->setText(QCoreApplication::translate("MainUI", "PushButton", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainUI: public Ui_MainUI {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINUI_H
