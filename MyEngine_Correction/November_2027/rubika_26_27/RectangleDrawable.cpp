#include "RectangleDrawable.h"

#include <Globals.h>

RectangleDrawable::RectangleDrawable()
{
	Drawable = &DrawableCasted;
}

RectangleDrawable::~RectangleDrawable()
{}

void RectangleDrawable::Start()
{
	sf::IntRect rect;
	if (!TileData.IsRevertedX)
	{
		rect.size.x = TileData.SizeX;
		rect.position.x = TileData.StartX;
	}
	else
	{
		rect.size.x = -TileData.SizeX;
		rect.position.x = TileData.StartX + TileData.SizeX;
	}

	if (!TileData.IsRevertedY)
	{
		rect.size.y = TileData.SizeY;
		rect.position.y = TileData.StartY;
	}
	else
	{
		rect.size.y = -TileData.SizeY;
		rect.position.y = TileData.StartY + TileData.SizeY;
	}

	DrawableCasted.setSize(sf::Vector2f((float)TileData.SizeX, (float)TileData.SizeY));
	DrawableCasted.setTextureRect(rect);
}

void RectangleDrawable::Update(float)
{}

void RectangleDrawable::Destroy()
{
}

void RectangleDrawable::SetTexture(const std::string& textureName)
{
	const TextureData& textureData = gData.TextureMgr->GetTextureData(textureName);
	DrawableCasted.setTexture(&textureData.Texture);

	CurrentTexture = textureName;
}

void RectangleDrawable::SetTile(const std::string& tileNameName)
{
	const TextureData& textureData = gData.TextureMgr->GetTextureData(CurrentTexture);
	TileData = textureData.StaticTilesData.at(tileNameName);

	CurrentTile = tileNameName;
}

void RectangleDrawable::SetFillColor(sf::Color color)
{
	DrawableCasted.setFillColor(color);
}
