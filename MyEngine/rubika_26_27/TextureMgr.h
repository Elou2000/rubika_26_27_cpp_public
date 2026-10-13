#pragma once

#include <SFML/Graphics/Texture.hpp>

#include <filesystem>
#include <string>
#include <unordered_map>
#include <rapidxml/rapidxml.hpp>
#include <rapidxml/rapidxml_utils.hpp>
#include "DebugMgr.h"
#include <globals.h>

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
	TextureData();
	~TextureData();

	sf::Texture Texture;
	std::unordered_map<std::string, AnimationData> animationsData;
	std::unordered_map<std::string, StaticTileData> staticTilesData;


	friend class TextureMgr;
};

class TextureMgr : IDebugable
{
public:
	TextureMgr();
	~TextureMgr();
	void Init();
	void Shut();

	bool LoadTexture(const std::filesystem::path& path);
	const TextureData& GetTextureData(const std::string& name) const;
	

	bool ReadInt(const rapidxml::xml_node<>& parent, const char* name, int& out);
	bool ReadBool(const rapidxml::xml_node<>& parent, const char* name, bool& out);
	bool ReadName(const rapidxml::xml_node<>& node, std::string& out);

	bool FileExists(const std::filesystem::path& path);
	bool LoadMetaData(const std::filesystem::path& metaPath, TextureData& out);
	bool LoadAnimations(const rapidxml::xml_node<>& root, TextureData& out);
	bool LoadBackgrounds(const rapidxml::xml_node<>& root, TextureData& out);

	virtual void DrawDebug() override;
	
	std::unordered_map<std::string, TextureData> m_textures;
	TextureData m_defaultTexture;
	
	
};

