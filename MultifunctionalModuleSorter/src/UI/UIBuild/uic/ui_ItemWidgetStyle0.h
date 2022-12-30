/********************************************************************************
** Form generated from reading UI file 'ItemWidgetStyle0.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ITEMWIDGETSTYLE0_H
#define UI_ITEMWIDGETSTYLE0_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ItemWidgetStyle0Class
{
public:
    QGridLayout *gridLayout;
    QLabel *label;

    void setupUi(QWidget *ItemWidgetStyle0Class)
    {
        if (ItemWidgetStyle0Class->objectName().isEmpty())
            ItemWidgetStyle0Class->setObjectName(QString::fromUtf8("ItemWidgetStyle0Class"));
        ItemWidgetStyle0Class->resize(160, 100);
        ItemWidgetStyle0Class->setMinimumSize(QSize(160, 100));
        ItemWidgetStyle0Class->setMaximumSize(QSize(160, 100));
        gridLayout = new QGridLayout(ItemWidgetStyle0Class);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(ItemWidgetStyle0Class);
        label->setObjectName(QString::fromUtf8("label"));
        label->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label, 0, 0, 1, 1);


        retranslateUi(ItemWidgetStyle0Class);

        QMetaObject::connectSlotsByName(ItemWidgetStyle0Class);
    } // setupUi

    void retranslateUi(QWidget *ItemWidgetStyle0Class)
    {
        ItemWidgetStyle0Class->setWindowTitle(QCoreApplication::translate("ItemWidgetStyle0Class", "ItemWidgetStyle0", nullptr));
        label->setText(QCoreApplication::translate("ItemWidgetStyle0Class", "null", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ItemWidgetStyle0Class: public Ui_ItemWidgetStyle0Class {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ITEMWIDGETSTYLE0_H
