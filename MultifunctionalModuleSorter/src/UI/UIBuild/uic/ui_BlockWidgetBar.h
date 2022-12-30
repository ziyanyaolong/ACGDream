/********************************************************************************
** Form generated from reading UI file 'BlockWidgetBar.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BLOCKWIDGETBAR_H
#define UI_BLOCKWIDGETBAR_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_BlockWidgetBarClass
{
public:
    QGridLayout *gridLayout;
    QListWidget *listWidget;

    void setupUi(QWidget *BlockWidgetBarClass)
    {
        if (BlockWidgetBarClass->objectName().isEmpty())
            BlockWidgetBarClass->setObjectName(QString::fromUtf8("BlockWidgetBarClass"));
        BlockWidgetBarClass->resize(160, 400);
        BlockWidgetBarClass->setMinimumSize(QSize(160, 0));
        BlockWidgetBarClass->setMaximumSize(QSize(160, 16777215));
        gridLayout = new QGridLayout(BlockWidgetBarClass);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        listWidget = new QListWidget(BlockWidgetBarClass);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));

        gridLayout->addWidget(listWidget, 0, 0, 1, 1);


        retranslateUi(BlockWidgetBarClass);

        QMetaObject::connectSlotsByName(BlockWidgetBarClass);
    } // setupUi

    void retranslateUi(QWidget *BlockWidgetBarClass)
    {
        BlockWidgetBarClass->setWindowTitle(QCoreApplication::translate("BlockWidgetBarClass", "BlockWidgetBar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class BlockWidgetBarClass: public Ui_BlockWidgetBarClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BLOCKWIDGETBAR_H
