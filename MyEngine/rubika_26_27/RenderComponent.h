#pragma once
#include "IComponent.h"
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

	StaticRectangle* rectangleDrawable;
};

