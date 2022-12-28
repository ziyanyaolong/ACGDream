#include "DesignWidget.h"

DesignWidget::DesignWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

DesignWidget::~DesignWidget()
{
}

void DesignWidget::mouseMoveEvent(QMouseEvent* event)
{
	QWidget::mouseMoveEvent(event);
}