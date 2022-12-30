#pragma once

#include <QWidget>
#include "ui_RectangleBlock.h"

#include <QPaintEvent>
#include <QPoint>

#include "../BlockObject/BlockObject.h"

class RectangleBlock : public BlockObject
{
	Q_OBJECT

public:
	explicit RectangleBlock(BlockObject* parent = Q_NULLPTR);
	virtual ~RectangleBlock();

protected:
	//virtual void paintEvent(QPaintEvent* event) override;

private:
	Ui::RectangleBlockClass ui;
};
