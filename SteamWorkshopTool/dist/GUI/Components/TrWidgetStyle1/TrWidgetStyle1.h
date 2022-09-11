#pragma once

#include <QWidget>
#include <QPainter>

#include "ui_TrWidgetStyle1.h"

class TrWidgetStyle1 : public QWidget
{
	Q_OBJECT

public:
	explicit TrWidgetStyle1(QWidget *parent = nullptr);
	virtual ~TrWidgetStyle1();

protected:
	virtual void paintEvent(QPaintEvent* event) override;

private:
	Ui::TrWidgetStyle1Class ui;
};
