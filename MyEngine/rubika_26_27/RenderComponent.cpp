#include "RenderComponent.h"
#include "IDrawable.h"
#include "Entity.h"
#include "Logger.h"

RenderComponent::RenderComponent(Entity& entity) : IComponent(entity)
{
	//rectangleDrawable = new StaticRectangle();
	//AddDrawable<StaticRectangle>();
}

RenderComponent::~RenderComponent()
{
}

void RenderComponent::Start() 
{
	for (auto drawable : drawables)
	{
		drawable->Start();
	}
	
}

void RenderComponent::Update(float fDeltaTime)
{
	for (auto drawable : drawables)
	{
		drawable->Update(fDeltaTime);
	}
}

void RenderComponent::Draw(sf::RenderWindow& window)
{
	for (auto drawable : drawables)
	{
		drawable->Draw(window);
	}


}

void RenderComponent::Destroy()
{
	for (auto drawable : drawables)
	{
		drawable->Destroy();
	}
}
