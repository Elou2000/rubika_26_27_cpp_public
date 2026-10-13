#include "TextureMgr.h"
#include "Logger.h"

#ifdef _USE_IMGUI
#include <Imgui/imgui.h>
#include <Imgui/imgui-SFML.h>
#endif


TextureMgr::TextureMgr()
{
	gData.DebugMgr->RegisterDebugableWindow("TexturesMgr", this);
}

TextureMgr::~TextureMgr()
{
	gData.DebugMgr->UnregisterDebugableWindow("TexturesMgr");
}

void TextureMgr::Init()
{
	
}

void TextureMgr::Shut()
{
	m_textures.clear();
}

bool TextureMgr::ReadInt(const rapidxml::xml_node<>& parent, const char* name, int& out)
{
	const rapidxml::xml_node<>* child = parent.first_node(name);
	if (!child)
	{
		Logger::Warning(std::string("Missing <") + name + "> node");
		return false;
	}

	try
	{
		out = std::stoi(std::string(child->value(), child->value_size()));
	}
	catch (const std::exception&)
	{
		Logger::Warning(std::string("Invalid integer in <") + name + ">");
		return false;
	}
	return true;
}

bool TextureMgr::ReadBool(const rapidxml::xml_node<>& parent, const char* name, bool& out)
{
	int v = 0;
	if (!ReadInt(parent, name, v)) return false;
	out = (v != 0);
	return true;
}

bool TextureMgr::ReadName(const rapidxml::xml_node<>& node, std::string& out)
{
	const auto* attr = node.first_attribute("Name");
	if (!attr || attr->value_size() == 0)
	{
		Logger::Warning(std::string("<") + node.name() + "> without a Name attribute");
		return false;
	}
	out.assign(attr->value(), attr->value_size());
	return true;
}

bool TextureMgr::FileExists(const std::filesystem::path& path)
{
	return std::filesystem::is_regular_file(path) && std::filesystem::exists(path);
}

bool TextureMgr::LoadTexture(const std::filesystem::path& path)
{
	std::filesystem::path metaPath = path;
	metaPath.replace_extension(".xml");

	if (!FileExists(path))
	{
		Logger::Error("Texture file not found: " + path.string());
		return false;
	}
	if (!FileExists(metaPath))
	{
		Logger::Error("Metadata file not found: " + metaPath.string());
		return false;
	}

	TextureData data;
	if (!data.Texture.loadFromFile(path.string()))
	{
		Logger::Error("Failed to load texture: " + path.string());
		return false;
	}

	if (!LoadMetaData(metaPath, data))
		return false;

	const std::string key = path.stem().string();
	if (m_textures.count(key))
		Logger::Error("Texture " + key + " already loaded, replacing it");

	m_textures[key] = std::move(data);
	return true;
}

bool TextureMgr::LoadMetaData(const std::filesystem::path& metaPath, TextureData& out)
{
	try
	{
		rapidxml::file<> file(metaPath.string().c_str());
		rapidxml::xml_document<> doc;
		doc.parse<0>(file.data());

		const rapidxml::xml_node<>* animations = doc.first_node("Animations");
		const rapidxml::xml_node<>* backgrounds = doc.first_node("Backgrounds");

		if (!animations && !backgrounds)
		{
			Logger::Error("No <Animations> or <Backgrounds> root in " + metaPath.string());
			return false;
		}
		if (animations && !LoadAnimations(*animations, out))
			return false;
		if (backgrounds && !LoadBackgrounds(*backgrounds, out))
			return false;
	}
	catch (std::string& e)
	{
		Logger::Error(std::string("XML parse error in ") + metaPath.string() + ": " + e);
		return false;
	}
	
	return true;
}

bool TextureMgr::LoadAnimations(const rapidxml::xml_node<>& root, TextureData& out)
{
	for (const rapidxml::xml_node<>* n = root.first_node("Animation"); n; n = n->next_sibling("Animation"))
	{
		std::string name;
		AnimationData d;

		ReadName(*n, name);
		ReadInt(*n, "X", d.StartX);;
		ReadInt(*n, "Y", d.StartY);
		ReadInt(*n, "SizeX", d.SizeX);
		ReadInt(*n, "SizeY", d.SizeY);
		ReadInt(*n, "OffsetX", d.OffsetX);
		ReadInt(*n, "OffsetY", d.OffsetY);
		ReadInt(*n, "SpriteNum", d.AnimationSpriteCount);
		ReadInt(*n, "SpritesOnLine", d.SpriteOnLine);
		ReadBool(*n, "Reverted", d.IsReverted);

		if (!out.animationsData.emplace(name, d).second)
			Logger::Error("Duplicate animation name " + name + " keeping the first one");
	}
	return true;
}

bool TextureMgr::LoadBackgrounds(const rapidxml::xml_node<>& root, TextureData& out)
{
	for (const rapidxml::xml_node<>* n = root.first_node("Tile"); n; n = n->next_sibling("Tile"))
	{
		std::string name;
		StaticTileData d;

		ReadName(*n, name);
		ReadInt(*n, "X", d.StartX);
		ReadInt(*n, "Y", d.StartY);
		ReadInt(*n, "SizeX", d.SizeX);
		ReadInt(*n, "SizeY", d.SizeY);
		ReadBool(*n, "RevertedX", d.IsRevertedX);
		ReadBool(*n, "RevertedY", d.IsRevertedY);
		

		if (!out.staticTilesData.emplace(name, d).second)
			Logger::Error("Duplicate tile name " + name + " keeping the first one");
	}
	return true;
}

void TextureMgr::DrawDebug()
{
#ifdef _USE_IMGUI

	for (const auto& texture : m_textures)
	{
		const std::string& name = texture.first;
		const auto& data = texture.second;

		if (ImGui::BeginCombo(("##" + name).c_str(), name.c_str()))
		{
			if (ImGui::BeginCombo(("##" + name + "Stat").c_str(), "Stats"))
			{
				if (!data.animationsData.empty())
				{
					for (const auto& animationData : data.animationsData)
					{
						const std::string& animName = animationData.first;
						const auto& anim = animationData.second;

						if (ImGui::BeginTable("AnimationStats", 2))
						{
							ImGui::TableNextColumn();
							if (ImGui::TreeNode(animName.c_str()))
							{
								ImGui::TableNextColumn();
								ImGui::Text("StartX: %d", anim.StartX);
								ImGui::Text("StartY: %d", anim.StartY);
								ImGui::Text("SizeX: %d", anim.SizeX);
								ImGui::Text("SizeY: %d", anim.SizeY);
								ImGui::Text("OffsetX: %d", anim.OffsetX);
								ImGui::Text("OffsetY: %d", anim.OffsetY);
								ImGui::Text("AnimationSpriteCount: %d", anim.AnimationSpriteCount);
								ImGui::Text("SpriteOnLine: %d", anim.SpriteOnLine);
								ImGui::Text("IsReverted: %d", anim.IsReverted);
								ImGui::Text("TimeBetweenAnimationInS: %d", anim.TimeBetweenAnimationInS);


								ImGui::TreePop();
							}
							ImGui::EndTable();
						}
						
					}
				}
				else if (!data.staticTilesData.empty())
				{
					for (const auto& staticTileData : data.staticTilesData)
					{
						const std::string& tileName = staticTileData.first;
						const auto& tile = staticTileData.second;

						if (ImGui::BeginTable("TileStats", 2))
						{
							ImGui::TableNextColumn();
							if (ImGui::TreeNode(tileName.c_str(), "Tiles"))
							{
								ImGui::TableNextColumn();
								ImGui::Text("StartX: %d", tile.StartX);
								ImGui::Text("StartY: %d", tile.StartY);
								ImGui::Text("SizeX: %d", tile.SizeX);
								ImGui::Text("SizeY: %d", tile.SizeY);
								ImGui::Text("IsRevertedX: %d", tile.IsRevertedX);
								ImGui::Text("IsRevertedY: %d", tile.IsRevertedY);

								ImGui::TreePop();
							}
							ImGui::EndTable();
						}
						
					}
				}
				else
				{
					ImGui::Text("No stats available.");
				}

				ImGui::EndCombo();
			}

		
			if (ImGui::BeginCombo(("##" + name + "Visual").c_str(), "Visual"))
			{
				ImGui::Image(texture.second.Texture);

				ImGui::EndCombo();
			}

			ImGui::EndCombo();
		}
	}

#endif
}

const TextureData& TextureMgr::GetTextureData(const std::string& name) const
{
	auto it = m_textures.find(name);
	if (it == m_textures.end())
	{
		Logger::Error("Unknown texture " + name + " returning default");
		return m_defaultTexture;
	}
	return it->second;
}

TextureData::TextureData()
{
}

TextureData::~TextureData()
{
}
