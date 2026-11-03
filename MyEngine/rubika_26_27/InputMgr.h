#pragma once
#include "DebugMgr.h"
#include <globals.h>
#include <InputAction.h>
#include "SFML/Window.hpp"
#include <filesystem>
#include <string>
#include <unordered_map>
#include <rapidxml/rapidxml.hpp>
#include <rapidxml/rapidxml_utils.hpp>

#ifdef _USE_IMGUI
#include <Imgui/imgui.h>
#include <Imgui/imgui-SFML.h>
#endif

class InputMgr :  IDebugable
{
public:
	InputMgr();
	~InputMgr();
	
	struct JoystickBinding
	{
		unsigned int JoystickId;
		unsigned int Button;
		InputAction* Action;
	};

	struct JoystickAxisBinding
	{
		unsigned int JoystickId;
		sf::Joystick::Axis Axis;
		float Direction; 
		InputAction* Action;
	};



	virtual void Start();
	virtual void Update(float fDeltaTime);
	virtual void Destroy();
	virtual void DrawDebug() override;

	void CreateInputAction(const std::string& name);
	void RemoveInputAction(const std::string& name);
	void RegisterToInputAction(const std::string& actionName, const std::string& debugName, std::function<void(float)> callback, void* owner);
	void UnRegisterToInputAction(const std::string& actionName, void* owner);

	bool LoadInputs(const std::filesystem::path& path);
	bool RegisterKey(const rapidxml::xml_node<>& parent, const char* name);
	bool RegisterJoystickButton(const rapidxml::xml_node<>& parent);
	bool FileExists(const std::filesystem::path& path);
	bool LoadInputAction(const std::filesystem::path& metaPath);
	bool LoadInputKey(const rapidxml::xml_node<>& root);

	void BindKeyToInputAction(sf::Keyboard::Key key, const std::string actionName);
	void UnBindActionFromKey(sf::Keyboard::Key key, const std::string& actionName);
	void BindJoystickButton(unsigned int joystickId, unsigned int button, const std::string& actionName);
	void UnBindJoystickButton(unsigned int joystickId, unsigned int button, const std::string& actionName);

	void BindJoystickAxis(unsigned int joystickId, sf::Joystick::Axis axis, float direction, const std::string& actionName);
	InputAction* FindAction(const std::string& name);
	static bool AxisFromString(const std::string& s, sf::Joystick::Axis& out);
	bool RegisterJoystickAxis(const rapidxml::xml_node<>& parent);

private:
	static const char* KeyToString(sf::Keyboard::Key key);


	int DebugSelectedKey = 0;
	int DebugSelectedAction = 0;
	char DebugNewActionName[64] = "";
	
	std::unordered_map<std::size_t, InputAction> InputActions;
	std::unordered_map<sf::Keyboard::Key, std::vector<InputAction*>> KeyBindings;
	std::vector<JoystickBinding> JoystickBindings;
	std::vector<JoystickAxisBinding> JoystickAxisBindings;

	
	

	


};

