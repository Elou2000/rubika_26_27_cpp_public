#include "Entity.h"
#include "Logger.h"



Entity::Entity(std::string friendlyName)
{
	entityName = friendlyName;
	//tc = new TransformComponent(*this);
	//rc = new RenderComponent(*this);
	//if(IComponent* comp = AddComponent<RenderComponent>()) comp->Start();
	//if (IComponent* comp = AddComponent<TransformComponent>()) comp->Start();
	Logger::Info("Unit spawned");
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
		component->Update( dt);
	}
	
}

void Entity::Destroy()
{
	for (auto component : components)
	{
		component->Destroy();
	}
	
}

//TransformComponent* Entity::AddTransformComponent()
//{
//	if (GetTransformComponent())
//	{
//		Logger::Error("Entity already has this component");
//	}
//	else
//	{
//		components.emplace_back(new TransformComponent(*this));
//	}
//}
//
//RenderComponent* Entity::AddRenderComponent()
//{
//	if (GetRenderComponent())
//	{
//		Logger::Error("Entity already has this component");
//	}
//	else
//	{
//		components.emplace_back(new RenderComponent(*this));
//	}
//}
//
//TransformComponent* Entity::GetTransformComponent() const
//{
//	for (auto component : components)
//	{
//		if (TransformComponent* comp = dynamic_cast<TransformComponent*>(component))
//		{
//			return comp;
//		}	
//	}
//	return nullptr;
//}
//
//RenderComponent* Entity::GetRenderComponent() const
//{
//	for (auto component : components)
//	{
//		if (RenderComponent* comp = dynamic_cast<RenderComponent*>(component))
//		{
//			return comp;
//		}
//	}
//	return nullptr;
//}


void Entity::Draw(sf::RenderWindow& window) const
{
	GetComponent<RenderComponent>()->Draw(window);
}
