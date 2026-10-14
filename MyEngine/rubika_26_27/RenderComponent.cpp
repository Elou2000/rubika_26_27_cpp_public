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
	const sf::Transform* matrix = nullptr;

	if (auto* tc = GetEntity().GetComponent<TransformComponent>())
		matrix = &tc->GetMatrix();

	for (auto drawable : drawables)
	{
		if (matrix)
			drawable->SetWorldTransform(*matrix);

		if (drawable->IsVisible())
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


void RenderComponent::DrawDebug()
{
#ifdef _USE_IMGUI
	ImGui::Text("Drawables : %zu", drawables.size());

	if (ImGui::Button("Add StaticRectangle"))
		AddDrawable<StaticRectangle>();
#endif
}
