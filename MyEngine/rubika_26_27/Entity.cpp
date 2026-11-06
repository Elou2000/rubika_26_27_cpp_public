#include "Entity.h"
#include "Logger.h"
#include "DynamicColliderComponent.h"
#include "SceneMgr.h"



Entity::Entity(std::string friendlyName)
{
	entityName = friendlyName;	
}

Entity::~Entity()
{

}

void Entity::Start()
{
	
	for (auto component : components)
	{
		component->Start();
	}
	
}

void Entity::Update(float dt)
{
	for (auto component : components)
	{
		component->Update(dt);
	}
	
}

void Entity::Destroy()
{
	for (auto component : components)
	{
		component->Destroy();
	}
	
}

void Entity::AddTag(unsigned int tag)
{
	m_Tags |= tag;

	Scene* scene = gData.SceneMgr->currentScene;
	if (!scene) return;

	
	auto& all = scene->entityList;
	if (std::find(all.begin(), all.end(), this) == all.end()) return;

	if (tag & ENTITY_TAG_STATIC) scene->staticGrid.AddEntity(this);
	if (tag & ENTITY_TAG_DYNAMIC) scene->dynamicGrid.AddEntity(this);
}

void Entity::RemoveTag(unsigned int tag)
{
	m_Tags &= ~tag;

	Scene* scene = gData.SceneMgr->currentScene;
	if (!scene) return;

	if (tag & ENTITY_TAG_STATIC)
		scene->staticGrid.RemoveEntity(this);
	if (tag & ENTITY_TAG_DYNAMIC)
		scene->dynamicGrid.RemoveEntity(this);
}


bool Entity::HasTag(unsigned int tag) const
{
	return tag != ENTITY_TAG_NONE && (m_Tags & tag) == tag;
}

unsigned int Entity::GetTags() const
{
	return m_Tags;
}


void Entity::Draw(sf::RenderWindow& window) const
{
	if (RenderComponent* comp = GetComponent<RenderComponent>())
	{
		comp->Draw(window);
	}

#ifdef _DEBUG
	if (DynamicColliderComponent* comp = GetComponent<DynamicColliderComponent>())
	{
		comp->Draw(window);
	}
#endif 

	
}
