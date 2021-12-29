#pragma once

#include <QWidget>
#include <QListWidgetItem>
#include <QPixmap>
#include <QSize>
#include <QDebug>

#include "ui_Widget1.h"

class Widget1 : public QWidget
{
	Q_OBJECT

private:
	friend class ListWidgetItemWidget;

public:
	enum class Star
	{
		Zero,
		One,
		Two,
		Three,
		Four,
		Five
	};

	explicit Widget1(QWidget* parent = Q_NULLPTR);
	virtual ~Widget1();

	const QString& readId() { return mid; }
	const QString& readTitle() { return ui.label_2->text(); }
	void setId(const QString& id) { mid = id; }
	void setTitle(const QString& title) { ui.label_2->setText(title); }

	void setSubscription(bool subscription);
	void setImage(const QPixmap& image);
	void setImage(const QString& image);
	void setStar(Star star);

private:
	bool _isSubscription = false;
	static QIcon YSIcon;
	static QIcon NSIcon;
	Ui::Widget1 ui;
	QPixmap _image;
	QString mid = "";
};
