#include "DynamicColliderComponent.h"
#include "StaticRectangle.h"
#include "Sprite.h"
#include "Entity.h"
#include "Globals.h"
#include "InputMgr.h"
#include "SceneMgr.h"
#include "Scene.h"



DynamicColliderComponent::DynamicColliderComponent(Entity& entity) : IComponent(entity)
{
	gData.InputMgr->CreateInputAction("ToggleDebugCollider");
	gData.InputMgr->RegisterToInputAction("ToggleDebugCollider", "ToggleDebugCollider", [this](float value) 
		{
			if (value > 0)
			{
				ToggleDebugVisual();
			}
		}, this);
	gData.InputMgr->BindKeyToInputAction(sf::Keyboard::Key::A, "ToggleDebugCollider");


}


DynamicColliderComponent::~DynamicColliderComponent()
{
}

void DynamicColliderComponent::Start()
{

}


void DynamicColliderComponent::Update(float fDeltaTime)
{
	PROFILER_EVENT_BEGIN(PROFILER_COLOR_GREEN, "Update Collider Component");

	Entity& owner = GetEntity();
	visualDebug.setPosition(hitbox.position + owner.GetComponent<TransformComponent>()->GetWorldPosition());
	visualDebug.setSize(sf::Vector2f(hitbox.size.x * owner.GetComponent<TransformComponent>()->GetScale().x, hitbox.size.y * owner.GetComponent<TransformComponent>()->GetScale().y));
	visualDebug.setFillColor(sf::Color::Transparent);
	visualDebug.setOutlineThickness(2);
	visualDebug.setOutlineColor(sf::Color::Green);

	DetectCollisions();
	PROFILER_EVENT_END();
}

void DynamicColliderComponent::Draw(sf::RenderWindow& window)
{
	if (drawDebug)
	{
		window.draw(visualDebug);
	}
	
}

void DynamicColliderComponent::Destroy()
{

}

void DynamicColliderComponent::DrawDebug()
{
#ifdef _USE_IMGUI
	float pos[2] = { hitbox.position.x, hitbox.position.y };
	if (ImGui::DragFloat2("Offset", pos, 1.f))
		SetHitboxOffset(sf::Vector2(pos[0], pos[1]));


	float scl[2] = { hitbox.size.x, hitbox.size.y };
	if (ImGui::DragFloat2("Scale", scl, 0.01f, 0.01f, 100.f))
		SetHitboxSize(sf::Vector2(scl[0], scl[1]));

	if (ImGui::Button("Reset"))
	{
		SetHitboxOffset(sf::Vector2(0.f, 0.f));
		SetHitboxSize(sf::Vector2(1.f, 1.f));
	}

	ImGui::Checkbox("Draw Visual", &drawDebug);
	
		
	
#endif
}

void DynamicColliderComponent::SetHitboxSize(sf::Vector2f size)
{
	hitbox.size.x = size.x;
	hitbox.size.y = size.y;

}

void DynamicColliderComponent::SetHitboxOffset(sf::Vector2f offset)
{
	hitbox.position.x = offset.x;
	hitbox.position.y = offset.y;

}

void DynamicColliderComponent::EnableCollision(bool bEnable)
{
	collisionEnabled = bEnable;
}

bool DynamicColliderComponent::AreCollisionsEnabled() const
{
	return collisionEnabled;
}

void DynamicColliderComponent::SetCollisionFilterMask(unsigned int mask)
{
	collisionFilterMask = mask;
}

void DynamicColliderComponent::SetCollisionFilterMode(CollisionFilterMode mode)
{
	collisionFilterMode = mode;
}

bool DynamicColliderComponent::PassesCollisionFilter(const Entity* other) const
{
	if (collisionFilterMask == ENTITY_TAG_NONE)
		return false;

	switch (collisionFilterMode)
	{
	case CollisionFilterMode::Any:
		return (other->GetTags() & collisionFilterMask) != 0;

	case CollisionFilterMode::All:
		return (other->GetTags() & collisionFilterMask) == collisionFilterMask;
	}

	return false;
}



bool DynamicColliderComponent::IsColliding(sf::FloatRect otherHitbox)
{
	return GetWorldHitbox().findIntersection(otherHitbox).has_value();
}

void DynamicColliderComponent::DetectCollisions()
{
	if (!collisionEnabled || !onCollision) return;

	Entity& owner = GetEntity();
	auto* transform = owner.GetComponent<TransformComponent>();

	for (Entity* other : gData.SceneMgr->currentScene->entityList)
	{
		if (other == &owner) continue;

		auto* otherColl = other->GetComponent<DynamicColliderComponent>();
		if (!otherColl || !otherColl->AreCollisionsEnabled()) continue;

		if (!PassesCollisionFilter(other))continue;

		sf::FloatRect myBox = GetWorldHitbox();
		auto overlap = myBox.findIntersection(otherColl->GetWorldHitbox());
		if (!overlap) continue;

		if (onCollision(other) != OnCollisionChoice::Block) continue;

	
		const sf::FloatRect& o = *overlap;
		sf::Vector2f myCenter = myBox.getCenter();
		sf::Vector2f otherCenter = otherColl->GetWorldHitbox().getCenter();
		sf::Vector2f push(0.f, 0.f);

		if (o.size.x < o.size.y) push.x = (myCenter.x < otherCenter.x) ? -o.size.x : o.size.x;
		else push.y = (myCenter.y < otherCenter.y) ? -o.size.y : o.size.y;

		transform->SetWorldPosition(transform->GetWorldPosition() + push);
	}
}

sf::FloatRect DynamicColliderComponent::GetWorldHitbox() const
{
	const auto* transform = GetEntity().GetComponent<TransformComponent>();

	sf::Vector2f position = transform->GetWorldPosition();
	sf::Vector2f scale = transform->GetScale();

	return sf::FloatRect(hitbox.position + position,sf::Vector2f(hitbox.size.x * scale.x,hitbox.size.y * scale.y));
}

const sf::FloatRect& DynamicColliderComponent::GetHitbox() const
{
	return hitbox;
}

void DynamicColliderComponent::SetOnCollisionCallback(std::function<OnCollisionChoice(const Entity*)> callback)
{
	onCollision = std::move(callback);
}

void DynamicColliderComponent::ToggleDebugVisual()
{
	drawDebug = !drawDebug;
}


