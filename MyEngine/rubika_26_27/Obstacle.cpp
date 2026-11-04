#include "Obstacle.h"
#include "DynamicColliderComponent.h"
#include "StaticRectangle.h"
#include "IDrawable.h"


Obstacle::Obstacle(std::string name) : Entity(name)
{
    AddComponent<TransformComponent>();
    RenderComponent* rc = AddComponent<RenderComponent>();
    StaticRectangle* drawable = rc->AddDrawable<StaticRectangle>();
    if (drawable->SetTextureAndStaticTile("Basement", "Wall_Classic_Up"))
    {
        Logger::Info("Success put drawable");
    }
    
    
    DynamicColliderComponent* dc = AddComponent<DynamicColliderComponent>();
    dc->SetHitboxSize(sf::Vector2f(50, 50));
    dc->SetCollisionFilterMask(ENTITY_TAG_PLAYER);

    dc->SetCollisionFilterMode(CollisionFilterMode::Any);

    AddTag(ENTITY_TAG_STATIC);
}

Obstacle::~Obstacle()
{
}
