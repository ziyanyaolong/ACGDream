#pragma once

#include <QObject>

class AnimationAPI : public QObject
{
public:
	explicit AnimationAPI(QObject* parent = Q_NULLPTR);
	virtual ~AnimationAPI();
	virtual void start() = 0;
};

