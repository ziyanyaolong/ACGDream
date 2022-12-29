#pragma once

#include <QWidget>
#include "ui_DesignWidget.h"

#include <QDebug>
#include <QMouseEvent>
#include <QPoint>
#include <QDrag>
#include <QMimeData>
#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QDragMoveEvent>
#include <QDropEvent>

#include <QByteArray>
#include <QDataStream>

#include <QPaintEvent>
#include <QPainter>
#include <QBrush>
#include <QPen>

#include "../Blocks/RectangleBlock/RectangleBlock.h"

class DesignWidget : public QWidget
{
	Q_OBJECT

public:
	DesignWidget(QWidget *parent = nullptr);
	virtual ~DesignWidget();

protected:
	virtual bool eventFilter(QObject* watched, QEvent* event) override;
	virtual void mousePressEvent(QMouseEvent* event) override;
	virtual void mouseReleaseEvent(QMouseEvent* event) override;
	virtual void mouseDoubleClickEvent(QMouseEvent* event) override;
	virtual void mouseMoveEvent(QMouseEvent* event) override;
	virtual void dragEnterEvent(QDragEnterEvent* event) override;
	virtual void dragLeaveEvent(QDragLeaveEvent* event) override;
	virtual void dragMoveEvent(QDragMoveEvent* event) override;
	virtual void dropEvent(QDropEvent* event) override;

	virtual void paintEvent(QPaintEvent*) override;

private:
	Ui::DesignWidgetClass ui;

	void addEventFilterAllWidget();

};
