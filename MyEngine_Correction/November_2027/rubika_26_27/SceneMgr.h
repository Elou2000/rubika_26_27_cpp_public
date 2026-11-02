#pragma once

#include <DebugMgr.h>

#include <unordered_map>

class Scene;

namespace sf
{
	class RenderWindow;
}

class SceneMgr : public IDebugable
{
public:

	void Init();
	void Start();
	void Update(float deltaTime);
	void Shut();

	/*
	 * This function will be removed later, when the rendering system will be more robust. But for now, we need it here.
	 * Function that will be called each frame to draw the entity
	*/
	void Draw(sf::RenderWindow& window) const;

	Scene* CreateScene(const std::string& sceneName);
	void DeleteScene(Scene* s);

	Scene* SetActiveScene(const std::string& sceneName);

	virtual void DrawDebug() override;

protected:

	std::unordered_map<std::size_t, Scene*> Scenes;
	Scene* ActiveScene = nullptr;
};