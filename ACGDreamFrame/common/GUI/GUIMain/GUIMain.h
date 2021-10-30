#pragma once

#include <QWidget>
#include <QPushButton>
#include <QPainter>
#include <QImage>
#include <QRect>
#include <QMap>
#include <QDebug>
#include <QVector>
#include <QGraphicsBlurEffect>
#include <QPropertyAnimation>

#include "../UIBuild/uic/ui_GUIMain.h"
#include "../../Components/Components.h"

class GUIMain : public QWidget
{
	Q_OBJECT

public:
	enum
	{
		AutoDelete,
		ManualDelete
	};
	explicit GUIMain(QWidget *parent = Q_NULLPTR);
	~GUIMain();
	QPushButton* addWidght(QWidget* widget);

private:
	Ui::GUIMain ui;
	bool beginEndValue = false;

	QImage* background;
	QMap<QObject*, QWidget*> pushButtonMap;
	QMap<QObject*, QPropertyAnimation*> propertyAnimationMap;
	QVector<QWidget*> widgetList;
	 
	virtual void paintEvent(QPaintEvent* event) override;
	virtual void resizeEvent(QResizeEvent* event) override;

signals:
	void beginEnd();

};
