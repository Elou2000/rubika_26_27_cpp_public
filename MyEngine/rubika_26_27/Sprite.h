#pragma once
#include "IDrawable.h"
#include "SFML/Graphics/Sprite.hpp";


class Sprite : IDrawable
{
	void Start();
	void Update(float dt);
	void Destroy();

	sf::Sprite sprite;
};



