#include "ScrollAreaStyle1.h"

ScrollAreaStyle1::ScrollAreaStyle1(QWidget* parent)
	: QScrollArea(parent)
{
	//QPalette pa = this->palette();
	//pa.setBrush(QPalette::Window, Qt::transparent);
	//this->setPalette(pa);
	timer = new QTimer(this);
	setAttribute(Qt::WA_Hover, true);
	setFrameShape(QFrame::NoFrame);
	QFile* file = new QFile("G:/QT/VS/ACGDream/ACGDreamFrame/common/Components/StyleSheet/ScrollBar/ScrollBarStyle1.qss");
	if (file->open(QFile::ReadOnly | QIODevice::Text))
		this->verticalScrollBar()->setStyleSheet(file->readAll());

	((QWidget*)(this->verticalScrollBar()))->installEventFilter(this);

	graphicsOpacityEffect = new QGraphicsOpacityEffect(((QObject*)(this->verticalScrollBar())));
	((QWidget*)(this->verticalScrollBar()))->setGraphicsEffect(graphicsOpacityEffect);

	fadeIn = new QPropertyAnimation((QObject*)(graphicsOpacityEffect), "opacity", this);
	fadeIn->stop();
	fadeIn->setDuration(300);
	fadeIn->setStartValue(0);
	fadeIn->setEndValue(1);
	fadeIn->setEasingCurve(QEasingCurve::Linear);

	fadeOut = new QPropertyAnimation((QObject*)(graphicsOpacityEffect), "opacity", this);
	fadeOut->stop();
	fadeOut->setDuration(500);
	fadeOut->setStartValue(1);
	fadeOut->setEndValue(0);
	fadeOut->setEasingCurve(QEasingCurve::Linear);

	setStyleSheet("background-color:transparent;");

	this->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);

	connect(timer, &QTimer::timeout, this, &ScrollAreaStyle1::closeScrollBar);

	connect(fadeIn, &QPropertyAnimation::finished, this, [&]() {
		fadeIn->stop();
		});
	connect(fadeOut, &QPropertyAnimation::finished, this, [&]() {
		setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		fadeOut->stop();
		});

}

ScrollAreaStyle1::~ScrollAreaStyle1()
{
	if (timer)
	{
		timer->stop();
		delete timer;
	}
	if (fadeIn)
	{
		fadeIn->stop();
		delete fadeIn;
	}
	if (fadeOut)
	{
		fadeOut->stop();
		delete fadeOut;
	}
}

bool ScrollAreaStyle1::eventFilter(QObject* target, QEvent* event)
{
	if (target == (QObject*)(this->verticalScrollBar()))
	{
		if ((event->type() == QEvent::MouseMove) || (event->type() == QEvent::MouseButtonPress) || (event->type() == QEvent::HoverEnter) || (event->type() == QEvent::Wheel))
		{
			emit this->openScrollBar();
		}
	}
	return QScrollArea::eventFilter(target, event);
}

void ScrollAreaStyle1::closeScrollBar()
{
	fadeOut->start();
	timer->stop();
}

void ScrollAreaStyle1::openScrollBar()
{
	if (this->verticalScrollBarPolicy() == Qt::ScrollBarAlwaysOff)
	{
		setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
		fadeIn->start();
	}
	else
	{
		fadeIn->stop();
		fadeOut->stop();
		graphicsOpacityEffect->setOpacity(1);
		setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	}
	timer->stop();
	timer->start(lifetime);
}