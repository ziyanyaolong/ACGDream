#pragma once

#include <QPushButton>
#include <QPainter>

class PushButtonStyle1 : public QPushButton
{
private:
	virtual void paintEvent(QPaintEvent* event) override;

	unsigned int cohesionSize = 1;

public:
	explicit PushButtonStyle1(QWidget* parent = Q_NULLPTR);
	virtual ~PushButtonStyle1();

};

