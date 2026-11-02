#pragma once

#include <string>

class Entity;

class IComponent abstract
{
public:
	IComponent() = delete;
	IComponent(Entity& entity) : ParentEntity(entity) {}

	virtual ~IComponent() = default;

	virtual void Start() = 0;

	virtual void Update(float fDeltaTime) = 0;

	virtual void Destroy() = 0;

	virtual const char* GetName() const = 0;

	virtual void DrawDebug() = 0;

protected:
	Entity& GetEntity() const { return ParentEntity; };

private:
	Entity& ParentEntity;
};
