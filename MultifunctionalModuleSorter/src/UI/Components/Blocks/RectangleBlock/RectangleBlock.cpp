#include "RectangleBlock.h"

RectangleBlock::RectangleBlock(DesignComponentObject* parent)
	: DesignComponentObject(parent)
{
	ui.setupUi(this);
}

RectangleBlock::~RectangleBlock()
{
}

void RectangleBlock::paintEvent(QPaintEvent* event)
{
	//if (this->isMoving())
	//{
	//	this->move(this->getPoint());
	//}
	DesignComponentObject::paintEvent(event);
}
