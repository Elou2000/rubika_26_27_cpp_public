#pragma once
#include "TransformComponent.h"
#include "RenderComponent.h"
#include <string>

class RenderComponent;
namespace sf
{
	class RenderWindow;
}

class Entity
{
public:
	Entity(std::string friendlyName = "");
	virtual ~Entity() ;

	virtual void Start() ;
	virtual void Update(float fDeltaTime);
	virtual void Destroy();

	std::string entityName;
	TransformComponent* tc;
	RenderComponent* rc;
	/*
	 * This function will be removed later, when the rendering system will be more robust. But for now, we need it here.
	 * Function that will be called each frame to draw the entity
	*/
	void Draw(sf::RenderWindow& window) const;
};
