#pragma once

#include <QWidget>
#include "ui_DesignWidget.h"

#include <QDebug>
#include <QMouseEvent>
#include <QPoint>

#include "../Blocks/RectangleBlock/RectangleBlock.h"

class DesignWidget : public QWidget
{
	Q_OBJECT

public:
	DesignWidget(QWidget *parent = nullptr);
	virtual ~DesignWidget();
	virtual void mouseMoveEvent(QMouseEvent* event) override;

private:
	Ui::DesignWidgetClass ui;

};
