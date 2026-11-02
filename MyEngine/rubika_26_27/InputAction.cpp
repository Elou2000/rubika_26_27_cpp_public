#include "InputAction.h"

InputAction::InputAction(const std::string& name)
{
	SetActionName(name);
}

void InputAction::SetActionName(const std::string& name)
{
	ActionName = name;
	ActionNameHash = std::hash<std::string>{}(name);  
}

void InputAction::SetValue(float value)
{
	Value = value;
	for (const auto& callback : Callbacks)    
	{
		callback.Callback(Value);
	}
}

void InputAction::RegisterCallback(const std::string& debugName, std::function<void(float)> callback, void* owner)
{
	sCallbackInfo info = { debugName, callback, owner };
	Callbacks.push_back(info);

}

void InputAction::UnregisterCallbacks(void* owner)
{
	Callbacks.erase(std::remove_if(Callbacks.begin(), Callbacks.end(),[owner](const sCallbackInfo& info)
		{ return info.Owner == owner; }),
		Callbacks.end());
}
