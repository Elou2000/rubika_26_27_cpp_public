#pragma once

#include <functional>
#include <string>

class InputAction
{
public:
	InputAction() = default;
	InputAction(const std::string& name);
	void SetActionName(const std::string& name);
	void SetValue(float value);
	void RegisterCallback(const std::string& debugName, std::function<void(float)> callback, void* owner);
	void UnregisterCallbacks(void* owner);

private:
	struct sCallbackInfo
	{
		std::string DebugName;
		std::function<void(float)> Callback;
		void* Owner = nullptr;
	};

	std::string ActionName;
	std::size_t ActionNameHash;
	float Value = 0.f;
	container<sCallbackInfo> Callbacks
};