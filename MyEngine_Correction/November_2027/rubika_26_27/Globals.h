#pragma once

class DebugMgr;
class SceneMgr;
class TextureMgr;
class Logger;

class Globals
{
public:
	Globals();
	~Globals();

	void Init();
	void Update(float deltaTime);
	void Shut();
	void Destroy();

	void IncrementFrameCount();

	unsigned long long FrameCount;

	DebugMgr* DebugMgr;
	Logger* Logger;
	SceneMgr* SceneMgr;
	TextureMgr* TextureMgr;
};

extern Globals gData;