#include "GUIMain.h"

GUIMain::GUIMain(QMainWindow*parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	background = QSharedPointer<QImage>(new QImage("F:/1.jpg"));
}

GUIMain::~GUIMain()
{
	clearAllWidget();
	close();
}

void GUIMain::clearAllWidget()
{
	for (QMap<QPushButton*, QWidget*>::iterator i = pushButtonMap.begin(); i != pushButtonMap.end(); i++)
	{
		if (i.key())
		{
			auto tempWidget = i.value();
			if (tempWidget)
				tempWidget->deleteLater();
			i.key()->close();
			i.key()->deleteLater();
		}
	}
	pushButtonMap.clear();
}

void GUIMain::paintEvent(QPaintEvent* event)
{
	QPainter painter;
	painter.begin(this);
	painter.fillRect(QRect(0, 0, this->width(), this->height()), QColor(100,10,150,30));
	painter.drawImage(QRect(0, 0, this->width(), this->height()), *background);
	painter.end();
	this->QWidget::paintEvent(event);
	if (!beginEndValue)
	{
		emit this->beginEnd();
		beginEndValue = true;
	}
}

void GUIMain::resizeEvent(QResizeEvent* event)
{
	foreach(QWidget* i, ui.widget_3->findChildren<QWidget*>())
	{
		if (!(i->isHidden()) && widgetList.count(i) > 0)
		{
			i->resize(ui.widget_3->width(), ui.widget_3->height());
			break;
		}
	}
	this->QWidget::resizeEvent(event);
}

QPushButton* GUIMain::addWidght(QWidget* widget)
{
	PushButtonStyle1* pushButton = new PushButtonStyle1(ui.splitter);
	//QPropertyAnimation* pPosAnimation1 = new QPropertyAnimation(pushButton, "pos", ui.splitter);
	pushButton->setMinimumHeight(50);
	pushButton->setMinimumWidth(80);
	ui.splitter->addWidget(pushButton);
	pushButton->show();
	widget->hide();
	widget->setParent(ui.widget_3);
	pushButtonMap[pushButton] = widget;
	if (widgetList.count(widget) <= 0)
	{
		widgetList.push_back(widget);
	}
	else
	{
		widgetList[widgetList.indexOf(widget)] = widget;
	}
	
	//pPosAnimation1->setDuration(1000);
	//pPosAnimation1->setEasingCurve(QEasingCurve::InOutQuad);

	//propertyAnimationMap[pushButton] = pPosAnimation1;
	
	//connect(pPosAnimation1, &QPropertyAnimation::finished, pPosAnimation1, &QPropertyAnimation::stop);

	connect(pushButton, &PushButtonStyle1::clicked, this, [&]() {
		QPushButton* tempSenderObject = (QPushButton*)sender();
		QWidget* tempWidgetP = pushButtonMap[tempSenderObject];
		if (tempWidgetP->isHidden())
		{
			tempWidgetP->resize(ui.widget_3->width(), ui.widget_3->height());
			foreach(QWidget * i, ui.widget_3->findChildren<QWidget*>())
			{
				if ((i != tempWidgetP) && (!(i->isHidden())) && widgetList.count(i) > 0)
					i->hide();
			}
			/*if (propertyAnimationMap.count(tempSenderObject) > 0)
			{
				QPropertyAnimation* posAnimation1 = propertyAnimationMap[tempSenderObject];
				const QPoint& tempPoint(tempSenderObject->pos());
				posAnimation1->stop();
				posAnimation1->setStartValue(QPoint(tempPoint.x() - 100, tempPoint.y()));
				posAnimation1->setEndValue(tempPoint);
				posAnimation1->start();
			}*/
			tempWidgetP->show();
		}
		else
		{
			tempWidgetP->hide();
		}
		});

	return pushButton;
}