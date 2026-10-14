#include "IComponent.h"
#include "Logger.h"
#include "Entity.h"
#include <string>

template<typename C>
inline C* Entity::AddComponent()
{
	if (GetComponent<C>())
	{
		Logger::Error(std::string("Entity already has this component: ") + typeid(C).name());
		return nullptr;
	}
	else
	{	
		C* component = new C(*this);
		Logger::Info("Added " + component->GetName());
		components.emplace_back(component);
		component->Start();
		return component;
	}
}

template<typename C>
inline C* Entity::GetComponent() const
{
	for (IComponent* component : components)
	{
		if (C* comp = dynamic_cast<C*>(component))
		{
			return comp;
		}
	}

	return nullptr;
}
