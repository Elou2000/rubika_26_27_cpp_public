#pragma once

#include <string>
#include <vector>
#include <functional>
#include "Scene.h"
#include "DebugMgr.h"

class SceneMgr : IDebugable
{
public:
	enum mode {Navigate, InspectScene, InspectEntity};

	SceneMgr();
	~SceneMgr();
	std::vector<Scene*> sceneList;
	Scene* currentScene;
	int currentCursorPos = 0;

	mode currentMode = Navigate;
	Scene* currentInspectedScene = nullptr;
	Entity* currentInspectedEntity = nullptr;

	virtual void Start();
	virtual void Update(float fDeltaTime);
	virtual void Draw(sf::RenderWindow& window);
	virtual void Destroy();
	void AddNewScene(Scene*);
	void SelectScene(std::string);

	virtual void DrawDebug() override;
};





