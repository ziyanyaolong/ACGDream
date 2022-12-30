#pragma once

#include <QWidget>

class BlockObject  : public QWidget
{
	Q_OBJECT

public:
	explicit BlockObject(QWidget* parent = Q_NULLPTR);
	virtual ~BlockObject();
};
