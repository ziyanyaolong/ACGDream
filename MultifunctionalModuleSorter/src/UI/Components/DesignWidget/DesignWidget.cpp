#include "DesignWidget.h"

DesignWidget::DesignWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	this->setAcceptDrops(true);
	//this->setMouseTracking(true);
	addEventFilterAllWidget();
}

DesignWidget::~DesignWidget()
{
}

void DesignWidget::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	QPen pen(Qt::black, 4);
	painter.setPen(pen);
	painter.drawRect(2, 2, this->width() - 4, this->height() - 4); //x,y,w,h
	QWidget::paintEvent(event);
}

bool DesignWidget::dragDropUIStart(QMouseEvent* event)
{
	QWidget* child = childAt(event->pos());

	if (child == nullptr)
	{
		return false;
	}

	DragDropUIData ddd;
	ddd.pWidget = child;
	ddd.hotSpot = event->pos() - child->pos();
	ddd.offset = ddd.hotSpot;
	QByteArray itemData;
	itemData.append(QString::number((qulonglong)(&ddd)));

	QDrag* drag = new QDrag(this);
	QMimeData* mimeData = new QMimeData();

	mimeData->setData("dd/data", itemData);

	//QPixmap pixmap(QPixmap::fromImage(child->grab().toImage()));

	//drag->setPixmap(pixmap);
	drag->setHotSpot(ddd.hotSpot);
	drag->setMimeData(mimeData);
	drag->exec();
	drag->deleteLater();
	mimeData->deleteLater();

	return true;
}

bool DesignWidget::dragDropUIMove(QDragMoveEvent* event)
{
	if (event->mimeData()->hasFormat("dd/data"))
	{
		event->setDropAction(Qt::MoveAction);

		DragDropUIData* ddd = (DragDropUIData*)(event->mimeData()->data("dd/data").toULongLong());
		ddd->pWidget->move(event->pos() - ddd->offset);
		ddd->offset = QPoint(event->pos() - ddd->pWidget->pos());

		event->accept();
		return true;
	}
	else
	{
		event->ignore();
		return false;
	}
}

bool DesignWidget::dragDropUIEnd(QDropEvent* event)
{
	if (event->mimeData()->hasFormat("dd/data"))
	{
		event->setDropAction(Qt::MoveAction); //c.移动操作
		DragDropUIData* ddd = (DragDropUIData*)(event->mimeData()->data("dd/data").toULongLong());
		ddd->pWidget->move(event->pos() - ddd->offset);
		ddd->offset = QPoint(event->pos() - ddd->pWidget->pos());

		event->accept();
		return true;
	}
	else
	{
		event->ignore();
		return false;
	}
}

void DesignWidget::allWidgetMove(WidgetMoveData& wmd)
{
	auto list = this->children();
	foreach (QObject* pObject, list)
	{
		QWidget* pWidget = static_cast<QWidget*>(pObject);
		if (pWidget == nullptr)
		{
			continue;
		}
		pWidget->move(pWidget->pos() + (wmd.newPoint - wmd.oldPoint));
	}
}

void DesignWidget::allWidgetZoom(WidgetZoomData& wzd)
{
	auto list = this->children();
	foreach(QObject * pObject, list)
	{
		QWidget* pWidget = static_cast<QWidget*>(pObject);
		if (pWidget == nullptr)
		{
			continue;
		}

		if (wzd.flag)
		{
			if (wzd.zoomCount < wzd.zoomCountMax)
			{
				pWidget->resize(pWidget->size() * wzd.zoomInMultiple);
			}
		}
		else
		{
			if (wzd.zoomCount > (-wzd.zoomCountMax))
			{
				pWidget->resize(pWidget->size() * wzd.zoomOutMultiple);
			}
		}
	}

	if (wzd.flag)
	{
		if (wzd.zoomCount < wzd.zoomCountMax)
		{
			wzd.zoomCount++;
		}
	}
	else
	{
		if (wzd.zoomCount > (-wzd.zoomCountMax))
		{
			wzd.zoomCount--;
		}
	}
}

void DesignWidget::addEventFilterAllWidget()
{
	auto list = findChildren<QObject*>();
	foreach(QObject * i, list)
	{
		i->installEventFilter(this);
	}
}

bool DesignWidget::eventFilter(QObject* object, QEvent* event)
{
	return QWidget::eventFilter(object, event);
}

void DesignWidget::mousePressEvent(QMouseEvent* event)
{
	switch (event->buttons())
	{
	case Qt::LeftButton:
		this->dragDropUIStart(event);
		break;

	case Qt::MiddleButton:
		wmd.flag = true;
		wmd.oldPoint = event->pos();
		break;

	default:
		break;
	}

	QWidget::mousePressEvent(event);
}

void DesignWidget::mouseReleaseEvent(QMouseEvent* event)
{
	//if (wmd.flag)
	//{
	//	wmd.flag = false;
	//	wmd.newPoint = event->pos();
	//	this->allWidgetMove(wmd.oldPoint, wmd.newPoint);
	//}

	if (wmd.flag)
	{
		wmd.flag = false;
	}

	QWidget::mouseReleaseEvent(event);
}

void DesignWidget::mouseDoubleClickEvent(QMouseEvent* event)
{
	QWidget::mouseDoubleClickEvent(event);
}

void DesignWidget::mouseMoveEvent(QMouseEvent* event)
{
	switch (event->buttons())
	{
	case Qt::LeftButton:
		this->dragDropUIStart(event);
		break;

	case Qt::MiddleButton:
		if (wmd.flag)
		{
			wmd.newPoint = event->pos();
			this->allWidgetMove(wmd);
			wmd.oldPoint = wmd.newPoint;
		}
		break;

	default:
		break;
	}
	

	QWidget::mouseMoveEvent(event);
}

void DesignWidget::wheelEvent(QWheelEvent* event)
{
	if ((event->buttons() == Qt::LeftButton) || (event->buttons() == Qt::RightButton))
	{
		QWidget::wheelEvent(event);
		return;
	}

	if (event->delta() > 0) {
		wzd.flag = true;
		this->allWidgetZoom(wzd);
	}
	else
	{
		wzd.flag = false;
		this->allWidgetZoom(wzd);
	}

	QWidget::wheelEvent(event);
}

void DesignWidget::dragEnterEvent(QDragEnterEvent* event)
{
	this->dragDropUIMove(event);
}

void DesignWidget::dragLeaveEvent(QDragLeaveEvent* event)
{
	QWidget::dragLeaveEvent(event);
}

void DesignWidget::dragMoveEvent(QDragMoveEvent* event)
{
	this->dragDropUIMove(static_cast<QDragMoveEvent*>(event));
}

void DesignWidget::dropEvent(QDropEvent* event)
{
	this->dragDropUIEnd(event);
}