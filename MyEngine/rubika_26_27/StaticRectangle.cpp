#include "StaticRectangle.h"
#include "Globals.h"
#include "TextureMgr.h"

void StaticRectangle::Start()
{
	sf::RectangleShape* rectangle = new sf::RectangleShape;
	rectangle->setFillColor(sf::Color(255, 0, 0, 255));
	rectangle->setSize(sf::Vector2f(100, 100));
	rectangle->setPosition(sf::Vector2f(10, 200));
	rectangle->setTexture(&gData.TextureMgr->GetTextureData("enemy_texture_5").Texture);

	
	Drawable = rectangle;

}

void StaticRectangle::Update(float dt)
{

}

void StaticRectangle::Destroy()
{

}