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
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QWidget>
#include "../../Components/DesignWidget/DesignWidget.h"

QT_BEGIN_NAMESPACE

class Ui_MainUI
{
public:
    QHBoxLayout *horizontalLayout;
    QWidget *widget;
    DesignWidget *M_DesignWidget;

    void setupUi(QWidget *MainUI)
    {
        if (MainUI->objectName().isEmpty())
            MainUI->setObjectName(QString::fromUtf8("MainUI"));
        MainUI->resize(600, 400);
        horizontalLayout = new QHBoxLayout(MainUI);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        widget = new QWidget(MainUI);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setMinimumSize(QSize(160, 0));
        widget->setMaximumSize(QSize(160, 16777215));

        horizontalLayout->addWidget(widget);

        M_DesignWidget = new DesignWidget(MainUI);
        M_DesignWidget->setObjectName(QString::fromUtf8("M_DesignWidget"));

        horizontalLayout->addWidget(M_DesignWidget);


        retranslateUi(MainUI);

        QMetaObject::connectSlotsByName(MainUI);
    } // setupUi

    void retranslateUi(QWidget *MainUI)
    {
        MainUI->setWindowTitle(QCoreApplication::translate("MainUI", "MainUI", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainUI: public Ui_MainUI {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINUI_H
