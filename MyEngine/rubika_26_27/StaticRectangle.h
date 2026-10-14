#pragma once
#include "IDrawable.h"
#include "SFML/Graphics/RectangleShape.hpp"


class StaticRectangle : public IDrawable
{
public:
	void Start();
	void Update(float dt);
	void Destroy();

};

