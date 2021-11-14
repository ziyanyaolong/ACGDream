#include "PushButtonStyle1.h"

PushButtonStyle1::PushButtonStyle1(QWidget* parent)
	: QPushButton(parent)
{
}

PushButtonStyle1::~PushButtonStyle1()
{

}

void PushButtonStyle1::paintEvent(QPaintEvent* event)
{
	QPainter painter;
	painter.begin(this);
	painter.setPen(Qt::NoPen);
	painter.setBrush(QColor(0x00, 0x00, 0x00, 50)); // ���һ��Ϊ͸����  
	painter.setOpacity(0.6);
	painter.drawRoundRect(0 + cohesionSize, 0 + cohesionSize, width() - cohesionSize, height() - cohesionSize, 10, 10);// ��Բ�Ǿ���  
	painter.end();
}