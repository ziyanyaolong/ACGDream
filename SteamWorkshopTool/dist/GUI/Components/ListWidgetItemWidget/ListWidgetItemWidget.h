#pragma once

#include <QWidget>
#include <QListWidgetItem>
#include <QPixmap>
#include <QSize>
#include "ui_ListWidgetItemWidget.h"

class ListWidgetItemWidget : public QWidget
{
	Q_OBJECT

public:
	explicit ListWidgetItemWidget(QWidget *parent = Q_NULLPTR, QListWidgetItem* listWidgetItem = Q_NULLPTR);
	~ListWidgetItemWidget();

	QListWidgetItem* _listWidgetItem;

private:
	static QIcon YSIcon;
	static QIcon NSIcon;
	Ui::ListWidgetItemWidget ui;
	QPixmap _image;

public slots:
	void setSubscription(bool subscription);
	void setTitle(const QString& title) { ui.label_2->setText(title); }
	void setImage(const QPixmap& image)
	{
		_image = image;
		_image = _image.scaled(QSize(ui.label->width(), ui.label->height()));
		ui.label->setPixmap(_image);
	}

	void setImage(const QString& image)
	{
		if (!_image.load(image))
			return;
		_image = _image.scaled(QSize(ui.label->width(), ui.label->height()));
		ui.label->setPixmap(_image);
	}
};
