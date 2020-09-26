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

	// Used by STB & ImGui : You should not use it
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
	I32 mLastScroll;
	bgfx::ViewId mViewID;

	friend void ImGui::PushFont(ImGui::Font::Enum font);
};

} // namespace NAMESPACE_NAME

#endif // ENGINE_IMGUI