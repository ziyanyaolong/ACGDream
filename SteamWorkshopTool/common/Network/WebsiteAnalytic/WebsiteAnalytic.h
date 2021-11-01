#pragma once

#include <QObject>

class WebsiteAnalytic : public QObject
{
	Q_OBJECT

public:
	WebsiteAnalytic(QObject *parent);
	~WebsiteAnalytic();
};
