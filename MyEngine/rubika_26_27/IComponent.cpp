#include "IComponent.h"
#include "Logger.h"

IComponent::IComponent(Entity& entity) : ParentEntity(entity)
{
	
}

IComponent::~IComponent()
{
}

std::string IComponent::GetName() const
{
    std::string name = typeid(*this).name();

    if (name.starts_with("class "))
        name.erase(0, 6);

    return name;
}

