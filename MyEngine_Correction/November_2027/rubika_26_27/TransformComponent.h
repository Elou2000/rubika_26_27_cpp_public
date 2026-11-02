#pragma once

#include <IComponent.h>

#include <SFML/Graphics/Transform.hpp>

class TransformComponent : public IComponent
{
public:
	struct DecomposedData
	{
		sf::Vector2f Position;
		float Rotation;
		sf::Vector2f Scale;
	};

	TransformComponent(Entity& entity);

	const sf::Transform& GetMatrix() const;

	void SetWorldPosition(sf::Vector2f newPosition);
	sf::Vector2f GetWorldPosition() const;

	void SetRotation(float newRotation);
	float GetRotation() const;

	void SetScale(sf::Vector2f newScale);
	sf::Vector2f GetScale() const;

	void Start();
	void Update(float fDeltaTime);
	void Destroy();

	const char* GetName() const override { return "TransformComponent"; }
	void DrawDebug() override;

private:
	sf::Transform Matrix;

	sf::Vector2f WorldPosition;
	float Rotation;
	sf::Vector2f Scale;

	void UpdateMatrix();
};
