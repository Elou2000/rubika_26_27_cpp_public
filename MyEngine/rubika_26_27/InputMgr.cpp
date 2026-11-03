#include "InputMgr.h"
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <Logger.h>


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
	sf::Joystick::update();
	std::unordered_map<InputAction*, float> values;
	for (auto& [hash, action] : InputActions)
		values[&action] = 0.f;

	for (const auto& [key, actions] : KeyBindings)
		if (sf::Keyboard::isKeyPressed(key))
			for (auto* a : actions) values[a] = 1.f;

	for (const auto& b : JoystickBindings)
		if (sf::Joystick::isConnected(b.JoystickId) &&
			sf::Joystick::isButtonPressed(b.JoystickId, b.Button))
			values[b.Action] = 1.f;

	for (const auto& b : JoystickAxisBindings)
	{
		if (!sf::Joystick::isConnected(b.JoystickId) ||
			!sf::Joystick::hasAxis(b.JoystickId, b.Axis))
			continue;

		constexpr float Deadzone = 0.2f;

		float raw = sf::Joystick::getAxisPosition(b.JoystickId, b.Axis) / 100.f;
		float mag = std::abs(raw);
		if (mag < Deadzone) continue;

		float scaled = ((mag - Deadzone) / (1.f - Deadzone)) * (raw < 0.f ? -1.f : 1.f) * b.Direction;
		float v = std::max(0.f, scaled);
		if (v <= 0.f) continue;

		float& current = values[b.Action];
		if (std::abs(v) > std::abs(current)) current = v;
	}

	for (auto& [action, v] : values)
		action->SetValue(v);
}

void InputMgr::Destroy()
{

}

const char* InputMgr::KeyToString(sf::Keyboard::Key key)
{
	static const char* names[] = {
		"A", "B" ,"C" ,
		"D" , "E" ,"F" , "G" ,
		"H" , "I" , "J" ,"K" ,
		"L" ," M" , "N" ,"O" ,
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

			if (ImGui::Button("Bind"))BindKeyToInputAction(static_cast<sf::Keyboard::Key>(DebugSelectedKey),*names[DebugSelectedAction]);
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

	JoystickBindings.erase(std::remove_if(JoystickBindings.begin(), JoystickBindings.end(),[ptr](const JoystickBinding& b) 
		{ return b.Action == ptr; }),
		JoystickBindings.end());

	JoystickAxisBindings.erase(std::remove_if(JoystickAxisBindings.begin(), JoystickAxisBindings.end(),
		[ptr](const JoystickAxisBinding& b) { return b.Action == ptr; }), JoystickAxisBindings.end());
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

bool InputMgr::LoadInputs(const std::filesystem::path& path)
{
	std::filesystem::path metaPath = path;
	metaPath.replace_extension(".xml");

	if (!FileExists(metaPath))
	{
		Logger::Error("Metadata file not found: " + metaPath.string());
		return false;
	}


	if (!LoadInputAction(metaPath))
		return false;

	return true;
}

bool InputMgr::RegisterKey(const rapidxml::xml_node<>& parent, const char* name)
{
	const rapidxml::xml_node<>* child = parent.first_node(name);
	if (!child)
	{
		Logger::Warning(std::string("Missing <") + name + "> node");
		return false;
	}

	try
	{	
		int key = std::stoi(std::string(child->value(), child->value_size()));
		BindKeyToInputAction(static_cast<sf::Keyboard::Key>(key), parent.first_attribute("Name")->value());
		
	}
	catch (const std::exception&)
	{
		Logger::Warning(std::string("Invalid integer in <") + name + ">");
		return false;
	}
	return true;
}
bool InputMgr::RegisterJoystickButton(const rapidxml::xml_node<>& parent)
{
	const auto* node = parent.first_node("JoystickButton");

	if (!node)
		return false;

	try
	{
		unsigned int button = std::stoi(node->value());

		unsigned int joystickId = 0;

		if (auto* attr = node->first_attribute("JoystickId")) joystickId = std::stoi(attr->value());

		auto* name = parent.first_attribute("Name");

		if (!name) return false;

		BindJoystickButton(joystickId,button,name->value());
	}
	catch (...)
	{
		return false;
	}

	return true;
}

bool InputMgr::FileExists(const std::filesystem::path& path)
{
	return std::filesystem::is_regular_file(path) && std::filesystem::exists(path);
}
bool InputMgr::LoadInputAction(const std::filesystem::path& metaPath)
{
	try
	{
		rapidxml::file<> file(metaPath.string().c_str());
		rapidxml::xml_document<> doc;
		doc.parse<0>(file.data());

		const rapidxml::xml_node<>* root = doc.first_node("InputsData");
		if (!root)
		{
			Logger::Error("No <InputsData> root in " + metaPath.string());
			return false;
		}

		if (!LoadInputKey(*root)) return false;
	}
	catch (std::string& e)
	{
		Logger::Error(std::string("XML parse error in ") + metaPath.string() + ": " + e);
		return false;
	}

	return true;
}

bool InputMgr::LoadInputKey(const rapidxml::xml_node<>& root)
{
	for (const rapidxml::xml_node<>* n = root.first_node("InputAction"); n; n = n->next_sibling("InputAction"))
	{
		RegisterKey(*n, "Key1");
		RegisterKey(*n, "Key2");
		RegisterJoystickButton(*n);
		RegisterJoystickAxis(*n);
	}
	return true;
}


void InputMgr::BindJoystickButton(unsigned int joystickId,unsigned int button,const std::string& actionName)
{
	auto it = InputActions.find(std::hash<std::string>{}(actionName));

	if (it == InputActions.end())
		return;

	InputAction* action = &it->second;

	auto existing = std::find_if(JoystickBindings.begin(),JoystickBindings.end(),[&](const JoystickBinding& binding)
		{
			return binding.JoystickId == joystickId && binding.Button == button &&binding.Action == action;
		}
	);

	if (existing == JoystickBindings.end())
	{
		JoystickBindings.push_back({joystickId,button,action});
	}
}

void InputMgr::UnBindJoystickButton(unsigned int joystickId, unsigned int button, const std::string& actionName)
{
}

InputAction* InputMgr::FindAction(const std::string& name)
{
	auto it = InputActions.find(std::hash<std::string>{}(name));
	return it != InputActions.end() ? &it->second : nullptr;
}

bool InputMgr::AxisFromString(const std::string& s, sf::Joystick::Axis& out)
{
	static const std::unordered_map<std::string, sf::Joystick::Axis> map = {
		{"X", sf::Joystick::Axis::X}, {"Y", sf::Joystick::Axis::Y}, {"Z", sf::Joystick::Axis::Z},
		{"R", sf::Joystick::Axis::R}, {"U", sf::Joystick::Axis::U}, {"V", sf::Joystick::Axis::V},
		{"PovX", sf::Joystick::Axis::PovX}, {"PovY", sf::Joystick::Axis::PovY}
	};
	auto it = map.find(s);
	if (it == map.end()) return false;
	out = it->second;
	return true;
}


void InputMgr::BindJoystickAxis(unsigned int joystickId, sf::Joystick::Axis axis, float direction, const std::string& actionName)
{
	InputAction* action = FindAction(actionName);
	if (!action) return;

	JoystickAxisBindings.push_back({ joystickId, axis, direction, action });
}


bool InputMgr::RegisterJoystickAxis(const rapidxml::xml_node<>& parent)
{
	auto* name = parent.first_attribute("Name");
	if (!name) return false;

	for (const auto* a = parent.first_node("JoystickAxis"); a; a = a->next_sibling("JoystickAxis"))
	{
		unsigned int joystickId = 0;
		float direction = 1.f;
		sf::Joystick::Axis axis;

		try
		{
			if (auto* at = a->first_attribute("JoystickId")) joystickId = std::stoul(at->value());
			if (auto* at = a->first_attribute("Direction"))  direction = std::stoi(at->value()) < 0 ? -1.f : 1.f;
		}
		catch (const std::exception&)
		{
			Logger::Warning("Invalid <JoystickAxis> attribute");
			continue;
		}

		auto* axisAttr = a->first_attribute("Axis");
		if (!axisAttr || !AxisFromString(axisAttr->value(), axis))
		{
			Logger::Warning("Unknown or missing Axis in <JoystickAxis>");
			continue;
		}

		BindJoystickAxis(joystickId, axis, direction, name->value());
	}
	return true;
}




