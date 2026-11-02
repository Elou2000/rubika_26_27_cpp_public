#include "Entity.h"

#include <IComponent.h>
#include <RenderComponent.h>
#include <Imgui/imgui.h>


Entity::Entity(std::string friendlyName) : FriendlyName(friendlyName)
{}

Entity::~Entity()
{
	for (const IComponent* component : Components)
	{
		delete component;
	}

	Components.clear();
}

void Entity::Start()
{
	for (IComponent* c : Components)
	{
		c->Start();
	}
}

void Entity::Update(float fDeltaTime)
{
	for (IComponent* c : Components)
	{
		c->Update(fDeltaTime);
	}
}

void Entity::Destroy()
{
	for (IComponent* c : Components)
	{
		c->Destroy();
	}
}

void Entity::Draw(sf::RenderWindow& window) const
{
	RenderComponent* r = GetComponent<RenderComponent>();
	if (r)
	{
		r->Draw(window);
	}
}

void Entity::DrawDebug()
{
#ifdef _USE_IMGUI
	for (IComponent* c : Components)
	{
		if (ImGui::TreeNode(c->GetName()))
		{
			c->DrawDebug();
			ImGui::TreePop();
		}
	}
#endif
}
