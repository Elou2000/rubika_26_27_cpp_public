#pragma once

#include <IDrawable.h>
#include <TextureMgr.h>

#include <SFML/Graphics/Sprite.hpp>
#include <string>

class SpriteDrawable : public IDrawable
{
public:
	SpriteDrawable();

	virtual void Start() override;
	virtual void Update(float deltaTime) override;
	virtual void Destroy() override;

	void Reset();

	void SetTexture(const std::string& textureName);
	void SetAnimation(const std::string& animationName);

	void EnableAnimation(bool play);

protected:

	sf::Sprite DrawableCasted;
	AnimationData CurrentAnimationData;

	std::string CurrentTexture;
	std::string CurrentAnimation;

	int CurrentAnimationNb = 0;
	float CurrentAnimationTime = 0.f;

	bool PlayAnimation = true;
};