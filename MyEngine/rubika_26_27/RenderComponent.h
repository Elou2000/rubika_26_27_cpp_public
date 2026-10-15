#pragma once
#include "IComponent.h"
#include "TransformComponent.h"
#include "StaticRectangle.h"


class RenderComponent : public IComponent
{
public:
	RenderComponent(Entity& entity);
	~RenderComponent();

	void Start();
	void Update(float fDeltaTime);
	void Draw(sf::RenderWindow& window);
	void Destroy();
	void DrawDebug() override;

	template <typename C>
	C* AddDrawable();

	TransformComponent* transformOwner;

	std::vector<IDrawable*> drawables;
};

#include "RenderComponent.hxx"

