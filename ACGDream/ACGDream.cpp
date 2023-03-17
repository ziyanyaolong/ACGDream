#include "include/ACGDream.h"

ACGDream::ACGDream(QMainWindow* parent)
	: ACGDreamFrame(parent)
{
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

	this->initDirs();

	pluginThread = new QThread(this);
	pluginCore = new PluginCore(nullptr);
	
	pluginCore->moveToThread(pluginThread);
	if (!pluginThread->isRunning())
	{
		pluginThread->start();
	}

	setBackground(QImage(QCoreApplication::applicationDirPath() + "/BG.png"));

	//插件信号处理和传递
	connect(pluginCore, &PluginCore::removeUISignal, this, [&](QWidget* widget) {
		this->removeWidght(widget);
		});

	connect(pluginCore, &PluginCore::loaded, this, [&](const QString& name, PluginMetaData* pluginMetaData) {
		this->initDirs(name);
		});

	connect(pluginCore, &PluginCore::unloaded, this, [&](const QString& name, PluginMetaData* pluginMetaData) {
		this->initDirs(name);
		});

	connect(pluginCore, &PluginCore::regUISignal, this, [&](PluginCalInterface* plugin, PluginCalInterface::pCreateMainUIPoniterFunc func) {
		QWidget* pTempWidget = func();

		if (this->addWidght(pTempWidget) == nullptr)
		{
			QMetaObject::invokeMethod(pluginCore,
				"backPluginMainUI",
				Qt::QueuedConnection,
				Q_ARG(PluginCalInterface*, plugin),
				Q_ARG(QWidget*, nullptr));
		}
		else
		{
			QMetaObject::invokeMethod(pluginCore,
				"backPluginMainUI",
				Qt::QueuedConnection,
				Q_ARG(PluginCalInterface*, plugin),
				Q_ARG(QWidget*, pTempWidget));
		}
		});

	//connect(pluginReg, &PluginCore::loadError, this, [&](const QString& name, const QString& info) {
	//	QMessageBox::critical(this, name, info);
	//	});

	//connect(pluginReg, &PluginCore::backPluginMainUI, this, [&](PluginCalInterface* plugin, QWidget* mainWidget) {
	//	QWidget* pTempWidget = plugin->createMainUI();
	//	if (this->addWidght(pTempWidget) == nullptr)
	//	{
	//		QMetaObject::invokeMethod(pluginReg,
	//			"backPluginMainUI",
	//			Qt::QueuedConnection,
	//			Q_ARG(PluginCalInterface*, plugin),
	//			Q_ARG(QWidget*, nullptr));
	//	}
	//	else
	//	{
	//		QMetaObject::invokeMethod(pluginReg,
	//			"backPluginMainUI",
	//			Qt::QueuedConnection,
	//			Q_ARG(PluginCalInterface*, plugin),
	//			Q_ARG(QWidget*, pTempWidget));
	//	}
	//	});

	//界面按钮等处理和传递
	connect(ui.actions, &QAction::triggered, this, [&]() {
		auto actionsSet = ((QAction*)(sender()));
		if (!actionsSet->isEnabled())
		{
			return;
		}

		actionsSet->setEnabled(false);
		this->clearAllWidget();

		QMetaObject::invokeMethod(pluginCore,
			"loadAllPlugins",
			Qt::QueuedConnection,
			Q_ARG(const QString&, QCoreApplication::applicationDirPath() + "/Extra"));

		actionsSet->setEnabled(true);
		});

	emit this->ui.actions->triggered();

	show();
}

ACGDream::~ACGDream()
{
	//加载模块退出
	if (pluginCore)
	{
		pluginCore->deleteLater();
	}

	pluginCore = nullptr;

	if (pluginThread)
	{
		pluginThread->quit();
		pluginThread->wait();
		pluginThread->deleteLater();
	}

	pluginThread = nullptr;
}

void ACGDream::initDirs(QString name)
{
	//创建初始文件夹
	QDir dir(QCoreApplication::applicationDirPath() + "/Temp");
	if (!dir.exists())
	{
		dir.mkdir(QCoreApplication::applicationDirPath() + "/Temp");
	}

	dir.setPath(QCoreApplication::applicationDirPath() + "/Config");
	if (!dir.exists())
	{
		dir.mkdir(QCoreApplication::applicationDirPath() + "/Config");
	}

	if (name == "")
	{
		return;
	}

	dir.setPath(QCoreApplication::applicationDirPath() + "/Temp/" + name);
	if (!dir.exists())
	{
		dir.mkdir(QCoreApplication::applicationDirPath() + "/Temp/" + name);
	}

	dir.setPath(QCoreApplication::applicationDirPath() + "/Config/" + name);
	if (!dir.exists())
	{
		dir.mkdir(QCoreApplication::applicationDirPath() + "/Config/" + name);
	}

	dir.setPath(QCoreApplication::applicationDirPath() + "/Extra");
	if (!dir.exists())
	{
		dir.mkdir(QCoreApplication::applicationDirPath() + "/Extra");
	}
}

void ACGDream::closeEvent(QCloseEvent* e)
{
}