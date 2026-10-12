#include "Globals.h"

#include <DebugMgr.h>
#include <Logger.h>

Globals gData;

Globals::Globals() : FrameCount(0)
{
	DebugMgr = new ::DebugMgr();
	Logger = new ::Logger();
	
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
	delete DebugMgr;
	DebugMgr = nullptr;

	delete Logger;
	Logger = nullptr;
}

void Globals::IncrementFrameCount()
{
	++FrameCount;
}
