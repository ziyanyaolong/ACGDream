#include "DesignComponentObject.h"

//1.当我们鼠标 点击 send widget 时候 会触发 mousePressEvent
//2. 当我们按压鼠标 移动的过程中 会触发 mouseMoveEvent
//3. 当我们拖拽鼠标进入窗口 recv widget 会触发 dragEnterEvent
//4. 当我们松开鼠标 会触发 dropEvent

DesignComponentObject::DesignComponentObject(QWidget*parent)
	: QWidget(parent)
{
	point = QPoint();
}

DesignComponentObject::~DesignComponentObject()
{
}

void DesignComponentObject::mousePressEvent(QMouseEvent* event)
{
	if (event->buttons() & Qt::LeftButton)
	{
	}
	QWidget::mousePressEvent(event);
}

void DesignComponentObject::mouseReleaseEvent(QMouseEvent* event)
{
	if (event->buttons() & Qt::LeftButton)
	{
		isMoving_ = false;
	}

	QWidget::mouseReleaseEvent(event);
}

void DesignComponentObject::mouseDoubleClickEvent(QMouseEvent* event)
{
	QWidget::mouseDoubleClickEvent(event);
}

void DesignComponentObject::mouseMoveEvent(QMouseEvent* event)
{
	QWidget::mouseMoveEvent(event);
}

void DesignComponentObject::dragEnterEvent(QDragEnterEvent* event)
{
	/*if (event->source() == this)
		event->ignore();
	else*/
		event->accept();
}

void DesignComponentObject::dragLeaveEvent(QDragLeaveEvent* event)
{
	//QWidget::dragLeaveEvent(event);
	QWidget::dragLeaveEvent(event);
}

void DesignComponentObject::dragMoveEvent(QDragMoveEvent* event)
{
	QWidget::dragMoveEvent(event);
}

void DesignComponentObject::dropEvent(QDropEvent* event)
{
	//event->setDropAction(Qt::LinkAction);
	//event->accept();
	QWidget::dropEvent(event);
}