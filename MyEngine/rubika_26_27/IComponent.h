#pragma once

#include <string>

#ifdef _USE_IMGUI
#include <Imgui/imgui.h>
#endif

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

	virtual void DrawDebug() = 0;

	std::string GetName() const;

protected:
	Entity& GetEntity() const { return ParentEntity; };

private:
	Entity& ParentEntity;
};
