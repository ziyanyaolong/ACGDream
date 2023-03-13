#include "PluginCalInterface.h"

PluginCalInterface::PluginCalInterface(QObject* parent)
	: QObject(parent)
{
	qRegisterMetaType<PluginCalInterface::pCreateMainUIPoniterFunc>("PluginCalInterface::pCreateMainUIPoniterFunc");
	qRegisterMetaType<PluginCalInterface::pCreateMainUIPoniterFunc>("PluginCalInterface::pCreateMainUIPoniterFunc&");
}

PluginCalInterface::~PluginCalInterface()
{
	if (mainUI)
	{
		emit deleteMainUI(mainUI);
	}

	mainUI = nullptr;
}
void PluginCalInterface::regMainUI(pCreateMainUIPoniterFunc func)
{
	QEventLoop e(this); 
	emit this->regMainUIS(this, func);
	connect(this, &PluginCalInterface::quitRegMainUILock, &e, &QEventLoop::quit);
	e.exec(QEventLoop::WaitForMoreEvents);
}

void PluginCalInterface::backPluginMainUI(PluginCalInterface* plugin, QWidget* widget)
{
	mainUI = widget;

	emit quitRegMainUILock();
}
