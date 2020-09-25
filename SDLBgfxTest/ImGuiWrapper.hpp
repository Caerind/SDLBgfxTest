#pragma once

#include "EngineIntegration.hpp"

#ifdef ENGINE_IMGUI

#include <bx/allocator.h>
#include <bgfx/bgfx.h>
#include <dear-imgui/imgui.h>

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
	static void* MemAlloc(std::size_t size, void* userData);
	static void MemFree(void* ptr, void* userData);

private:
	static ImGuiWrapper& GetInstance();

	ImGuiWrapper();
	~ImGuiWrapper();

	bool mInitialized;
	ImGuiContext* mContext;
	bx::AllocatorI* mAllocator;
	I64 mLast;
	I32 mLastScroll;
	bgfx::ViewId mViewID;
};

} // namespace NAMESPACE_NAME

#endif // ENGINE_IMGUI