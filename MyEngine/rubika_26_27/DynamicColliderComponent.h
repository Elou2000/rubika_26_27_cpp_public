#pragma once

#include <IComponent.h>

#include <SfML/Graphics/Rect.hpp>
#include <functional>
#include <RenderComponent.h>

enum class OnCollisionChoice
{
	Ignore,
	Block,
};

class DynamicColliderComponent : public IComponent
{
public:
	DynamicColliderComponent(Entity& entity);
	~DynamicColliderComponent();

	virtual void Start() override;
	virtual void Update(float fDeltaTime) override;
	void Draw(sf::RenderWindow& window);
	virtual void Destroy() override;
	virtual void DrawDebug() override;

	void SetHitboxSize(sf::Vector2f size);
	void SetHitboxOffset(sf::Vector2f offset);

	void EnableCollision(bool bEnable);
	bool AreCollisionsEnabled() const;

	const sf::FloatRect& GetHitbox() const;

	void SetOnCollisionCallback(std::function<OnCollisionChoice(const Entity*)> callback);
	void ToggleDebugVisual();

private:
	sf::FloatRect hitbox;
	bool collisionEnabled;

	bool drawDebug;
	sf::RectangleShape visualDebug;
};