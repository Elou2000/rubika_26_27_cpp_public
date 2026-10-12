#include "Scene.h"

Scene::Scene(const std::string& name)
{
	sceneName = name;

	AddEntityToPool(new Entity("test1"));
	AddEntityToPool(new Entity("test2"));
	AddEntityToPool(new Entity("test3"));
}

Scene::~Scene()
{
}

void Scene::Start()
{
	for (auto entity : entityList)
	{
		entity->Start();
	}
}

void Scene::Update(float dt)
{
	for (auto entity : entityList)
	{
		entity->Update(dt);
	}
}

void Scene::Destroy()
{
	for (auto entity : entityList)
	{
		entity->Destroy();
	}
}

void Scene::AddEntityToPool(Entity* entityToAdd)
{
	entityList.push_back(entityToAdd);
}

void Scene::RemoveEntityFromPool(Entity* entityToRemove)
{
	auto it = std::find(entityList.begin(),entityList.end(),entityToRemove);

	if (it != entityList.end())
	{
		delete* it;
		entityList.erase(it);
	}
}
