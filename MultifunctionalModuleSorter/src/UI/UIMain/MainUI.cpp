#include "MainUI.h"

MainUI::MainUI(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	b.setParent(ui.M_DesignWidget);
}

MainUI::~MainUI()
{
}
