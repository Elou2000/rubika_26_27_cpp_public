#include "RenderComponent.h"
#include "IDrawable.h"
#include "Entity.h"
#include "Logger.h"

RenderComponent::RenderComponent(Entity& entity) : IComponent(entity)
{
	rectangleDrawable = new StaticRectangle();
}

RenderComponent::~RenderComponent()
{
}

void RenderComponent::Start() 
{
	rectangleDrawable->Start();
}

void RenderComponent::Update(float fDeltaTime)
{
	rectangleDrawable->Update(fDeltaTime);
}

void RenderComponent::Draw(sf::RenderWindow& window)
{
	rectangleDrawable->Draw(window);


}

void RenderComponent::Destroy()
{
	rectangleDrawable->Destroy();
}
