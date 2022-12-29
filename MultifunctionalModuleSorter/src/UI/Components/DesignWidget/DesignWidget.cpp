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
	if (event->buttons() & Qt::LeftButton)
	{
		QWidget* child = childAt(event->pos());

		if (child == nullptr)
		{
			return;
		}

		QString addr = QString::number((qulonglong)(child));
		QByteArray itemData;
		QDataStream dataStream(&itemData, QIODevice::WriteOnly);
		dataStream << addr << QPoint(event->pos() - child->pos());

		QDrag* drag = new QDrag(this);
		QMimeData* mimeData = new QMimeData();

		mimeData->setData("dd/data", itemData);

		QPixmap pixmap(QPixmap::fromImage(child->grab().toImage()));

		drag->setPixmap(pixmap);
		drag->setHotSpot(QPoint(20, 30));
		drag->setMimeData(mimeData);
		drag->exec();
	}
	QWidget::mousePressEvent(event);
}

void DesignWidget::mouseReleaseEvent(QMouseEvent* event)
{
	if (event->buttons() & Qt::LeftButton)
	{
	}

	QWidget::mouseReleaseEvent(event);
}

void DesignWidget::mouseDoubleClickEvent(QMouseEvent* event)
{
	QWidget::mouseDoubleClickEvent(event);
}

void DesignWidget::mouseMoveEvent(QMouseEvent* event)
{
	if (event->buttons() & Qt::LeftButton)
	{
		
	}

	QWidget::mouseMoveEvent(event);
}

void DesignWidget::dragEnterEvent(QDragEnterEvent* event)
{
	/*if (event->source() == this)
		event->ignore();
	else*/
	event->accept();
}

void DesignWidget::dragLeaveEvent(QDragLeaveEvent* event)
{
	//QWidget::dragLeaveEvent(event);
	QWidget::dragLeaveEvent(event);
}

void DesignWidget::dragMoveEvent(QDragMoveEvent* event)
{
	QWidget::dragMoveEvent(event);
}

void DesignWidget::dropEvent(QDropEvent* event)
{
	if (event->mimeData()->hasFormat("dd/data"))
	{
		QByteArray itemData = event->mimeData()->data("dd/data");
		QDataStream dataStream(&itemData, QIODevice::ReadOnly);
		QString addr;
		QPoint offset;
		dataStream >> addr >> offset;;

		QWidget* pWidget = (QWidget*)(addr.toULongLong());

		pWidget->move(event->pos()-offset);
		
		event->setDropAction(Qt::MoveAction); //c.移动操作
		event->accept();
	}
	else
	{
		event->ignore();
	}
}