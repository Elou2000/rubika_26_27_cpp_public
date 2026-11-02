#pragma once

class DebugMgr;
class Logger;
class SceneMgr;
class TextureMgr;
class InputMgr;

class Globals
{
public:
	Globals();
	~Globals();

	void Init();
	void Shut();
	void Destroy();

	void IncrementFrameCount();

	unsigned long long FrameCount;

	DebugMgr* DebugMgr;
	Logger* Logger;
	SceneMgr* SceneMgr;
	TextureMgr* TextureMgr;
	InputMgr* InputMgr;
};

extern Globals gData;