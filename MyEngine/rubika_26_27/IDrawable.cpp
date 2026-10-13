#include "IDrawable.h"

IDrawable::IDrawable() : Visible(true), WorldTransform(sf::Transform::Identity), Drawable(nullptr)
{}

IDrawable::~IDrawable()
{}

bool IDrawable::IsVisible() const
{
	return Visible;
}

void IDrawable::SetVisibility(bool visible)
{
	Visible = visible;
}

const sf::Transform& IDrawable::GetWorldTransform() const
{
	return WorldTransform;
}

void IDrawable::SetWorldTransform(const sf::Transform& transform)
{
	WorldTransform = transform;
}

void IDrawable::Draw(sf::RenderWindow& window) const
{
	if (!Drawable)
	{
		return;
	}

	sf::RenderStates states = sf::RenderStates::Default;
	states.transform = WorldTransform;

	window.draw(*Drawable, states);
}
