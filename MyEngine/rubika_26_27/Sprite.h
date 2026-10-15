#pragma once
#include "IDrawable.h"
#include "SFML/Graphics/Sprite.hpp";


class Sprite : public IDrawable
{
public:

	Sprite();

	void Start();
	void Update(float dt);
	void Destroy();


	bool SetTexture(const std::string& textureName);
	bool SetAnimationData(const std::string& staticTileName);
	bool SetTextureAndAnimationData(const std::string& textureName, const std::string& staticTileName);

	sf::Sprite sprite;
	const AnimationData* animData;

private:
	void UpdateTextureRect();
	int currentFrame = 0;
	float animationTimer = 0.f;
};



