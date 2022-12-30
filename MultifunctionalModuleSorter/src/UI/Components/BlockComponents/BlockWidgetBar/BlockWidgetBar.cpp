#include "BlockWidgetBar.h"

BlockWidgetBar::BlockWidgetBar(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	ui.listWidget;
}

BlockWidgetBar::~BlockWidgetBar()
{
}

void BlockWidgetBar::addBlock(BlockObject* block)
{
	if (block == nullptr)
	{
		emit addBlockError("Empty pointer!");
		return;
	}

	QListWidgetItem* item = new QListWidgetItem(ui.listWidget);
	BlockItemWidgetStyle::ItemWidgetStyle0* style0  = new BlockItemWidgetStyle::ItemWidgetStyle0(ui.listWidget);
	style0->setPixmap(QPixmap());

	ui.listWidget->addItem(item);
	ui.listWidget->setItemWidget(item, style0);
}
