#pragma once

#include <DebugMgr.h>

#include <string>
#include <vector>

class Logger : public IDebugable
{
public:
	void Init();
	void Shut();

	static void Info(const std::string& message);
	static void Warning(const std::string& message);
	static void Error(const std::string& message);

	virtual void DrawDebug() override;

protected:

	enum class eMessageType
	{
		Info,
		Warning,
		Error
	};

	struct sMessageInfo
	{
		// DateTime
		eMessageType Type;
		std::string Message;
	};

	void PushNewMessage(const sMessageInfo& message);
	std::vector<sMessageInfo> Messages;

	bool DisplayErrorMessage = true;
	bool DisplayWarningMessage = true;
	bool DisplayInfoMessage = true;
};