#include "Scene.h"

#include <Entity.h>

#include <Imgui/imgui.h>

Scene::Scene(const std::string& name) : Name(name)
{
	Hash = std::hash<std::string>{}(Name);
}

Scene::~Scene()
{}

void Scene::Start()
{
	for (Entity* e : Entities)
	{
		e->Start();
	}
}

void Scene::Update(float fDeltaTime)
{
	for (Entity* e : Entities)
	{
		e->Update(fDeltaTime);
	}
}

void Scene::Destroy()
{
	for (Entity* e : Entities)
	{
		e->Destroy();
	}
}

Entity* Scene::CreateEntity(const std::string& entityName)
{
	Entity* e = new Entity(entityName);
	Entities.push_back(e);
	return e;
}

void Scene::RemoveEntity(Entity* entity)
{
	for (auto it = Entities.begin(); it != Entities.end(); ++it)
	{
		if (entity == *it)
		{
			entity->Destroy();
			it = Entities.erase(it);
		}
	}
}

void Scene::Draw(sf::RenderWindow& window) const
{
	for (Entity* e : Entities)
	{
		e->Draw(window);
	}
}

void Scene::DrawDebug()
{
	for (Entity* e : Entities)
	{
		ImGui::PushID(e);

		const char* preview = e->GetName().size() ? e->GetName().c_str() : "Entity";
		if (ImGui::TreeNode(preview))
		{
			e->DrawDebug();
			ImGui::TreePop();
		}

		ImGui::PopID();
	}
}

