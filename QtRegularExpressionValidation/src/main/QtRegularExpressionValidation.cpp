#include "QtRegularExpressionValidation.h"

QtRegularExpressionValidation::QtRegularExpressionValidation()
	: PluginCalInterface()
{
	ui = new MainUI();
	connect(ui->ui.lineEdit, &QLineEdit::textChanged, this, &QtRegularExpressionValidation::management, Qt::QueuedConnection);
	connect(ui->ui.textEdit, &QTextEdit::textChanged, this, &QtRegularExpressionValidation::management, Qt::QueuedConnection);
	connect(this, &QtRegularExpressionValidation::sendInfo, ui, &MainUI::printText, Qt::QueuedConnection);
}

QtRegularExpressionValidation::~QtRegularExpressionValidation()
{
	ui = nullptr;
}

void QtRegularExpressionValidation::management()
{	
	QString str2 = ui->ui.textEdit->toPlainText();
	if (str2.isEmpty())
		return;
	QRegularExpression reg(ui->ui.lineEdit->text());
	QRegularExpressionMatch match = reg.match(str2);
	QString str3 = "";
	if (match.hasMatch())
	{
		str3 = match.captured(0);
	}
	emit this->sendInfo(str3);
}

void QtRegularExpressionValidation::pRun()
{
	emit this->regMainUI(ui);
}