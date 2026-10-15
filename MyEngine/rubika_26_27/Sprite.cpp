#include "Sprite.h"
#include "Logger.h"

static sf::Texture defaultTexture;
Sprite::Sprite(): sprite(defaultTexture)
{

}

void Sprite::Start()
{
	Drawable = &sprite;
}

void Sprite::Update(float dt)
{
	if (animData == nullptr)
		return;

	if (animData->AnimationSpriteCount <= 0)
		return;

	if (animData->SpriteOnLine <= 0)
		return;

	if (animData->TimeBetweenAnimationInS <= 0.f)
		return;

	animationTimer += dt;

	if (animationTimer >= animData->TimeBetweenAnimationInS)
	{
		animationTimer -= animData->TimeBetweenAnimationInS;
		currentFrame = (currentFrame + 1) % animData->AnimationSpriteCount;
	}

	UpdateTextureRect();
}

void Sprite::Destroy()
{
	textureData = nullptr;
	animData = nullptr;
}

bool Sprite::SetTexture(const std::string& textureName)
{
	const TextureData* data = &gData.TextureMgr->GetTextureData(textureName);

	if (data != nullptr)
	{
		textureData = data;
		sprite.setTexture(data->Texture);
		return true;
	}
	else
	{
		return false;
	}
}

bool Sprite::SetAnimationData(const std::string& animationData)
{
	if (textureData == nullptr) return false;

	auto it = textureData->animationsData.find(animationData);
	if (it == textureData->animationsData.end()) return false;
	

	animData = &it->second;

	currentFrame = 0;
	animationTimer = 0.f;

	Logger::Error(std::to_string(animData->TimeBetweenAnimationInS));
	UpdateTextureRect();
	
	return true;
}

bool Sprite::SetTextureAndAnimationData(const std::string& textureName, const std::string& staticTileName)
{
	if (!SetTexture(textureName)) return false;
	if (!SetAnimationData(staticTileName)) return false;

	return true;
}

void Sprite::UpdateTextureRect()
{
	const int frame = currentFrame;

	const int column = frame % animData->SpriteOnLine;
	const int row = frame / animData->SpriteOnLine;

	const int frameX = animData->StartX +
		column * (animData->SizeX + animData->OffsetX);

	const int frameY = animData->StartY +
		row * (animData->SizeY + animData->OffsetY);

	sprite.setTextureRect({
		{frameX, frameY},
		{animData->SizeX, animData->SizeY}
		});

	sprite.setScale({
		animData->IsReverted ? -1.f : 1.f,
		1.f
		});
}
