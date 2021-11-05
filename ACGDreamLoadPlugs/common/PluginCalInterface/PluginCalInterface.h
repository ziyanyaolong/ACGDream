#pragma once

#ifdef WIN32
#pragma execution_character_set("utf-8")
#endif

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

#include <QObject>
#include <QWidget>
#include <QThread>
#include <QRunnable>
#include <QThread>
#include <QEventLoop>

#define RegPlugin(fileName) \
Q_INTERFACES(PluginCalInterface) \
Q_PLUGIN_METADATA(IID PluginCalInterface_iid FILE fileName)	\

class ACGDream;
class PluginCalInterface : virtual public QWidget, virtual public QRunnable
{
private:
	QVector<QWidget*> guiList;
	ACGDream const* _acgDream = nullptr;
	QEventLoop* eventLoop = nullptr;
	bool _openEventLoop = false;

protected:
	PluginCalInterface(QWidget* parent = Q_NULLPTR) : QRunnable(), QWidget(parent) {}

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

	virtual ~PluginCalInterface()
	{ 
		closeEventLoop();
		exit(0);
	}

	int pluginRun(int argc, char* argv[], ACGDream const* acgDream) 
	{ 
		_acgDream = acgDream;
		this->run();
		if (_openEventLoop)
		{
			this->openEventLoop();
		}
		return 0;
	}

	bool addGui(QWidget* gui) 
	{ 
		if (guiList.indexOf(gui) == -1) 
		{
			guiList.push_back(gui);
			return true;
		}else
			return false;
	}

	QVector<QWidget*>& readGuiList() { return guiList; }

	ACGDream* getACGDream() {}
	bool setOpenEventLoop(bool data) { return _openEventLoop = data; }
};

#define PluginCalInterface_iid "net.ZiYanYaoLong.ACGDreamLoadPlugs.PluginCalInterface"

QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(PluginCalInterface, PluginCalInterface_iid)
QT_END_NAMESPACE