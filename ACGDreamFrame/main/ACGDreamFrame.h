#pragma once

#include "../include/ACGDreamFrameH.h"
#include "../common/GUI/GUIMain/GUIMain.h"
#include "../common/Animation/Animation.h"

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

class ACGDreamFrame : public GUIMain
{
	Q_OBJECT
private:
	//Animation* animation = nullptr;

public:
	explicit ACGDreamFrame(QWidget* parent = Q_NULLPTR);
	~ACGDreamFrame();

	//GUIMain* readGUI() { return guiMain; }

};
