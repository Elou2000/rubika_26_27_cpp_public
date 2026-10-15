#pragma once
#include <string>
#include "Entity.h"
#include <vector>

namespace sf
{
	class RenderWindow;
}

class Scene
{
public:

	Scene(const std::string& name);
	~Scene();

	virtual void Start();
	virtual void Update(float fDeltaTime);
	virtual void Destroy();

	void AddEntityToPool(Entity*);
	void RemoveEntityFromPool(Entity*);

	std::vector<Entity*> entityList;

	std::string sceneName;

	void Draw(sf::RenderWindow& window) const;
	
};
	