#include "Entity.h"
#include "Logger.h"
#include "DynamicColliderComponent.h"



Entity::Entity(std::string friendlyName)
{
	entityName = friendlyName;
}

Entity::~Entity()
{

}

void Entity::Start()
{
	
	for (auto component : components)
	{
		component->Start();
	}
	
}

void Entity::Update(float dt)
{
	for (auto component : components)
	{
		component->Update(dt);
	}
	
}

void Entity::Destroy()
{
	for (auto component : components)
	{
		component->Destroy();
	}
	
}


void Entity::Draw(sf::RenderWindow& window) const
{
	if (RenderComponent* comp = GetComponent<RenderComponent>())
	{
		comp->Draw(window);
	}

#ifdef _DEBUG
	if (DynamicColliderComponent* comp = GetComponent<DynamicColliderComponent>())
	{
		comp->Draw(window);
	}
#endif 

	
}
