#pragma once
#include "TransformComponent.h"
#include "RenderComponent.h"
#include <string>

#define ENTITY_TAG_NONE			0
#define ENTITY_TAG_STATIC		1 << 0
#define ENTITY_TAG_DYNAMIC		1 << 1
#define ENTITY_TAG_NEUTRAL		1 << 2
#define ENTITY_TAG_ENEMY		1 << 3
#define ENTITY_TAG_PLAYER		1 << 4

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

	virtual void AddTag(unsigned int tag);
	virtual void RemoveTag(unsigned int tag);
	virtual bool HasTag(unsigned int tag) const;

	unsigned int GetTags() const;

	std::string entityName;
	std::vector<IComponent*> components;

	template <typename C>
	C* AddComponent();

	template <typename C>
	C* GetComponent() const;

	void Draw(sf::RenderWindow& window) const;

private:
	unsigned int m_Tags = ENTITY_TAG_NONE;
	
};

#include "Entity.hxx"


