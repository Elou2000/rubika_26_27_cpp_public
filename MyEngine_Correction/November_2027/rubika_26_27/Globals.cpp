#include "Globals.h"

#include <DebugMgr.h>
#include <Logger.h>
#include <SceneMgr.h>
#include <TextureMgr.h>

Globals gData;

Globals::Globals() : FrameCount(0)
{
	DebugMgr = new ::DebugMgr();
	Logger = new ::Logger();
	SceneMgr = new ::SceneMgr();
	TextureMgr = new ::TextureMgr();
}

Globals::~Globals()
{
	Destroy();
}

void Globals::Init()
{
	Logger->Init();
	SceneMgr->Init();
	TextureMgr->Init();
}

void Globals::Update(float deltaTime)
{
	SceneMgr->Update(deltaTime);
}

void Globals::Shut()
{
	Logger->Shut();
	SceneMgr->Shut();
	TextureMgr->Shut();
}

void Globals::Destroy()
{
	delete DebugMgr;
	DebugMgr = nullptr;

	delete Logger;
	Logger = nullptr;

	delete SceneMgr;
	SceneMgr = nullptr;

	delete TextureMgr;
	TextureMgr = nullptr;
}

void Globals::IncrementFrameCount()
{
	++FrameCount;
}
