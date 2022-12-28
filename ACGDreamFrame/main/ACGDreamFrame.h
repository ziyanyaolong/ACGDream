#pragma once

#include "../include/ACGDreamFrameH.h"
#include "../src/UI/UIMain/UIMain.h"
#include "../src/Animation/Animation.h"

//__declspec(dllexport) class ACGDreamFrame : public QObject
//{
//	Q_OBJECT
//private:
//	Animation* animation = nullptr;
//	GUIMain* guiMain = nullptr;
//
//public:
//	explicit ACGDreamFrame(QObject* parent = Q_NULLPTR);
//	~ACGDreamFrame();
//};

class ACGDreamFrame : public UIMain
{
	Q_OBJECT
private:
	//Animation* animation = nullptr;

public:
	explicit ACGDreamFrame(QMainWindow* parent = Q_NULLPTR);
	virtual ~ACGDreamFrame();

	//UIMain* readUI() { return guiMain; }

};
