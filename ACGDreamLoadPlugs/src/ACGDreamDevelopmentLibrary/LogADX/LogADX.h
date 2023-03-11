#pragma once

#include <QObject>

class LogADX  : public QObject
{
	Q_OBJECT

public:
	LogADX(QObject *parent);
	virtual ~LogADX();
};
