#pragma once

#include "EngineIntegration.hpp"

#ifdef ENGINE_IMGUI

namespace NAMESPACE_NAME
{

class ImGuiWrapper
{
public:
	static bool Init();
	static bool IsInitialized();
	static bool Release();

	static void BeginFrame();
	static void EndFrame();

private:
	static ImGuiWrapper& GetInstance();

	ImGuiWrapper();
	~ImGuiWrapper();

	bool mInitialized;
};

} // namespace NAMESPACE_NAME

#endif // ENGINE_IMGUI