#pragma once

#include <QMainWindow>
#include <QPushButton>
#include <QPainter>
#include <QImage>
#include <QRect>
#include <QMap>
#include <QDebug>
#include <QVector>
#include <QGraphicsBlurEffect>
#include <QPropertyAnimation>
#include <QSharedPointer>
#include <QMessageBox>

#include "../UIBuild/uic/ui_GUIMain.h"
#include "../Components/Components.h"

class GUIMain : public QMainWindow
{
	Q_OBJECT

public:
	enum
	{
		AutoDelete,
		ManualDelete
	};
	explicit GUIMain(QMainWindow*parent = Q_NULLPTR);
	virtual ~GUIMain();
	QPushButton* addWidght(QWidget* widget);
	void clearAllWidget();
	Ui_MainWindow* readUI() { return &ui; }

protected:
	Ui_MainWindow ui;

private:
	bool beginEndValue = false;

	QSharedPointer<QImage> background;
	QMap<QPushButton*, QWidget*> pushButtonMap;
	QMap<QObject*, QPropertyAnimation*> propertyAnimationMap;
	QVector<QWidget*> widgetList;
	 
	virtual void paintEvent(QPaintEvent* event) override;
	virtual void resizeEvent(QResizeEvent* event) override;

signals:
	void beginEnd();

};
