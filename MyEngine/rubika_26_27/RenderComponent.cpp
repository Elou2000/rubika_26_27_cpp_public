#include "RenderComponent.h"
#include "IDrawable.h"
#include "Entity.h"
#include "Logger.h"

RenderComponent::RenderComponent(Entity& entity) : IComponent(entity)
{

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
	const sf::Transform* matrix = nullptr;
	if (transformOwner == nullptr)
	{
		if (auto* tc = GetEntity().GetComponent<TransformComponent>())
		{
			matrix = &tc->GetMatrix();
			transformOwner = tc;
		}
	}
	else
	{
		matrix = &transformOwner->GetMatrix();
	}

	for (auto drawable : drawables)
	{
		drawable->Update(fDeltaTime);


		if (matrix) drawable->SetWorldTransform(*matrix);

		
	}
}


void RenderComponent::Draw(sf::RenderWindow& window)
{
	
	for (auto drawable : drawables)
	{
		
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

	ImGui::Separator();

	if (drawables.empty())
		ImGui::TextDisabled("No drawables.");

	for (size_t i = 0; i < drawables.size(); ++i)
	{
		IDrawable* drawable = drawables[i];

		ImGui::PushID(drawable);
		std::string label = std::to_string(i) + " - " + typeid(*drawable).name();

		if (ImGui::TreeNode(label.c_str()))
		{
			drawable->DrawDebug();
			ImGui::TreePop();
		}
		ImGui::PopID();
	}
#endif
}
