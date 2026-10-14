#include "SceneMgr.h"
#include "Logger.h"
#include "Globals.h"

#ifdef _USE_IMGUI
#include <Imgui/imgui.h>
#endif

SceneMgr::SceneMgr()
{
	gData.DebugMgr->RegisterDebugableWindow("SceneMgr", this);
}

SceneMgr::~SceneMgr()
{
	gData.DebugMgr->UnregisterDebugableWindow("SceneMgr");
}

void SceneMgr::Start()
{
	if (currentScene == nullptr) return;
	currentScene->Start();
}

void SceneMgr::Update(float dt)
{
	if (currentScene == nullptr) return;
	currentScene->Update(dt);
}

void SceneMgr::Destroy()
{
	for (auto scene : sceneList)
	{
		scene->Destroy();
	}
}

void SceneMgr::AddNewScene(Scene* newScene)
{
	for (auto scene : sceneList)
	{
		if (scene->sceneName == newScene->sceneName)
		{
			Logger::Warning("Scene Name Already Exists");
			return;
		}
	}

	sceneList.push_back(newScene);
	if (currentScene == nullptr)  currentScene = newScene;
}

void SceneMgr::SelectScene(std::string sceneName)
{
	for (auto scene : sceneList)
	{
		if (scene->sceneName == sceneName)
		{
			currentScene = scene;
			return;
		}
	}
	Logger::Warning("No scene with this name in the sceneList");
}

void SceneMgr::DrawDebug()
{
#ifdef _USE_IMGUI

	if (currentMode == Navigate)
	{
		ImGui::Text("Scenes");
		ImGui::Separator();

		if (sceneList.empty() || currentScene == nullptr)
		{
			ImGui::TextDisabled("No scenes available.");
			if (ImGui::Button("Add New Scene")) AddNewScene(new Scene("Scene " + std::to_string(sceneList.size() + 1)));
			return;
		}

		ImGui::Text("Active : %s", currentScene->sceneName.c_str());
		ImGui::Spacing();

		if (ImGui::Button("Previous"))
			currentCursorPos = (currentCursorPos - 1 + sceneList.size()) % sceneList.size();
		ImGui::SameLine();
		if (ImGui::Button("Next"))
			currentCursorPos = (currentCursorPos + 1) % sceneList.size();
		ImGui::SameLine();
		if (ImGui::Button("Select"))
			currentScene = sceneList[currentCursorPos];
		ImGui::SameLine();
		if (ImGui::Button("Inspect"))
		{
			currentInspectedScene = sceneList[currentCursorPos];
			currentCursorPos = 0;
			currentMode = InspectScene;
			return;
		}

		if (ImGui::Button("Add New Scene")) AddNewScene(new Scene("Scene " + std::to_string(sceneList.size() + 1)));

		ImGui::Separator();

		for (size_t i = 0; i < sceneList.size(); ++i)
		{
			Scene* scene = sceneList[i];
			std::string label = scene->sceneName + (scene == currentScene ? "  (active)" : "");

			ImGui::PushID((int)i);
			if (ImGui::Selectable(label.c_str(), (int)i == currentCursorPos))
				currentCursorPos = (int)i;
			ImGui::PopID();
		}
	}
	else if (currentMode == InspectScene)
	{
		if (currentInspectedScene == nullptr) return;

		ImGui::Text("Scenes > %s", currentInspectedScene->sceneName.c_str());
		ImGui::Separator();

		auto& entities = currentInspectedScene->entityList;

		if (ImGui::Button("Previous") && !entities.empty()) currentCursorPos = (currentCursorPos - 1 + entities.size()) % entities.size();
		ImGui::SameLine();
		if (ImGui::Button("Next") && !entities.empty()) currentCursorPos = (currentCursorPos + 1) % entities.size();
		ImGui::SameLine();
		if (ImGui::Button("Inspect"))
		{
			if (!entities.empty() && currentCursorPos < (int)entities.size())
			{
				currentInspectedEntity = entities[currentCursorPos];
				currentMode = InspectEntity;
				return;
			}
		}
		ImGui::SameLine();
		if (ImGui::Button("Back"))
		{
			currentCursorPos = 0;
			currentMode = Navigate;
			currentInspectedScene = nullptr;
			return;
		}

		if (ImGui::Button("Add Entity")) currentInspectedScene->AddEntityToPool(new Entity("Entity " + std::to_string(entities.size() + 1)));
		ImGui::SameLine();
		if (ImGui::Button("Remove Entity"))
		{
			if (!entities.empty() && currentCursorPos < (int)entities.size())
			{
				currentInspectedScene->RemoveEntityFromPool(entities[currentCursorPos]);

				if (currentCursorPos >= (int)entities.size())
					currentCursorPos = entities.empty() ? 0 : (int)entities.size() - 1;
			}
		}

		ImGui::Separator();

		if (entities.empty()) ImGui::TextDisabled("No entities.");

		for (size_t i = 0; i < entities.size(); ++i)
		{
			ImGui::PushID((int)i);
			if (ImGui::Selectable(entities[i]->entityName.c_str(), (int)i == currentCursorPos))
				currentCursorPos = (int)i;
			ImGui::PopID();
		}
	}
	else if (currentMode == InspectEntity)
	{
		if (currentInspectedEntity == nullptr)
		{
			currentMode = InspectScene;
			return;
		}

		ImGui::Text("Scenes > %s > %s",
			currentInspectedScene ? currentInspectedScene->sceneName.c_str() : "?",
			currentInspectedEntity->entityName.c_str());
		ImGui::Separator();

		if (ImGui::Button("Back"))
		{
			currentInspectedEntity = nullptr;
			currentMode = InspectScene;
			return;
		}

		ImGui::Spacing();

		if (currentInspectedEntity->components.empty())
			ImGui::TextDisabled("No components.");

		for (auto component : currentInspectedEntity->components)
		{
			ImGui::PushID(component);
			if (ImGui::CollapsingHeader(component->GetName().c_str(), ImGuiTreeNodeFlags_DefaultOpen))
				component->DrawDebug();
			ImGui::PopID();
		}
	}

#endif
}

void SceneMgr::Draw(sf::RenderWindow& window)
{
	if (currentScene != nullptr)
	{
		currentScene->Draw(window);
	}
}
