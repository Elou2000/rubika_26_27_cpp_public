#pragma once

#include <IDrawable.h>
#include <TextureMgr.h>

#include <SFML/Graphics/RectangleShape.hpp>
#include <string>

class RectangleDrawable : public IDrawable
{
public:
	RectangleDrawable();
	~RectangleDrawable();

	virtual void Start() override;
	virtual void Update(float deltaTime) override;
	virtual void Destroy() override;

	void SetTexture(const std::string& textureName);
	void SetTile(const std::string& animationName);
	void SetFillColor(sf::Color color);

protected:
	sf::RectangleShape DrawableCasted;
	std::string CurrentTexture;
	std::string CurrentTile;

	StaticTileData TileData;
};