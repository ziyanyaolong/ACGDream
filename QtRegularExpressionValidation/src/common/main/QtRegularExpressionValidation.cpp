#include "QtRegularExpressionValidation.h"

QtRegularExpressionValidation::QtRegularExpressionValidation()
	: PluginCalInterface()
{
	ui = new GUI();
	addGui(ui);
	connect(ui->ui.lineEdit, &QLineEdit::textChanged, this, &QtRegularExpressionValidation::management, Qt::QueuedConnection);
	connect(ui->ui.textEdit, &QTextEdit::textChanged, this, &QtRegularExpressionValidation::management, Qt::QueuedConnection);
	connect(this, &QtRegularExpressionValidation::sendInfo, ui, &GUI::printText, Qt::QueuedConnection);
}

QtRegularExpressionValidation::~QtRegularExpressionValidation()
{
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
	/*for (int pos = 0; pos = regExp.indexIn(str2, pos), pos >= 0; pos += regExp.matchedLength())
	{
		if ((error <= 5000) && (str3 == ""))
		{
			qDebug() << str3;
		}
		else if ((error > 10000 ) && (str3 == ""))
			break;
		str3 += regExp.cap(0);
		error++;
	}*/
	emit this->sendInfo(str3);
}

void QtRegularExpressionValidation::pRun()
{
}