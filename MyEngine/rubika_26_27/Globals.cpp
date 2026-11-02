#include "Globals.h"

#include <DebugMgr.h>
#include <Logger.h>
#include <SceneMgr.h>
#include <TextureMgr.h>
#include <InputMgr.h>

Globals gData;

Globals::Globals() : FrameCount(0)
{
	DebugMgr = new ::DebugMgr();
	Logger = new::Logger();
	SceneMgr = new ::SceneMgr();
	TextureMgr = new ::TextureMgr();
	InputMgr = new ::InputMgr();
	
}

Globals::~Globals()
{
	Destroy();
}

void Globals::Init()
{
	Logger->Init();
}

void Globals::Shut()
{
	Logger->Shut();
}

void Globals::Destroy()
{
	delete Logger;
	Logger = nullptr;

	delete SceneMgr;
	SceneMgr = nullptr;

	delete TextureMgr;
	TextureMgr = nullptr;

	delete InputMgr;
	InputMgr = nullptr;

	delete DebugMgr;
	DebugMgr = nullptr;

	
}

void Globals::IncrementFrameCount()
{
	++FrameCount;
}
