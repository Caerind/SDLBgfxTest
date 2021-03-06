#pragma once

#include "../EngineIntegration.hpp"

#ifdef ENGINE_IMGUI

#include <bx/allocator.h>

#include <bgfx/bgfx.h>

#include <dear-imgui/imgui.h>

#include <iconfontheaders/icons_kenney.h>
#include <iconfontheaders/icons_font_awesome.h>

namespace NAMESPACE_NAME
{

class ImGuiWrapper
{
public:
	static bool Init();
	static bool IsInitialized();
	static bool Release();

	static void BeginFrame(bgfx::ViewId viewId);
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
	bgfx::VertexLayout mVertexLayout;
	bgfx::ProgramHandle mProgram;
	bgfx::ProgramHandle mImageProgram;
	bgfx::TextureHandle mTexture;
	bgfx::UniformHandle mSamplerTexture;
	bgfx::UniformHandle mImageLodEnabled;
	ImFont* mFonts[ImGui::Font::Count];
	I64 mLast;
	bgfx::ViewId mViewId;

	friend void ImGui::PushFont(ImGui::Font::Enum font);
	friend class ImGuiWrapperAllocatorAccess;
};

} // namespace NAMESPACE_NAME

#define IMGUI_FLAGS_NONE UINT8_C(0x00)
#define IMGUI_FLAGS_ALPHA_BLEND UINT8_C(0x01)

namespace ImGui
{

	// TODO : Add ImGui helpers here

} // namespace ImGui

#endif // ENGINE_IMGUI