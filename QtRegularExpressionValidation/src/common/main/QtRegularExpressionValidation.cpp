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
	qDebug() << ui->ui.lineEdit->text();
	QRegExp regExp(ui->ui.lineEdit->text());
	QString str2 = ui->ui.textEdit->toPlainText();
	QString str3 = "";
	uint64_t error = 0;
	for (int pos = 0; pos = regExp.indexIn(str2, pos), pos >= 0; pos += regExp.matchedLength())
	{
		if ((error <= 5000) && (str3 == ""))
		{
			qDebug() << str3;
		}
		else if ((error > 10000 ) && (str3 == ""))
			break;
		str3 += regExp.cap(0);
		error++;
	}
	emit this->sendInfo(str3);
}

void QtRegularExpressionValidation::pRun()
{
}