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
#include <QEventLoop>
#include <QPluginLoader>

class ACGDream;
class PluginCalInterface : public QObject
{
	Q_OBJECT

private:
	friend class PluginReg;

	const ACGDream* _acgDream = nullptr;
	bool separateThread = false;
	QWidget* mainUI = nullptr;
	QPluginLoader* pluginLoaderObject = nullptr;

	inline void setPluginLoader(QPluginLoader* pluginLoader) { pluginLoaderObject = pluginLoader; }
	inline QPluginLoader* getPluginLoader() { return pluginLoaderObject; }

protected slots:
	virtual void pRun() = 0;

protected:
	PluginCalInterface(QObject* parent = Q_NULLPTR) : QObject(parent) {}

	inline bool setSeparateThread(bool choice) { separateThread = choice; }
	inline QWidget* getMainUI() { return mainUI; }
	inline void regMainUI() { QEventLoop e(this); emit this->regMainUIS(); connect(this, &PluginCalInterface::quitRegMainUILock, &e, &QEventLoop::quit); e.exec(QEventLoop::WaitForMoreEvents); }
	
public:
	virtual ~PluginCalInterface()
	{
		if (mainUI)
		{
			emit deleteMainUI(mainUI);
			mainUI = nullptr;
		}
	}

	inline const ACGDream* getACGDream() { return _acgDream; }


	virtual QWidget* createMainUI() { return nullptr; }

	typedef QWidget* (*createMainUIPoniter)();

signals:
	void regMainUIS();
	void quitRegMainUILock();
	void deleteMainUI(QWidget* widget);

public slots:
	void backPluginMainUI(QWidget* widget) { mainUI = widget; emit quitRegMainUILock(); }

};

#define PluginCalInterface_iid "net.ZiYanYaoLong.ACGDreamLoadPlugs.PluginCalInterface"

QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(PluginCalInterface, PluginCalInterface_iid)
QT_END_NAMESPACE