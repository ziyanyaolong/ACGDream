#pragma once

#include <QWidget>
#include <QMap>

#include "API/AnimationAPI.h"
#include "MoveAnimation/MoveAnimation.h"

class Animation : public QObject
{
	Q_OBJECT

private:
	QMap<QObject*, QObject*> animationMap;
	MoveAnimation* moveAnimation = nullptr;

	QObject* map1(QObject* key, QObject* value);

public:
	enum Mode
	{
		Move,
		Rotate
	};

	explicit Animation(QObject* parent = Q_NULLPTR);
	virtual ~Animation();

	QObject* addWidgetAnimation(QObject* widget, Animation::Mode animationMode);

public slots:
	void play();
	void play(QObject* const object);
};
