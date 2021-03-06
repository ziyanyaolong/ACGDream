#include "include/ACGDream.h"

ACGDream::ACGDream(QMainWindow* parent)
	: ACGDreamFrame(parent)
{
	pluginReg = new PluginReg(this);
	QDir dir(QCoreApplication::applicationDirPath() + "/Temp");
	if (!dir.exists())
		dir.mkdir(QCoreApplication::applicationDirPath() + "/Temp");

	connect(pluginReg, &PluginReg::addGuiSignal, this, [&](QWidget* gui) {
		this->addWidght(gui);
		gui->hide();
		},Qt::QueuedConnection);

	connect(pluginReg, &PluginReg::loading, this, [&](QString name) {
		QDir dir(QCoreApplication::applicationDirPath() + "/Temp/" + name);
		if (!dir.exists())
			dir.mkdir(QCoreApplication::applicationDirPath() + "/Temp/" + name);
		});

	connect(ui.actions, &QAction::triggered, this, [&]() {
		this->clearAllWidget();
		if (!pluginReg->loadAllPlugins(QDir::currentPath() + "/lib")) {
			QMessageBox::warning(this, "Error", "Could not load the plugin");
		}
		});

	QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
	if (!pluginReg->loadAllPlugins(QDir::currentPath() + "/lib")) {
		QMessageBox::warning(this, "Error", "Could not load the plugin");
	}

	show();
}

ACGDream::~ACGDream()
{
	/*if (pluginReg)
		delete pluginReg;*/
}

void ACGDream::closeEvent(QCloseEvent* e)
{
}

//bool ACGDream::loadPluginAll()
//{
//	QDir pluginsDir(QDir::currentPath() + "/lib");
//
//	QStringList filters;
//	filters.append("*.dll");
//	pluginsDir.setNameFilters(filters);
//
//	bool isLoad = false;
//
//	foreach(QString fileName, pluginsDir.entryList(QDir::Files)) 
//	{
//		QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(fileName));
//		QObject* plugin = pluginLoader.instance();
//		if (!pluginLoader.isLoaded())
//			qDebug() << __FUNCTION__ << pluginLoader.errorString();
//		if (plugin) {
//			auto pluginClass = qobject_cast<PluginCalInterface*>(plugin);
//			if (m_pInterface.indexOf(pluginClass) == -1)
//			{
//				m_pInterface.push_back(pluginClass);
//			}
//			if (pluginClass)
//			{
//				foreach (QWidget* i, pluginClass->readGuiList())
//				{
//					i->hide();
//					this->addWidght(i);
//				}
//				emit pluginClass->pluginRun(this);
//				isLoad = true;
//			}
//		}
//	}
//	
//	if (isLoad)
//	{
//		return true;
//	}else
//		return false;
//}

//bool ACGDream::loadPlugin()
//{
//	QDir pluginsDir(qApp->applicationDirPath());
//#if defined(Q_OS_WIN)
//	if (pluginsDir.dirName().toLower() == "debug" || pluginsDir.dirName().toLower() == "release")
//		pluginsDir.cdUp();
//#elif defined(Q_OS_MAC)
//	if (pluginsDir.dirName() == "MacOS") {
//		pluginsDir.cdUp();
//		pluginsDir.cdUp();
//		pluginsDir.cdUp();
//	}
//#endif
//	pluginsDir.cd("");
//	foreach(QString fileName, pluginsDir.entryList(QDir::Files)) {
//		QPluginLoader pluginLoader(pluginsDir.absoluteFilePath("G:/QT/VS/ACGDream/ACGDream/lib/SteamWorkshopTool.dll"));
//		QObject* plugin = pluginLoader.instance();
//		if (!pluginLoader.isLoaded())
//			qDebug() << __FUNCTION__ << pluginLoader.errorString();
//		if (plugin) {
//			m_pInterface = qobject_cast<PluginCalInterface*>(plugin);
//			if (m_pInterface)
//			{
//				this->addWidght(m_pInterface);
//				m_pInterface->run(0, nullptr, this);
//				return true;
//			}
//		}
//	}
//
//	return false;
//}
