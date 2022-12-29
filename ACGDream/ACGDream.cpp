#include "include/ACGDream.h"

ACGDream::ACGDream(QMainWindow* parent)
	: ACGDreamFrame(parent)
{
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

	this->initDirs();


	pluginThread = new QThread(this);
	pluginReg = new PluginReg(nullptr);
	
	pluginReg->moveToThread(pluginThread);
	if (!pluginThread->isRunning())
	{
		pluginThread->start();
	}

	setBackground(QImage(QCoreApplication::applicationDirPath() + "/BG.png"));

	connect(pluginReg, &PluginReg::removeUISignal, this, [&](QWidget* widget) {
		this->removeWidght(widget);
		}, Qt::QueuedConnection);

	connect(pluginReg, &PluginReg::loading, this, [&](QString name) {
		this->initDirs(name);
		});

	connect(pluginReg, &PluginReg::loadError, this, [&](const QString& name, const QString& info) {
		QMessageBox::critical(this, name, info);
		});

	connect(pluginReg, &PluginReg::regPluginMainUI, this, [&](PluginCalInterface* plugin) {
		QWidget* pTempWidget = plugin->createMainUI();
		if (this->addWidght(pTempWidget) == nullptr)
		{
			QMetaObject::invokeMethod(pluginReg,
				"backPluginMainUI",
				Qt::QueuedConnection,
				Q_ARG(PluginCalInterface*, plugin),
				Q_ARG(QWidget*, nullptr));
		}
		else
		{
			QMetaObject::invokeMethod(pluginReg,
				"backPluginMainUI",
				Qt::QueuedConnection,
				Q_ARG(PluginCalInterface*, plugin),
				Q_ARG(QWidget*, pTempWidget));
		}
		});

	connect(ui.actions, &QAction::triggered, this, [&]() {
		auto actionsSet = ((QAction*)(sender()));
		if (!actionsSet->isEnabled())
		{
			return;
		}

		actionsSet->setEnabled(false);
		this->clearAllWidget();

		QMetaObject::invokeMethod(pluginReg,
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
	if (pluginReg)
	{
		pluginReg->deleteLater();
	}

	pluginReg = nullptr;

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