#pragma once

#include <QObject>
#include <QListWidget>
#include <QListWidgetItem>
#include <QPixmap>
#include <QSize>
#include <QDebug>
#include <QListWidgetItem>

#include "Widget/Widget1.h"

class ListWidgetItemWidget : public QObject
{
	Q_OBJECT

private:
	QListWidgetItem* listWidgetItem = nullptr;
	Widget1* widget = nullptr;

public:
	explicit ListWidgetItemWidget(QListWidget* parent);
	virtual ~ListWidgetItemWidget();

public slots:
	Ui::Widget1* readUi() { return &(widget->ui); }
	QListWidgetItem* readItem() { return listWidgetItem; }
	Widget1* readWidget() { return widget; }

signals:
	void pushButtonResponse_Subscription(bool isSubscription);

};
