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
	for (auto bindings : KeyBindings)
	{
		if (sf::Keyboard::isKeyPressed(bindings.first))
		{
			for (auto inputAction : bindings.second)
			{
				inputAction->SetValue(1);
			}
		}
		else
		{
			for (auto inputAction : bindings.second)
			{
				inputAction->SetValue(0);
			}
		}
	}
	
}

void InputMgr::Destroy()
{

}

namespace
{
	const std::vector<std::pair<std::string, sf::Keyboard::Key>>& GetDebugKeys()
	{
		static std::vector<std::pair<std::string, sf::Keyboard::Key>> keys;
		if (keys.empty())
		{
			for (int i = 0; i < 26; ++i)
				keys.push_back({ std::string(1, char('A' + i)), sf::Keyboard::Key(sf::Keyboard::A + i) });
			for (int i = 0; i < 10; ++i)
				keys.push_back({ "Num" + std::to_string(i), sf::Keyboard::Key(sf::Keyboard::Num0 + i) });
			keys.push_back({ "Space",  sf::Keyboard::Space });
			keys.push_back({ "Enter",  sf::Keyboard::Enter });
			keys.push_back({ "Escape", sf::Keyboard::Escape });
			keys.push_back({ "Tab",    sf::Keyboard::Tab });
			keys.push_back({ "LShift", sf::Keyboard::LShift });
			keys.push_back({ "LCtrl",  sf::Keyboard::LControl });
			keys.push_back({ "Left",   sf::Keyboard::Left });
			keys.push_back({ "Right",  sf::Keyboard::Right });
			keys.push_back({ "Up",     sf::Keyboard::Up });
			keys.push_back({ "Down",   sf::Keyboard::Down });
		}
		return keys;
	}

	const char* KeyName(sf::Keyboard::Key key)
	{
		for (const auto& k : GetDebugKeys())
			if (k.second == key) return k.first.c_str();
		return "Unknown";
	}
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
	InputActions.erase(hash);
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
	std::vector<InputAction*> action;
	size_t hash = std::hash<std::string>{}(actionName);
	auto it = InputActions.find(hash);

	if (KeyBindings.find(key) == KeyBindings.end())
	{

		if (it != InputActions.end()) {

			action.push_back(&(it->second));
			KeyBindings.insert({ key, action });
		}
	
	}
	else
	{
		if (it != InputActions.end()) {

			KeyBindings.at(key).push_back(&(it->second));
		}
		
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
}







