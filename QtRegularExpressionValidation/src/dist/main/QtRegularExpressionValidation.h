#pragma once

#include <QObject>
#include <QLineEdit>
#include <QTextBrowser>
#include <QTextEdit>
#include <QRegularExpression>
#include <QString>

#include "../GUI/GUI.h"

#include "../../../../ACGDreamLoadPlugs/include/ACGDreamLoadPlugs.h"
#pragma comment(lib,"../.build/vs/ACGDreamLoadPlugs/lib/ACGDreamLoadPlugs.lib")

class Q_WIDGETS_EXPORT QtRegularExpressionValidation : public PluginCalInterface
{
	Q_OBJECT
		RegPlugin("QtRegularExpressionValidation.json")

private:
	GUI* ui;

public:
	QtRegularExpressionValidation();
	~QtRegularExpressionValidation();
	virtual void pRun();

signals:
	void sendInfo(const QString&);

public slots:
	void management();

};
