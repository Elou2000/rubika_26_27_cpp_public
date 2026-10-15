#include "StaticRectangle.h"
#include "Globals.h"
#include "TextureMgr.h"

void StaticRectangle::Start()
{
	Drawable = &rectangle;

	

}

void StaticRectangle::Update(float dt)
{

}

void StaticRectangle::Destroy()
{

}

bool StaticRectangle::SetTexture(const std::string& textureName)
{
	const TextureData* data = &gData.TextureMgr->GetTextureData(textureName);


	if (data != nullptr)
	{
		textureData = data;
		rectangle.setTexture(&data->Texture);
		return true;
	}
	else
	{
		return false;
	}
}

bool StaticRectangle::SetStaticTile(const std::string& staticTileName)
{
	if (textureData == nullptr) return false;
	

	
	auto it = textureData->staticTilesData.find(staticTileName);
	if (it == textureData->staticTilesData.end()) return false;
	
	const StaticTileData& data = it->second;

	
	int XReversed = data.IsRevertedX ? -1 : 1;
	int YReversed = data.IsRevertedY ? -1 : 1;
	

	

	rectangle.setTextureRect(sf::IntRect(
		sf::Vector2i(
			data.IsRevertedX ? data.StartX + data.SizeX : data.StartX,
			data.IsRevertedY ? data.StartY + data.SizeY : data.StartY
		),
		sf::Vector2i(
			data.IsRevertedX ? -data.SizeX : data.SizeX,
			data.IsRevertedY ? -data.SizeY : data.SizeY
		)
	));

	rectangle.setSize(sf::Vector2f(data.SizeX * XReversed, data.SizeY * YReversed));

	return true;

}

bool StaticRectangle::SetTextureAndStaticTile(const std::string& textureName, const std::string& staticTileName)
{
	if(!SetTexture(textureName)) return false;
	if(!SetStaticTile(staticTileName)) return false;

	return true;
}
