#include "Animation.h"

Animation::Animation(QObject*parent)
	: QObject(parent)
{
	moveAnimation = new MoveAnimation(this);
	moveAnimation->movePoints(this, 1000, QPoint(0,0), QPoint(0, 0), QPoint(0, 0), QPoint(0, 0));
}

Animation::~Animation()
{
	if (moveAnimation)
		delete moveAnimation;
}

QObject* Animation::map1(QObject* key, QObject* value)
{
	if (animationMap.count(key) > 0)
	{
		auto tempValue = animationMap[key];
		if (tempValue != nullptr)
		{
			delete tempValue;
		}
	}
	animationMap[key] = value;
	return value;
}

QObject* Animation::addWidgetAnimation(QObject* const widget, Animation::Mode animationMode)
{
	switch (animationMode)
	{
	case Animation::Mode::Move:		
		return map1(widget, new MoveAnimation(this));
		break;

	case Animation::Mode::Rotate:
		return nullptr;
		break;

	default:
		return nullptr;
		break;
	}
}

void Animation::play()
{
	QObject* object = sender();
	if (animationMap.count(object) > 0)
	{
		static_cast<AnimationAPI*>(animationMap[object])->start();
	}
}

void Animation::play(QObject* const object)
{
	if (animationMap.count(object) > 0)
	{
		static_cast<AnimationAPI*>(animationMap[object])->start();
	}
}