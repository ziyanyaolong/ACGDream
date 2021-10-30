#include "MoveAnimation.h"

MoveAnimation::MoveAnimation(QObject *parent)
	: AnimationAPI(parent)
{
	_moveAnimation = new QPropertyAnimation(this);

	connect(_moveAnimation, &QPropertyAnimation::finished, _moveAnimation, &QPropertyAnimation::stop);
}

MoveAnimation::~MoveAnimation()
{
}

void MoveAnimation::movePoints(QObject* object, quint64 time, const QPoint& start, const QPoint& end)
{
	_moveAnimation->setPropertyName("pos");
	_moveAnimation->setDuration(time);
	_moveAnimation->setTargetObject(object);
	_moveAnimation->setStartValue(start);
	_moveAnimation->setEndValue(end);
}

void MoveAnimation::addCenterPoints(double timerShaft,const QPoint& centerPoints)
{
	_moveAnimation->setKeyValueAt(timerShaft, centerPoints);
}
