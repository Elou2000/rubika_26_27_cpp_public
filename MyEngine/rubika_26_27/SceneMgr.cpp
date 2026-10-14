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
		if (sceneList.empty() || currentScene == nullptr)
		{
			ImGui::Text("No scenes available.");

			if (ImGui::Button("Add New Scene"))
			{
				AddNewScene(new Scene("Scene " + std::to_string(sceneList.size() + 1)));
			}

			return;
		}

		ImGui::TextColored(ImColor(255, 255, 255), "Active scene : %s", currentScene->sceneName.c_str());

		if (ImGui::Button("Previous Scene"))
		{
			if (sceneList.empty()) return;
			currentCursorPos = (currentCursorPos - 1 + sceneList.size()) % sceneList.size();
		}

		ImGui::SameLine();

		if (ImGui::Button("Next Scene"))
		{
			if (sceneList.empty()) return;
			currentCursorPos = (currentCursorPos + 1) % sceneList.size();
		}

		ImGui::SameLine();

		if (ImGui::Button("Select"))
		{
			if (sceneList.empty()) return;
			currentScene = sceneList[currentCursorPos];
		}

		ImGui::SameLine();

		if (ImGui::Button("Inspect"))
		{
			if (sceneList.empty()) return;
			currentInspectedScene = sceneList[currentCursorPos];
			currentCursorPos = 0;
			currentMode = InspectScene;
		
		}

		if (ImGui::Button("Add New Scene"))
		{
			AddNewScene(new Scene("Scene " + std::to_string(sceneList.size() + 1)));
		}

		for (auto scene : sceneList)
		{

			if (sceneList[currentCursorPos] == scene)
			{
				ImGui::TextColored(ImColor(255, 0, 0), scene->sceneName.c_str());
			}
			else
			{
				ImGui::Text(scene->sceneName.c_str());
			}

		}
	}
	else if (currentMode == InspectScene)
	{
		if (currentInspectedScene == nullptr) return;
		ImGui::TextColored(ImColor(255, 255, 255), "Currenly inspected scene : %s", currentInspectedScene->sceneName.c_str());

		if (ImGui::Button("Previous Entitie"))
		{
			if (currentInspectedScene->entityList.size() <= 0) return;
			currentCursorPos = (currentCursorPos - 1 + currentInspectedScene->entityList.size()) % currentInspectedScene->entityList.size();
		}

		ImGui::SameLine();

		if (ImGui::Button("Next Entities"))
		{
			if (currentInspectedScene->entityList.size() <= 0) return;
			currentCursorPos = (currentCursorPos + 1) % currentInspectedScene->entityList.size();
		}

		ImGui::SameLine();

		if (ImGui::Button("Back to navigate"))
		{
			currentCursorPos = 0;
			currentMode = Navigate;
			currentInspectedScene = nullptr;
		}

		ImGui::SameLine();

		if (ImGui::Button("Inspect Entity"))
		{
			auto& ents = currentInspectedScene->entityList;
			if (!ents.empty() && currentCursorPos < (int)ents.size())
			{
				currentInspectedEntity = ents[currentCursorPos];
				currentMode = InspectEntity;
			}
		}

		if (currentInspectedScene == nullptr) return;

		if (ImGui::Button("Add Entitie"))
		{
			std::string entityName = "Entity " + std::to_string(currentInspectedScene->entityList.size() + 1);
			currentInspectedScene->AddEntityToPool(new Entity(entityName));
		}
		ImGui::SameLine();

		if (ImGui::Button("Remove Entitie"))
		{
			auto& entities = currentInspectedScene->entityList;

			if (!entities.empty() && currentCursorPos < entities.size())
			{
				Entity* entity = entities[currentCursorPos];

				currentInspectedScene->RemoveEntityFromPool(entity);

				if (entities.empty() || currentCursorPos == 0)
				{
					currentCursorPos = 0;
				}
				else if (currentCursorPos >= entities.size())
				{
					currentCursorPos = entities.size() - 1;
				}
			}
		}

		auto& entities = currentInspectedScene->entityList;

		for (size_t i = 0; i < entities.size(); ++i)
		{
			Entity* entity = entities[i];

			if (i == currentCursorPos)
			{
				ImGui::TextColored(
					ImColor(255, 0, 0),
					"%s",
					entity->entityName.c_str()
				);
			}
			else
			{
				ImGui::Text("%s", entity->entityName.c_str());
			}
		}
	

	}
	else if (currentMode == InspectEntity)
	{
		if (currentInspectedEntity == nullptr)
		{
			currentMode = InspectScene;
			return;
		}

		ImGui::TextColored(ImColor(255, 255, 255), "Inspected entity : %s",
			currentInspectedEntity->entityName.c_str());

		if (ImGui::Button("Back to entities"))
		{
			currentInspectedEntity = nullptr;
			currentMode = InspectScene;
			return;
		}

		ImGui::Separator();

		if (currentInspectedEntity->components.empty())
			ImGui::Text("No components.");

		for (auto component : currentInspectedEntity->components)
		{
			ImGui::PushID(component); 
			if (ImGui::CollapsingHeader(component->GetName().c_str(),
				ImGuiTreeNodeFlags_DefaultOpen))
			{
				component->DrawDebug();
			}
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
