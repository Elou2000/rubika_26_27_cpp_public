#pragma once
#include <string>
#include "Entity.h"
#include <vector>
#include "DynamicColliderComponent.h"
#include "GridEntityCache.h"


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
	void ManageCollisions();
	void ResolveAgainst(Entity* mover, DynamicColliderComponent* moverColl, const std::vector<Entity*>& candidates);

	void SetupGrid(const sf::FloatRect& bounds, const sf::Vector2f& cellSize);

	std::vector<Entity*> entityList;
	//std::vector<Entity*> staticEntityList;
	GridEntityCache dynamicGrid;
	GridEntityCache staticGrid;
	bool debugGrid = false;

	std::string sceneName;

	void Draw(sf::RenderWindow& window) const;
	
};
	