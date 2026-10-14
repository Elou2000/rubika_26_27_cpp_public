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
	std::vector<IComponent*> components;
	//TransformComponent* tc;
	//RenderComponent* rc;

	template <typename C>
	C* AddComponent();

	template <typename C>
	C* GetComponent() const;


	void Draw(sf::RenderWindow& window) const;

};

#include "Entity.hxx"


