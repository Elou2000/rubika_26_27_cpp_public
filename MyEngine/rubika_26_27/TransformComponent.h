#pragma once

#include <SFML/Graphics/Transform.hpp>
#include "IComponent.h"

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
	~TransformComponent();


	const sf::Transform& GetMatrix() const;

	void SetWorldPosition(sf::Vector2f newPosition);
	sf::Vector2f GetWorldPosition() const;

	void SetWorldPositionQuery(sf::Vector2f newPosition);
	sf::Vector2f GetWorldPositionQuery() const;

	void SetRotation(float newRotation);
	float GetRotation() const;

	void SetScale(sf::Vector2f newScale);
	sf::Vector2f GetScale() const;

	void Start();
	void Update(float fDeltaTime);
	void Destroy();

	void DrawDebug() override;

private:
	sf::Transform Matrix;

	sf::Vector2f WorldPosition;
	float Rotation;
	sf::Vector2f Scale;

	sf::Vector2f WorldPositionQuery;

	void UpdateMatrix();
};
