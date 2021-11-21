#pragma once

#include <QWidget>
#include <QListWidgetItem>
#include <QPixmap>
#include <QSize>
#include <QDebug>

#include "ui_ListWidgetItemWidget.h"

class ListWidgetItemWidget : public QWidget
{
	Q_OBJECT

public:
	explicit ListWidgetItemWidget(QWidget *parent = Q_NULLPTR, QListWidgetItem* listWidgetItem = Q_NULLPTR);
	~ListWidgetItemWidget();

	const QString& readId() { return mid; }
	const QString& readTitle() { return ui.label_2->text(); }

	QListWidgetItem* _listWidgetItem = nullptr;

private:
	bool _isSubscription = false;
	static QIcon YSIcon;
	static QIcon NSIcon;
	Ui::ListWidgetItemWidget ui;
	QPixmap _image;
	QString mid = "";

signals:
	void pushButtonResponse_Subscription(bool isSubscription);

public slots:
	Ui::ListWidgetItemWidget* readUi() { return &ui; }
	void setSubscription(bool subscription);
	void setId(const QString& id) { mid = id; }
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
