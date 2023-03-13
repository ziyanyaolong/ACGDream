#pragma once

#include <QDebug>

#include "../UI/UIMain/MainUI.h"

#include <ACGDreamLoadPlugs.h>

class _declspec(dllexport) MultifunctionalModuleSorter : public PluginCalInterface
{
	Q_OBJECT
		RegPlugin("MultifunctionalModuleSorter.json")

public:
    explicit MultifunctionalModuleSorter();
    virtual ~MultifunctionalModuleSorter();

	ACGD_CREATE_MAIN_UI(createMUI, MainUI)

private:
    virtual void pRun() override;

    MainUI* ui = nullptr;
};
