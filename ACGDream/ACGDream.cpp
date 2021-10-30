#include "include/ACGDream.h"

ACGDream::ACGDream(QWidget *parent)
	: ACGDreamFrame(parent)
{
	if (!loadPlugin()) {
		QMessageBox::warning(this, "Error", "Could not load the plugin");
	}
	show();
}

ACGDream::~ACGDream()
{
}

bool ACGDream::loadPlugin()
{
	QDir pluginsDir(qApp->applicationDirPath());
#if defined(Q_OS_WIN)
	if (pluginsDir.dirName().toLower() == "debug" || pluginsDir.dirName().toLower() == "release")
		pluginsDir.cdUp();
#elif defined(Q_OS_MAC)
	if (pluginsDir.dirName() == "MacOS") {
		pluginsDir.cdUp();
		pluginsDir.cdUp();
		pluginsDir.cdUp();
	}
#endif
	pluginsDir.cd("");
	foreach(QString fileName, pluginsDir.entryList(QDir::Files)) {
		QPluginLoader pluginLoader(pluginsDir.absoluteFilePath("G:/QT/VS/ACGDream/ACGDream/lib/SteamWorkshopTool.dll"));
		QObject* plugin = pluginLoader.instance();
		if (!pluginLoader.isLoaded())
			qDebug() << __FUNCTION__ << pluginLoader.errorString();
		if (plugin) {
			m_pInterface = qobject_cast<PluginCalInterface*>(plugin);
			if (m_pInterface)
			{
				m_pInterface->run(0, nullptr, this);
				return true;
			}
		}
	}

	return false;
}
