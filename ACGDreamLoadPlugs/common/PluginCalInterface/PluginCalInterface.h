#pragma once

#ifdef WIN32
#pragma execution_character_set("utf-8")
#endif

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

#define RegPlugin(fileName) \
Q_INTERFACES(PluginCalInterface) \
Q_PLUGIN_METADATA(IID PluginCalInterface_iid FILE fileName)	\

#include <QObject>
#include <QThread>
#include <QThread>

class ACGDream;
class PluginCalInterface : public QObject
{
	Q_OBJECT

private:
	friend class PluginReg;
	QVector<QWidget*> guiList;
	const ACGDream* _acgDream = nullptr;
	bool separateThread = false;

protected:
	PluginCalInterface(QObject* parent = Q_NULLPTR) : QObject(parent) {}
	virtual void pRun() = 0;
	bool setSeparateThread(bool choice) { separateThread = choice; }

public:

	virtual ~PluginCalInterface()
	{ 
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

	const ACGDream* getACGDream() { return _acgDream; }
};

#define PluginCalInterface_iid "net.ZiYanYaoLong.ACGDreamLoadPlugs.PluginCalInterface"

QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(PluginCalInterface, PluginCalInterface_iid)
QT_END_NAMESPACE