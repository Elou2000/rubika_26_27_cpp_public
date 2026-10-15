#pragma once
#include "IDrawable.h"
#include "SFML/Graphics/RectangleShape.hpp"



class StaticRectangle : public IDrawable
{
public:
	void Start();
	void Update(float dt);
	void Destroy();

	bool SetTexture(const std::string& textureName);
	bool SetStaticTile(const std::string& staticTileName);
	bool SetTextureAndStaticTile(const std::string& textureName, const std::string& staticTileName);

	sf::RectangleShape rectangle;

};

