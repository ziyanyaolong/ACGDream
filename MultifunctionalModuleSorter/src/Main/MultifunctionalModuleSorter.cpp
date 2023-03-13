#include "MultifunctionalModuleSorter.h"

MultifunctionalModuleSorter::MultifunctionalModuleSorter()
    : PluginCalInterface()
{
}

void MultifunctionalModuleSorter::pRun()
{
	emit this->regMainUI(&createMUI);

	this->ui = static_cast<MainUI*>(this->getMainUI());
}

MultifunctionalModuleSorter::~MultifunctionalModuleSorter()
{
	ui = nullptr;
}
