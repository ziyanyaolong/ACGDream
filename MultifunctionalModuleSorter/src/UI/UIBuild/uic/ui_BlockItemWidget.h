/********************************************************************************
** Form generated from reading UI file 'BlockItemWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BLOCKITEMWIDGET_H
#define UI_BLOCKITEMWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_BlockItemWidgetClass
{
public:
    QGridLayout *gridLayout;
    QLabel *label;

    void setupUi(QWidget *BlockItemWidgetClass)
    {
        if (BlockItemWidgetClass->objectName().isEmpty())
            BlockItemWidgetClass->setObjectName(QString::fromUtf8("BlockItemWidgetClass"));
        BlockItemWidgetClass->resize(160, 100);
        BlockItemWidgetClass->setMinimumSize(QSize(160, 100));
        BlockItemWidgetClass->setMaximumSize(QSize(160, 100));
        gridLayout = new QGridLayout(BlockItemWidgetClass);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(BlockItemWidgetClass);
        label->setObjectName(QString::fromUtf8("label"));
        label->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label, 0, 0, 1, 1);


        retranslateUi(BlockItemWidgetClass);

        QMetaObject::connectSlotsByName(BlockItemWidgetClass);
    } // setupUi

    void retranslateUi(QWidget *BlockItemWidgetClass)
    {
        BlockItemWidgetClass->setWindowTitle(QCoreApplication::translate("BlockItemWidgetClass", "BlockItemWidget", nullptr));
        label->setText(QCoreApplication::translate("BlockItemWidgetClass", "null", nullptr));
    } // retranslateUi

};

namespace Ui {
    class BlockItemWidgetClass: public Ui_BlockItemWidgetClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BLOCKITEMWIDGET_H
