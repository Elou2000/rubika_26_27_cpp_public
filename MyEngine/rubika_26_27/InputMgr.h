#pragma once
#include "DebugMgr.h"
#include <globals.h>
#include <InputAction.h>
#include "SFML/Window.hpp"
#ifdef _USE_IMGUI
#include <Imgui/imgui.h>
#include <Imgui/imgui-SFML.h>
#endif

class InputMgr :  IDebugable
{
public:
	InputMgr();
	~InputMgr();
	
	virtual void Start();
	virtual void Update(float fDeltaTime);
	virtual void Destroy();
	virtual void DrawDebug() override;

	void CreateInputAction(const std::string& name);
	void RemoveInputAction(const std::string& name);
	void RegisterToInputAction(const std::string& actionName, const std::string& debugName, std::function<void(float)> callback, void* owner);
	void UnRegisterToInputAction(const std::string& actionName, void* owner);

	void BindKeyToInputAction(sf::Keyboard::Key key, const std::string actionName);
	void UnBindActionFromKey(sf::Keyboard::Key key, const std::string& actionName);

private:
	
	std::unordered_map<std::size_t, InputAction> InputActions;
	std::unordered_map<sf::Keyboard::Key, std::vector<InputAction*>> KeyBindings;


};

