/********************************************************************************
** Form generated from reading UI file 'RectangleBlock.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RECTANGLEBLOCK_H
#define UI_RECTANGLEBLOCK_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RectangleBlockClass
{
public:
    QGridLayout *gridLayout;
    QPushButton *pushButton;

    void setupUi(QWidget *RectangleBlockClass)
    {
        if (RectangleBlockClass->objectName().isEmpty())
            RectangleBlockClass->setObjectName(QString::fromUtf8("RectangleBlockClass"));
        RectangleBlockClass->resize(108, 50);
        gridLayout = new QGridLayout(RectangleBlockClass);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        pushButton = new QPushButton(RectangleBlockClass);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        gridLayout->addWidget(pushButton, 0, 0, 1, 1);


        retranslateUi(RectangleBlockClass);

        QMetaObject::connectSlotsByName(RectangleBlockClass);
    } // setupUi

    void retranslateUi(QWidget *RectangleBlockClass)
    {
        RectangleBlockClass->setWindowTitle(QCoreApplication::translate("RectangleBlockClass", "RectangleBlock", nullptr));
        pushButton->setText(QCoreApplication::translate("RectangleBlockClass", "PushButton", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RectangleBlockClass: public Ui_RectangleBlockClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RECTANGLEBLOCK_H
