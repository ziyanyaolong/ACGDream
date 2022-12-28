#pragma once

#include <QDebug>

#include "../UI/MainUI.h"

#include <ACGDreamLoadPlugs.h>

class _declspec(dllexport) MultifunctionalModuleSorter : public PluginCalInterface
{
	Q_OBJECT
		RegPlugin("MultifunctionalModuleSorter.json")

public:
    MultifunctionalModuleSorter();
    virtual ~MultifunctionalModuleSorter();

private:
    virtual void pRun() override;

    MainUI* ui = nullptr;
};
