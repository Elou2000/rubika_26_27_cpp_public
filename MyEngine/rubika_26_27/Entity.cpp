#include "Entity.h"
#include "Logger.h"



Entity::Entity(std::string friendlyName)
{
	entityName = friendlyName;
	tc = new TransformComponent(*this);
	rc = new RenderComponent(*this);
	Logger::Info("Unit spawned");
}

Entity::~Entity()
{

}

void Entity::Start()
{
	rc->Start();
}

void Entity::Update(float dt)
{
	rc->Update(dt);
}

void Entity::Destroy()
{
	rc->Destroy();
}

void Entity::Draw(sf::RenderWindow& window) const
{
	rc->Draw(window);
}
