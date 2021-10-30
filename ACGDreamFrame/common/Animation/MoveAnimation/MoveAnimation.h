#pragma once

#include <QObject>
#include <QPoint>
#include <QPropertyAnimation>

#include "../API/AnimationAPI.h"

class MoveAnimation : public AnimationAPI
{
	Q_OBJECT

private:
	QObject* _object = nullptr;
	QPropertyAnimation* _moveAnimation = nullptr;

public:
	explicit MoveAnimation(QObject* parent = Q_NULLPTR);
	virtual ~MoveAnimation();

	void movePoints(QObject* object, quint64 time, const QPoint& start, const QPoint& end);
	void addCenterPoints(double timerShaft, const QPoint& centerPoints);

	template <typename ... T>
	void autoAddCenterPoints(std::size_t centerPointSize, const QPoint& centerPoints, const T&... moreCenterPoints)
	{
		std::vector<QPoint> tempPointList;
		tempPointList.push_back(moreCenterPoints...);
		double increase = (1 / centerPointSize);
		double nowValue = 0.0f;
		for (std::size_t i = 0; i < tempPointList.size(); i++)
		{
			addCenterPoints((nowValue += increase), tempPointList[i]);
		}
	}

	template <typename ... T>
	void movePoints(QObject* object, quint64 time, const QPoint& start, const QPoint& end, const T&... centerPoints)
	{
		double i = 0.0f;
		std::size_t centerPointSize = sizeof...(centerPoints);
		movePoints(object, time, start, end);
		autoAddCenterPoints(centerPointSize, centerPoints...);
	}

	virtual void start() { _moveAnimation->start(); }
};
