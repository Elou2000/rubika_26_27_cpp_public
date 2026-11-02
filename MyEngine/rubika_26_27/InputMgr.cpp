#include "InputMgr.h"
#include <algorithm>
#include <vector>
#include <unordered_map>


InputMgr::InputMgr()
{
	gData.DebugMgr->RegisterDebugableWindow("InputMgr", this);
}

InputMgr::~InputMgr()
{
	gData.DebugMgr->UnregisterDebugableWindow("InputMgr");
}

void InputMgr::Start()
{
	
}

void InputMgr::Update(float fDeltaTime)
{
	for (const auto& bindings : KeyBindings) 
	{
		const float value = sf::Keyboard::isKeyPressed(bindings.first) ? 1.f : 0.f;
		for (auto* inputAction : bindings.second)
		{
			inputAction->SetValue(value);
		}
			
	}
}

void InputMgr::Destroy()
{

}

void InputMgr::DrawDebug()
{
#ifdef _USE_IMGUI

	ImGui::Text("Inputs regustered : %zu", InputActions.size());
	ImGui::Separator();


#endif
}

void InputMgr::CreateInputAction(const std::string& name)
{
	size_t hash = std::hash<std::string>{}(name);
	InputActions.insert({ hash, InputAction(name) });
}

void InputMgr::RemoveInputAction(const std::string& name)
{
	size_t hash = std::hash<std::string>{}(name);
	auto it = InputActions.find(hash);
	if (it == InputActions.end()) return;

	InputAction* ptr = &it->second;
	for (auto& pair : KeyBindings)
	{
		auto& v = pair.second;
		v.erase(std::remove(v.begin(), v.end(), ptr), v.end());
	}
	InputActions.erase(it);
}

void InputMgr::RegisterToInputAction(const std::string& actionName, const std::string& debugName, std::function<void(float)> callback, void* owner)
{
	size_t hash = std::hash<std::string>{}(actionName);
	InputActions.at(hash).RegisterCallback(debugName, callback, owner);
}

void InputMgr::UnRegisterToInputAction(const std::string& actionName, void* owner)
{
	size_t hash = std::hash<std::string>{}(actionName);
	InputActions.at(hash).UnregisterCallbacks(owner);
}

void InputMgr::BindKeyToInputAction(sf::Keyboard::Key key, const std::string actionName)
{
	auto it = InputActions.find(std::hash<std::string>{}(actionName));
	if (it == InputActions.end()) return;

	auto& list = KeyBindings[key];  
	InputAction* ptr = &it->second;
	if (std::find(list.begin(), list.end(), ptr) == list.end())
	{
		list.push_back(ptr);
	}
		
}

void InputMgr::UnBindActionFromKey(sf::Keyboard::Key key, const std::string& actionName)
{
	size_t hash = std::hash<std::string>{}(actionName);

	auto it = InputActions.find(hash);
	if (it == InputActions.end()) return;

	auto keyIt = KeyBindings.find(key);
	if (keyIt == KeyBindings.end()) return;

	auto& input = keyIt->second;

	input.erase(std::remove(input.begin(), input.end(), &it->second),input.end());
	if (input.empty()) KeyBindings.erase(keyIt);
}







