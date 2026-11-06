#include "Scene.h"
#include "Profiler.h"

Scene::Scene(const std::string& name)
{
	sceneName = name;

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

	ManageCollisions();
}

void Scene::Draw(sf::RenderWindow& window) const
{
	for (auto entity : entityList)
	{
		entity->Draw(window);
	}

	if (debugGrid)
	{
		staticGrid.DrawDebug(window);
		dynamicGrid.DrawDebug(window);

		//for (Entity* dynamic : dynamicEntityList)
		//{
		//	auto* coll = dynamic->GetComponent<DynamicColliderComponent>();
		//	if (!coll) continue;
//
		//	staticGrid.DrawDebugCellsOf(window, coll->GetWorldHitbox(), sf::Color(255, 0, 0, 110));
		//}
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

	if (entityToAdd->HasTag(ENTITY_TAG_STATIC))
		staticGrid.AddEntity(entityToAdd);
	if (entityToAdd->HasTag(ENTITY_TAG_DYNAMIC))
		dynamicGrid.AddEntity(entityToAdd);

	entityToAdd->Start();
}

void Scene::RemoveEntityFromPool(Entity* entityToRemove)
{
	auto it = std::find(entityList.begin(), entityList.end(), entityToRemove);
	if (it == entityList.end()) return;

	staticGrid.RemoveEntity(entityToRemove);
	dynamicGrid.RemoveEntity(entityToRemove);

	delete* it;
	entityList.erase(it);
}

void Scene::ManageCollisions()
{

	std::vector<Entity*> staticCandidates;	
	std::vector<Entity*> dynamicCandidates;
	std::vector<Entity*> dynamics;
	dynamicGrid.GetAllEntities(dynamics);

	for (Entity* entity : dynamics)
		dynamicGrid.UpdateEntity(entity);
	
	for (Entity* entity : dynamics)
	{
		auto* coll = entity->GetComponent<DynamicColliderComponent>();
		if (!coll || !coll->AreCollisionsEnabled() || !coll->HasCollisionCallback()) continue;

		staticGrid.GetEntities(coll->GetWorldHitbox(), staticCandidates);
		dynamicGrid.GetEntities(coll->GetWorldHitbox(), dynamicCandidates);
		ResolveAgainst(entity, coll, staticCandidates);
		ResolveAgainst(entity, coll, dynamicCandidates);
	}

}

void Scene::ResolveAgainst(Entity* mover, DynamicColliderComponent* moverColl, const std::vector<Entity*>& candidates)
{


	PROFILER_EVENT_BEGIN(PROFILER_COLOR_BLUE, "ResolveCollision");
	auto* transform = mover->GetComponent<TransformComponent>();

	for (Entity* other : candidates)
	{
		if (other == mover) continue;

		auto* otherColl = other->GetComponent<DynamicColliderComponent>();
		if (!otherColl || !otherColl->AreCollisionsEnabled()) continue;

		if (!moverColl->PassesCollisionFilter(other)) continue;

		sf::FloatRect myBox = moverColl->GetWorldHitbox();
		sf::FloatRect otherBox = otherColl->GetWorldHitbox();

		auto overlap = myBox.findIntersection(otherBox);
		if (!overlap) continue;

		if (moverColl->NotifyCollision(other) != OnCollisionChoice::Block) continue;

		const sf::FloatRect& o = *overlap;
		sf::Vector2f myCenter = myBox.getCenter();
		sf::Vector2f otherCenter = otherBox.getCenter();
		sf::Vector2f push(0.f, 0.f);

		if (o.size.x < o.size.y) push.x = (myCenter.x < otherCenter.x) ? -o.size.x : o.size.x;
		else push.y = (myCenter.y < otherCenter.y) ? -o.size.y : o.size.y;

		transform->SetWorldPosition(transform->GetWorldPosition() + push);
	}
	PROFILER_EVENT_END();
}

void Scene::SetupGrid(const sf::FloatRect& bounds, const sf::Vector2f& cellSize)
{
	staticGrid.SetBounds(bounds);
	staticGrid.SetCellSize(cellSize);
	staticGrid.SetDebugColor(sf::Color(255, 200, 0, 90));
	staticGrid.Build();

	dynamicGrid.SetBounds(bounds);
	dynamicGrid.SetCellSize(cellSize);
	dynamicGrid.SetDebugColor(sf::Color(255, 0, 0, 110));
	dynamicGrid.Build();
}


