#pragma once

#ifdef WIN32
#pragma execution_character_set("utf-8")
#elif _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

#define ACGDRegPlugin(fileName) \
Q_INTERFACES(PluginCalInterface) \
Q_PLUGIN_METADATA(IID PluginCalInterface_iid FILE fileName)	\

#include <QObject>
#include <QThread>
#include <QWidget>
#include <QList>
#include <QEventLoop>
#include <QPluginLoader>

#include "../PluginData/PluginMetaData/PluginMetaData.h"

#define ACGD_CREATE_MAIN_UI(funcName, UI_Type, ...)	\
static QWidget* funcName()							\
{													\
	return new UI_Type(__VA_ARGS__);				\
}													\

class ACGDream;
class PluginCalInterface : public QObject
{
	Q_OBJECT

public:
	virtual ~PluginCalInterface();

	inline const ACGDream* getACGDream() { return _acgDream; }


	virtual QWidget* createMainUI() { return nullptr; }

	typedef QWidget* (*pCreateMainUIPoniterFunc)();

protected:
	virtual void pRun() = 0;

	PluginCalInterface(QObject* parent = Q_NULLPTR);

	inline QWidget* getMainUI() { return mainUI; }

	inline bool setSeparateThread(bool choice) { separateThread = choice; }

	void regMainUI(pCreateMainUIPoniterFunc func);

private:
	friend class PluginReg;
	friend class PluginCore;

	typedef QMap<QString, bool> ModuleAttachedList;

	const ACGDream* _acgDream = nullptr;

	bool separateThread = false;

	QWidget* mainUI = nullptr;
	
	const PluginMetaData* pluginMetaData = nullptr;

	inline void setPluginLoader(QPluginLoader* pluginLoader) { pluginLoader = pluginLoader; }
	inline const PluginMetaData* getPluginMetaData() { return pluginMetaData; }

signals:
	void regMainUIS(PluginCalInterface* plugin, pCreateMainUIPoniterFunc func);
	void quitRegMainUILock();
	void removeMainUI(QWidget* widget);

public slots:
	void backPluginMainUI(PluginCalInterface* plugin, QWidget* widget);

};

#define PluginCalInterface_iid "net.ZiYanYaoLong.ACGDreamLoadPlugs.PluginCalInterface"

QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(PluginCalInterface, PluginCalInterface_iid)
QT_END_NAMESPACE