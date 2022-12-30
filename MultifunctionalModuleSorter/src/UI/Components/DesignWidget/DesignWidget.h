#pragma once

#include <QWidget>
#include "ui_DesignWidget.h"

#include <QDebug>

#include <QVector>

#include <QMouseEvent>
#include <QWheelEvent>

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

#include <QTimer>

//#include "../RectangleBlock/RectangleBlock.h"

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
	virtual void wheelEvent(QWheelEvent* event) override;

	virtual void dragEnterEvent(QDragEnterEvent* event) override;
	virtual void dragLeaveEvent(QDragLeaveEvent* event) override;
	virtual void dragMoveEvent(QDragMoveEvent* event) override;
	virtual void dropEvent(QDropEvent* event) override;

	virtual void paintEvent(QPaintEvent*) override;

private:
	struct DragDropUIData
	{
		QPoint offset;
		QPoint hotSpot;
		QWidget* pWidget = nullptr;
	};

	struct WidgetMoveData
	{
		QPoint oldPoint;
		QPoint newPoint;
		bool flag = false;
	};

	struct WidgetZoomData
	{
		double zoomInMultiple = 1.2;
		double zoomOutMultiple = 0.8;
		qint16 zoomCount = 0;
		qint16 zoomCountMax = 5;
		bool flag = false;
	};

	Ui::DesignWidgetClass ui;

	WidgetMoveData wmd;
	WidgetZoomData wzd;

	void addEventFilterAllWidget();

	bool dragDropUIStart(QMouseEvent* event);
	bool dragDropUIMove(QDragMoveEvent* event);
	bool dragDropUIEnd(QDropEvent* event);
	
	void allWidgetMove(WidgetMoveData& wmd);
	void allWidgetZoom(WidgetZoomData& wzd);

	
};
