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

const char* InputMgr::KeyToString(sf::Keyboard::Key key)
{
	static const char* names[] = {
		"A", "B" ,"C" ,
		"D" , "E" ,"F" , "G" ,
		"H" , "I" , "J" ," K" ,
		"L" ," M" , "N" ," O" ,
		"P" ,"Q" , "R" , "S" ,
		"T" , "U" , "V" , "W" ,
		"X" , "Y" , "Z" , "Num0" ,
		"Num1" , "Num2" , "Num3" , "Num4" ,
		"Num5" , "Num6" , "Num7" , "Num8" ,
		"Num9" , "Escape" , "LControl" , "LShift" ,
		"LAlt" , "LSystem" , "RControl" , "RShift" ,
		"RAlt" , "RSystem" , "Menu" , "LBracket" ,
		"RBracket" , "SemiColon" , "Comma" , "Period" ,
		"Quote" , "Slash" , "BackSlash" , "Tilde" ,
		"Equal" , "Dash" , "Space" , "Return" ,
		"BackSpace" , "Tab" , "PageUp" , "PageDown" ,
		"End" , "Home" , "Insert" , "Delete" ,
		"Add" , "Subtract" , "Multiply" , "Divide" ,
		"Left" , "Right" , "Up" , "Down" ,
		"Numpad0" , "Numpad1" , "Numpad2" , "Numpad3" ,
		"Numpad4" , "Numpad5" , "Numpad6" , "Numpad7" ,
		"Numpad8" , "Numpad9" , "F1" , "F2" ,
		"F3" , "F4" , "F5" , "F6" ,
		"F7" , "F8" , "F9" , "F10" ,
		"F11" , "F12" , "F13" , "F14" ,
		"F15" , "Pause"
	};

	static_assert(sizeof(names) / sizeof(names[0]) == sf::Keyboard::KeyCount,"Key name table out of sync with sf::Keyboard::Key");
	int idx = static_cast<int>(key);
	return (idx >= 0 && idx < sf::Keyboard::KeyCount) ? names[idx] : "Unknown";
}

void InputMgr::DrawDebug()
{
#ifdef _USE_IMGUI
	ImGui::Text("Inputs registered : %zu", InputActions.size());
	ImGui::Separator();

	ImGui::InputText("##newaction", DebugNewActionName, sizeof(DebugNewActionName));
	ImGui::SameLine();
	if (ImGui::Button("Create action") && DebugNewActionName[0] != '\0')
	{
		CreateInputAction(DebugNewActionName);
		DebugNewActionName[0] = '\0';
	}
	ImGui::Separator();

	std::string actionToRemove;
	if (ImGui::CollapsingHeader("Input Actions", ImGuiTreeNodeFlags_DefaultOpen))
	{
		for (const auto& pair : InputActions)
		{
			const InputAction& action = pair.second;

			ImGui::PushID(&action);
			bool open = ImGui::TreeNode("action", "%s  [value: %.2f]", action.GetName().c_str(), action.GetValue());
			ImGui::SameLine();
			if (ImGui::SmallButton("Remove")) actionToRemove = action.GetName();   

			if (open)
			{
				const auto& callbacks = action.GetCallbacks();
				ImGui::Text("Callbacks: %zu", callbacks.size());
				for (const auto& cb : callbacks) ImGui::BulletText("%s (owner: %p)", cb.DebugName.c_str(), cb.Owner);
				ImGui::TreePop();
			}
			ImGui::PopID();
		}
	}

	std::pair<sf::Keyboard::Key, std::string> bindingToRemove;
	bool removeBinding = false;

	if (ImGui::CollapsingHeader("Bindings", ImGuiTreeNodeFlags_DefaultOpen))
	{
		for (const auto& binding : KeyBindings)
		{
			ImGui::PushID(static_cast<int>(binding.first));
			ImGui::Text("%s", KeyToString(binding.first));
			ImGui::Indent();
			for (const InputAction* action : binding.second)
			{
				ImGui::PushID(action);
				ImGui::Text("-> %s", action->GetName().c_str());
				ImGui::SameLine();
				if (ImGui::SmallButton("Remove"))
				{
					bindingToRemove = { binding.first, action->GetName() };
					removeBinding = true;
				}
				ImGui::PopID();
			}
			ImGui::Unindent();
			ImGui::PopID();
		}
	}

	if (ImGui::CollapsingHeader("New binding", ImGuiTreeNodeFlags_DefaultOpen))
	{
		const int keyCount = static_cast<int>(sf::Keyboard::KeyCount);
		DebugSelectedKey = std::clamp(DebugSelectedKey, 0, keyCount - 1);
		if (ImGui::BeginCombo("Key", KeyToString(static_cast<sf::Keyboard::Key>(DebugSelectedKey))))
		{
			for (int i = 0; i < keyCount; ++i)
			{
				bool selected = (i == DebugSelectedKey);
				if (ImGui::Selectable(KeyToString(static_cast<sf::Keyboard::Key>(i)), selected))
					DebugSelectedKey = i;
				if (selected) ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		std::vector<const std::string*> names;
		for (const auto& pair : InputActions)
			names.push_back(&pair.second.GetName());

		if (names.empty())
		{
			ImGui::TextDisabled("No action available");
		}
		else
		{
			DebugSelectedAction = std::clamp(DebugSelectedAction, 0, (int)names.size() - 1);
			if (ImGui::BeginCombo("Action", names[DebugSelectedAction]->c_str()))
			{
				for (int i = 0; i < (int)names.size(); ++i)
				{
					bool selected = (i == DebugSelectedAction);
					if (ImGui::Selectable(names[i]->c_str(), selected))
						DebugSelectedAction = i;
					if (selected) ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}

			if (ImGui::Button("Bind"))
				BindKeyToInputAction(static_cast<sf::Keyboard::Key>(DebugSelectedKey),
					*names[DebugSelectedAction]);
		}
	}

	if (removeBinding)
		UnBindActionFromKey(bindingToRemove.first, bindingToRemove.second);
	if (!actionToRemove.empty())
		RemoveInputAction(actionToRemove);
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







