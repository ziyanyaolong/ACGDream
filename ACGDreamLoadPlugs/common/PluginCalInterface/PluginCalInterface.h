#pragma once

#include <QObject>
#include <QRunnable>
#include <QThread>
#include <QWidget>
#include <QEventLoop>

class ACGDream;
class PluginCalInterface : public QRunnable, public QWidget
{
private:
	ACGDream const* _acgDream = nullptr;
	QEventLoop* eventLoop = nullptr;
	bool _openEventLoop = false;

public:
	void closeEventLoop()
	{
		if (eventLoop)
		{
			eventLoop->exit();
			delete eventLoop;
			eventLoop = nullptr;
		}
	}
	void openEventLoop()
	{
		closeEventLoop();
		eventLoop = new QEventLoop(this);
		eventLoop->exec();
	}

	PluginCalInterface(QWidget* parent = Q_NULLPTR) : QRunnable(), QWidget(parent) {}

	virtual ~PluginCalInterface()
	{ 
		closeEventLoop();
		close();
		exit(0);
	}

	int pluginRun(int argc, char* argv[], ACGDream const* acgDream) 
	{ 
		_acgDream = acgDream;
		this->run();
		if (_openEventLoop)
		{
			openEventLoop();
		}
		return 0;
	}

	ACGDream* getACGDream() {}
	bool setOpenEventLoop(bool data) { return _openEventLoop = data; }
};

#define PluginCalInterface_iid "net.ZiYanYaoLong.ACGDreamLoadPlugs.PluginCalInterface"

QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(PluginCalInterface, PluginCalInterface_iid)
QT_END_NAMESPACE