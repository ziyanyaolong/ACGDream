#pragma once

#include <QObject>

class BlockManager  : public QObject
{
	Q_OBJECT

public:
	explicit BlockManager(QObject *parent);
	virtual ~BlockManager();
};
