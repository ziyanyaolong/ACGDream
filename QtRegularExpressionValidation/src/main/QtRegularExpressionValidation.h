#pragma once

#include <QObject>
#include <QLineEdit>
#include <QTextBrowser>
#include <QTextEdit>
#include <QRegularExpression>
#include <QString>

#include "../UI/MainUI.h"

#include <ACGDreamLoadPlugs.h>

class _declspec(dllexport) QtRegularExpressionValidation : public PluginCalInterface
{
	Q_OBJECT
		RegPlugin("QtRegularExpressionValidation.json")

private:
	MainUI* ui;

public:
	explicit QtRegularExpressionValidation();
	~QtRegularExpressionValidation();
	virtual void pRun();

signals:
	void sendInfo(const QString&);

public slots:
	void management();

};
