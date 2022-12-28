#include "DesignComponentObject.h"

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
		isMoving_ = true;
		QDrag* drag = new QDrag(this);
		QMimeData* data = new QMimeData;
		drag->setMimeData(data);
		drag->exec(Qt::MoveAction);
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
	//if (event->buttons() & Qt::LeftButton)
	//{
	//	point = this->mapFromGlobal(QCursor().pos());
	//	this->move(point);
	//	qDebug() << point;
	//}

	QWidget::mouseMoveEvent(event);
}

void DesignComponentObject::dragEnterEvent(QDragEnterEvent* event)
{
	//if (event->source() == this)
	//	event->ignore();
	//else
	//	event->accept();
}

void DesignComponentObject::dragLeaveEvent(QDragLeaveEvent* event)
{
	//QWidget::dragLeaveEvent(event);
}

void DesignComponentObject::dragMoveEvent(QDragMoveEvent* event)
{
	//this->move(event->pos());
	//this->update();
}

void DesignComponentObject::dropEvent(QDropEvent* event)
{
	//event->setDropAction(Qt::LinkAction);
	//event->accept();
}