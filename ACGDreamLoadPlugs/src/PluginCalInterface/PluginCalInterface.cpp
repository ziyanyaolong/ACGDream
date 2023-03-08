#include "PluginCalInterface.h"

PluginCalInterface::~PluginCalInterface()
{
	if (mainUI)
	{
		emit deleteMainUI(mainUI);
	}

	mainUI = nullptr;
}
void PluginCalInterface::regMainUI()
{
	QEventLoop e(this); 
	emit this->regMainUIS();
	connect(this, &PluginCalInterface::quitRegMainUILock, &e, &QEventLoop::quit);
	e.exec(QEventLoop::WaitForMoreEvents);
}

void PluginCalInterface::backPluginMainUI(QWidget* widget)
{
	mainUI = widget;
	emit quitRegMainUILock();
}
