#pragma once

#include <QWidget>
#include <QMouseEvent>

#include <QDebug>
#include <QDrag>
#include <QMimeData>
#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QDragMoveEvent>
#include <QDropEvent>

class DesignComponentObject : public QWidget
{
	Q_OBJECT

public:
	explicit DesignComponentObject(QWidget* parent);
	virtual ~DesignComponentObject();

	inline bool isMoving() { return isMoving_; }
	inline QPoint getPoint() { return point; }
	
protected:
	virtual void mousePressEvent(QMouseEvent* event) override;
	virtual void mouseReleaseEvent(QMouseEvent* event) override;
	virtual void mouseDoubleClickEvent(QMouseEvent* event) override;
	virtual void mouseMoveEvent(QMouseEvent* event) override;
	virtual void dragEnterEvent(QDragEnterEvent* event) override;
	virtual void dragLeaveEvent(QDragLeaveEvent* event) override;
	virtual void dragMoveEvent(QDragMoveEvent* event) override;
	virtual void dropEvent(QDropEvent* event) override;

private:
	QPoint point;
	bool isMoving_ = false;
};
