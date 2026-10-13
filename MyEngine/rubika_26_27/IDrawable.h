#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transform.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class IDrawable abstract
{
public:
	IDrawable();
	virtual ~IDrawable();

	virtual void Start() = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void Destroy(float deltaTime) = 0;

	bool IsVisible() const;
	void SetVisibility(bool visible);

	const sf::Transform& GetWorldTransform() const;
	void SetWorldTransform(const sf::Transform& transform);

	void Draw(sf::RenderWindow& window) const;

protected:
	bool Visible;
	sf::Transform WorldTransform;
	sf::Drawable* Drawable; // Pointer to the SFML derived class defined in IDrawable's derived class
};