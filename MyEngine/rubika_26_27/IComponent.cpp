#include "IComponent.h"

IComponent::IComponent(Entity& entity) : ParentEntity(entity)
{

}

IComponent::~IComponent()
{
}
