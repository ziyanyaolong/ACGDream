#pragma once

#ifdef WIN32
#pragma execution_character_set("utf-8")
#elif _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

#define RegPlugin(fileName) \
Q_INTERFACES(PluginCalInterface) \
Q_PLUGIN_METADATA(IID PluginCalInterface_iid FILE fileName)	\

#include <QObject>
#include <QThread>
#include <QWidget>

class ACGDream;
class PluginCalInterface : public QObject
{
	Q_OBJECT

private:
	friend class PluginReg;
	const ACGDream* _acgDream = nullptr;
	bool separateThread = false;

protected slots:
	virtual void pRun() = 0;

protected:
	PluginCalInterface(QObject* parent = Q_NULLPTR) : QObject(parent) {}
	bool setSeparateThread(bool choice) { separateThread = choice; }

public:

	virtual ~PluginCalInterface()
	{ 
	}

	const ACGDream* getACGDream() { return _acgDream; }

signals:
	void addGui(QWidget* gui);

};

#define PluginCalInterface_iid "net.ZiYanYaoLong.ACGDreamLoadPlugs.PluginCalInterface"

QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(PluginCalInterface, PluginCalInterface_iid)
QT_END_NAMESPACE