#include "SceneMgr.h"

#include <Scene.h>
#include <Globals.h>

#include <Imgui/imgui.h>

void SceneMgr::Init()
{
	gData.DebugMgr->RegisterDebugableWindow("SceneMgr", this);
}

void SceneMgr::Start()
{
	if (ActiveScene)
	{
		ActiveScene->Start();
	}
}

void SceneMgr::Update(float deltaTime)
{
	if (ActiveScene)
	{
		ActiveScene->Update(deltaTime);
	}
}

void SceneMgr::Shut()
{
	for (const auto& it : Scenes)
	{
		Scene* s = it.second;
		s->Destroy();
		delete s;
	}
	Scenes.clear();

	gData.DebugMgr->UnregisterDebugableWindow("SceneMgr");
}

void SceneMgr::Draw(sf::RenderWindow& window) const
{
	if (!ActiveScene)
	{
		return;
	}

	ActiveScene->Draw(window);
}

Scene* SceneMgr::CreateScene(const std::string& sceneName)
{
	std::size_t hash = std::hash<std::string>{}(sceneName);
	const auto& it = Scenes.find(hash);
	if (it != Scenes.end())
	{
		return nullptr;
	}
	Scene* s = new Scene(sceneName);
	Scenes.emplace(hash, s);
	return s;
}

void SceneMgr::DeleteScene(Scene* s)
{
	const auto& it = Scenes.find(s->GetHash());
	if (it != Scenes.end())
	{
		Scenes.erase(it);
	}
}

Scene* SceneMgr::SetActiveScene(const std::string& sceneName)
{
	std::size_t hash = std::hash<std::string>{}(sceneName);
	const auto& it = Scenes.find(hash);
	if (it == Scenes.end())
	{
		return nullptr;
	}

	ActiveScene = it->second;
	return ActiveScene;
}

void SceneMgr::DrawDebug()
{
#ifdef _USE_IMGUI

	for (auto it : Scenes)
	{
		ImGui::PushID(it.second);

		ImVec4 color = it.second == ActiveScene ? ImVec4(0, 200, 0, 255) : ImVec4(255, 255, 255, 255);
		ImGui::PushStyleColor(ImGuiCol_Text, color);

		bool bTree = ImGui::TreeNode("%s (%lld)", it.second->GetName().c_str(), it.second->GetHash());
		ImGui::PopStyleColor();
		if (bTree)
		{
			it.second->DrawDebug();
			ImGui::TreePop();
		}

		ImGui::PopID();
	}

#endif
}
