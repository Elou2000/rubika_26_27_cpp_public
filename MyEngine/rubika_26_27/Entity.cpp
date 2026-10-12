#include "Entity.h"
#include "Logger.h"


Entity::Entity(std::string friendlyName)
{
	entityName = friendlyName;
	tc = TransformComponent();
	Logger::Info("Unit spawned");
}

Entity::~Entity()
{

}

void Entity::Start()
{
	
}

void Entity::Update(float dt)
{

}

void Entity::Destroy()
{

}