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
	void DrawDebug() override;

	template <typename C>
	C* AddDrawable();



	std::vector<IDrawable*> drawables;
	//StaticRectangle* rectangleDrawable;
};

#include "RenderComponent.hxx"

