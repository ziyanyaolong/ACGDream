#include "SWTGUI.h"

SWTGUI::SWTGUI(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	ui.textBrowser->insertPlainText(QString::fromUtf8("创意工坊工具启动完成\n"));
	connect(ui.pushButton, &QPushButton::clicked, this, &SWTGUI::pushButtonResponse_WebAddress);
}

SWTGUI::~SWTGUI()
{
}

void SWTGUI::pushButtonResponse_WebAddress()
{
	emit this->haveWebAddress(QString("https://steamcommunity.com/workshop/browse/?appid=" + ui.lineEdit->text()));
}