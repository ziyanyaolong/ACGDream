#pragma once

#include <QWidget>
#include "ui_RectangleBlock.h"

#include <QPaintEvent>
#include <QPoint>

#include "../../DesignComponentObject/DesignComponentObject.h"

class RectangleBlock : public DesignComponentObject
{
	Q_OBJECT

public:
	explicit RectangleBlock(DesignComponentObject* parent = nullptr);
	virtual ~RectangleBlock();

protected:
	virtual void paintEvent(QPaintEvent* event) override;

private:
	Ui::RectangleBlockClass ui;
};
