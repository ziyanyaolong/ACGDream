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

    virtual QWidget* createMainUI() override { return new MainUI(); }

private:
    virtual void pRun() override;

    MainUI* ui = nullptr;
};
