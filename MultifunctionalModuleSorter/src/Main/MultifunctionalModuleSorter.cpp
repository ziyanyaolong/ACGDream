#include "MultifunctionalModuleSorter.h"

MultifunctionalModuleSorter::MultifunctionalModuleSorter()
    : PluginCalInterface()
{
    ui = new MainUI();
}

void MultifunctionalModuleSorter::pRun()
{
    emit this->regMainUI(ui);
}

MultifunctionalModuleSorter::~MultifunctionalModuleSorter()
{
	ui = nullptr;
}
