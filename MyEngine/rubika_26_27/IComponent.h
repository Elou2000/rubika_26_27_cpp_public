#pragma once

#include <string>

class Entity;

class IComponent abstract
{
public:
	IComponent() = delete;
	IComponent(Entity& entity);

	virtual ~IComponent();

	virtual void Start() = 0;

	virtual void Update(float fDeltaTime) = 0;

	virtual void Destroy() = 0;

protected:
	Entity& GetEntity() const { return ParentEntity; };

private:
	Entity& ParentEntity;
};
