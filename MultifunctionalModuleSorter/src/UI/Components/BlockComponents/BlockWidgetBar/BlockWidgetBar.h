#pragma once

#include <QWidget>
#include "ui_BlockWidgetBar.h"

#include "BlockItemWidget/Style/0/ItemWidgetStyle0.h"
#include "../Blocks/BlockObject/BlockObject.h"

class BlockWidgetBar : public QWidget
{
	Q_OBJECT

public:
	explicit BlockWidgetBar(QWidget *parent = Q_NULLPTR);
	virtual ~BlockWidgetBar();

private:
	Ui::BlockWidgetBarClass ui;

signals:
	void addBlockError(const QString& str);

public slots:
	void addBlock(BlockObject* block);
};
