#pragma once

#include <QScrollArea>
#include <QPainter>
#include <QDebug>
#include <QEvent>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QTimer>
#include <QPropertyAnimation>
#include <QPushButton>
#include <QGraphicsOpacityEffect>
#include <QScrollBar>
#include <QDir>
#include <QFile>

class ScrollAreaStyle1 : public QScrollArea
{
	Q_OBJECT

private:
	virtual bool eventFilter(QObject* target, QEvent* event) override;
	QTimer* timer = nullptr;
	QPropertyAnimation* fadeIn = nullptr;
	QPropertyAnimation* fadeOut = nullptr;
	QGraphicsOpacityEffect* graphicsOpacityEffect = nullptr;
	unsigned long long lifetime = 2000;

public:
	explicit ScrollAreaStyle1(QWidget* parent = Q_NULLPTR);
	virtual ~ScrollAreaStyle1();

public slots:
	void closeScrollBar();
	void openScrollBar();
};

