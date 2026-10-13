#pragma once

#include <string>
#include <vector>
#include <functional>
#include "Scene.h"
#include "DebugMgr.h"

class SceneMgr : IDebugable
{
public:
	enum mode {Navigate, Inspect};

	SceneMgr();
	~SceneMgr();
	std::vector<Scene*> sceneList;
	Scene* currentScene;
	int currentCursorPos = 0;

	mode currentMode = Navigate;
	Scene* currentInspectedScene;

	virtual void Start();
	virtual void Update(float fDeltaTime);
	virtual void Destroy();

	void AddNewScene(Scene*);
	void SelectScene(std::string);

	virtual void DrawDebug() override;
};





