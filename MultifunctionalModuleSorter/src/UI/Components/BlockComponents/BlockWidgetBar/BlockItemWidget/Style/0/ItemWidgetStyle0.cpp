#include "ItemWidgetStyle0.h"

BlockItemWidgetStyle::ItemWidgetStyle0::ItemWidgetStyle0(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

BlockItemWidgetStyle::ItemWidgetStyle0::~ItemWidgetStyle0()
{}

void BlockItemWidgetStyle::ItemWidgetStyle0::setPixmap(const QPixmap& pixmap)
{
	ui.label->setPixmap(pixmap);
}