#pragma once

#include <DebugMgr.h>

#include <SFML/Graphics/Texture.hpp>
#include <rapidxml/rapidxml.hpp>

#include <filesystem>
#include <unordered_map>
#include <string>

struct AnimationData
{
	int StartX = 0;
	int StartY = 0;
	int SizeX = 0;
	int SizeY = 0;
	int OffsetX = 0;
	int OffsetY = 0;
	int AnimationSpriteCount = 0;
	int SpriteOnLine = 0;
	bool IsReverted = false;
	float TimeBetweenAnimationInS = 0.f;
};

struct StaticTileData
{
	int StartX = 0;
	int StartY = 0;
	int SizeX = 0;
	int SizeY = 0;
	bool IsRevertedX = false;
	bool IsRevertedY = false;
};

struct TextureData
{
	sf::Texture Texture;
	std::unordered_map<std::string, AnimationData> AnimationsData;
	std::unordered_map<std::string, StaticTileData> StaticTilesData;

	friend class TextureMgr;
};

class TextureMgr final : public IDebugable
{
public:
	void Init();
	void Shut();

	/*
	 * Loads the texture using the given path, including its metadata.
	 * Obviously, this function will also store the loaded data
	 * If errors occur during the loading process, this function must return false.
	 * Return true otherwise.
	*/
	bool LoadTexture(const std::filesystem::path& path);

	/*
	 * Returns the corresponding TextureData using the name of the texture. Returns the default TextureData in case of an unknown name
	 */
	const TextureData& GetTextureData(const std::string& name) const;

	virtual void DrawDebug() override;

	static const sf::Texture& GetEmptyTexture();

private:
	std::unordered_map<std::string, TextureData> Textures;

	bool LoadTextureMetadata(const std::filesystem::path& path, TextureData& textureData);
	bool LoadAnimationMetadata(rapidxml::xml_node<>* node, TextureData& textureData);
	bool LoadStaticTileMetadata(rapidxml::xml_node<>* node, TextureData& textureData);
};