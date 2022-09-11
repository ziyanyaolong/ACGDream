#pragma once

#include <QObject>
#include <QLineEdit>
#include <QTextBrowser>
#include <QTextEdit>
#include <QRegularExpression>
#include <QString>

#include "../GUI/GUI.h"

#include <ACGDreamLoadPlugs.h>

class _declspec(dllexport) QtRegularExpressionValidation : public PluginCalInterface
{
	Q_OBJECT
		RegPlugin("QtRegularExpressionValidation.json")

private:
	GUI* ui;

public:
	explicit QtRegularExpressionValidation();
	~QtRegularExpressionValidation();
	virtual void pRun();

signals:
	void sendInfo(const QString&);

public slots:
	void management();

};
