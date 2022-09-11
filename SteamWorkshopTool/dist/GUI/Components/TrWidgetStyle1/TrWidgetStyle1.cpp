#include "TrWidgetStyle1.h"

TrWidgetStyle1::TrWidgetStyle1(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

TrWidgetStyle1::~TrWidgetStyle1()
{}

void TrWidgetStyle1::paintEvent(QPaintEvent* event)
{

	QPainter p(this);
	p.setPen(QColor(144, 144, 144, 150));
	p.setBrush(QColor(144, 144, 144, 150));
	p.drawRoundRect(this->rect(), 8, 8);
	QWidget::paintEvent(event);
}