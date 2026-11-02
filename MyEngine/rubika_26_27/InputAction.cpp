#include "InputAction.h"

InputAction::InputAction(const std::string& name)
{
	SetActionName(name);
}

void InputAction::SetActionName(const std::string& name)
{
	ActionName = name;
	ActionNameHash = name.size();
}

void InputAction::SetValue(float value)
{
	Value = value;
	for (auto callback : Callbacks)
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
	for (int i = 0; i < Callbacks.size(); i++) 
	{
		if (Callbacks[i].Owner == owner)
		{
			Callbacks.erase(Callbacks.begin() + i);
		}
	}
}
