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
	auto* currentScene = gData.SceneMgr->currentScene;
	if (tag == ENTITY_TAG_STATIC 
		&& std::find(currentScene->staticEntityList.begin(), currentScene->staticEntityList.end(), this) != currentScene->staticEntityList.end())
	{
		currentScene->staticEntityList.push_back(this);
	}
	if (tag == ENTITY_TAG_DYNAMIC
		&& std::find(currentScene->dynamicEntityList.begin(), currentScene->dynamicEntityList.end(), this) != currentScene->dynamicEntityList.end()) {

		currentScene->dynamicEntityList.push_back(this);
	}
	
}

void Entity::RemoveTag(unsigned int tag)
{
	m_Tags &= ~tag;
	auto* currentScene = gData.SceneMgr->currentScene;
	if (tag == ENTITY_TAG_STATIC)
	{
		currentScene->staticEntityList.erase(std::remove(currentScene->staticEntityList.begin(), currentScene->staticEntityList.end(), this), currentScene->staticEntityList.end());
	}
	else if (tag == ENTITY_TAG_DYNAMIC)
	{
		currentScene->dynamicEntityList.erase(std::remove(currentScene->dynamicEntityList.begin(), currentScene->dynamicEntityList.end(), this), currentScene->dynamicEntityList.end());
	}

		
	
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
