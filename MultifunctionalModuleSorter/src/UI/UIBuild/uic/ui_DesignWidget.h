/********************************************************************************
** Form generated from reading UI file 'DesignWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DESIGNWIDGET_H
#define UI_DESIGNWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DesignWidgetClass
{
public:

    void setupUi(QWidget *DesignWidgetClass)
    {
        if (DesignWidgetClass->objectName().isEmpty())
            DesignWidgetClass->setObjectName(QString::fromUtf8("DesignWidgetClass"));
        DesignWidgetClass->resize(600, 400);

        retranslateUi(DesignWidgetClass);

        QMetaObject::connectSlotsByName(DesignWidgetClass);
    } // setupUi

    void retranslateUi(QWidget *DesignWidgetClass)
    {
        DesignWidgetClass->setWindowTitle(QCoreApplication::translate("DesignWidgetClass", "DesignWidget", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DesignWidgetClass: public Ui_DesignWidgetClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DESIGNWIDGET_H
