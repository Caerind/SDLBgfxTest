#include "ImGuiWrapper.hpp"

#ifdef ENGINE_IMGUI

#include <bx/timer.h>

namespace NAMESPACE_NAME
{

bool ImGuiWrapper::Init()
{
    ImGuiWrapper& imgui = GetInstance();

    assert(!imgui.mInitialized);

	// Create ctx
	static bx::DefaultAllocator defaultAllocator;
	imgui.mAllocator = &defaultAllocator;

	imgui.mLastScroll = 0;
	imgui.mLast = bx::getHPCounter();
	imgui.mViewID = 255;

	ImGui::SetAllocatorFunctions(MemAlloc, MemFree, nullptr);
	imgui.mContext = ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();

	io.DisplaySize = ImVec2(1280.0f, 720.0f);
	io.DeltaTime = 1.0f / 60.0f;
	io.IniFilename = nullptr;

	// Custom style
	/*
	ImGuiStyle& style = ImGui::GetStyle();
	if (true)
	{
		ImGui::StyleColorsDark(&style);
	}
	else
	{
		ImGui::StyleColorsLight(&style);
	}
	style.FrameRounding = 4.0f;
	style.WindowBorderSize = 0.0f;
	*/


	/*
	#if USE_ENTRY
		io.KeyMap[ImGuiKey_Tab]        = (int)entry::Key::Tab;
		io.KeyMap[ImGuiKey_LeftArrow]  = (int)entry::Key::Left;
		io.KeyMap[ImGuiKey_RightArrow] = (int)entry::Key::Right;
		io.KeyMap[ImGuiKey_UpArrow]    = (int)entry::Key::Up;
		io.KeyMap[ImGuiKey_DownArrow]  = (int)entry::Key::Down;
		io.KeyMap[ImGuiKey_PageUp]     = (int)entry::Key::PageUp;
		io.KeyMap[ImGuiKey_PageDown]   = (int)entry::Key::PageDown;
		io.KeyMap[ImGuiKey_Home]       = (int)entry::Key::Home;
		io.KeyMap[ImGuiKey_End]        = (int)entry::Key::End;
		io.KeyMap[ImGuiKey_Insert]     = (int)entry::Key::Insert;
		io.KeyMap[ImGuiKey_Delete]     = (int)entry::Key::Delete;
		io.KeyMap[ImGuiKey_Backspace]  = (int)entry::Key::Backspace;
		io.KeyMap[ImGuiKey_Space]      = (int)entry::Key::Space;
		io.KeyMap[ImGuiKey_Enter]      = (int)entry::Key::Return;
		io.KeyMap[ImGuiKey_Escape]     = (int)entry::Key::Esc;
		io.KeyMap[ImGuiKey_A]          = (int)entry::Key::KeyA;
		io.KeyMap[ImGuiKey_C]          = (int)entry::Key::KeyC;
		io.KeyMap[ImGuiKey_V]          = (int)entry::Key::KeyV;
		io.KeyMap[ImGuiKey_X]          = (int)entry::Key::KeyX;
		io.KeyMap[ImGuiKey_Y]          = (int)entry::Key::KeyY;
		io.KeyMap[ImGuiKey_Z]          = (int)entry::Key::KeyZ;

		io.ConfigFlags |= 0
			| ImGuiConfigFlags_NavEnableGamepad
			| ImGuiConfigFlags_NavEnableKeyboard
			;

		io.NavInputs[ImGuiNavInput_Activate]    = (int)entry::Key::GamepadA;
		io.NavInputs[ImGuiNavInput_Cancel]      = (int)entry::Key::GamepadB;
//		io.NavInputs[ImGuiNavInput_Input]       = (int)entry::Key::;
//		io.NavInputs[ImGuiNavInput_Menu]        = (int)entry::Key::;
		io.NavInputs[ImGuiNavInput_DpadLeft]    = (int)entry::Key::GamepadLeft;
		io.NavInputs[ImGuiNavInput_DpadRight]   = (int)entry::Key::GamepadRight;
		io.NavInputs[ImGuiNavInput_DpadUp]      = (int)entry::Key::GamepadUp;
		io.NavInputs[ImGuiNavInput_DpadDown]    = (int)entry::Key::GamepadDown;
//		io.NavInputs[ImGuiNavInput_LStickLeft]  = (int)entry::Key::;
//		io.NavInputs[ImGuiNavInput_LStickRight] = (int)entry::Key::;
//		io.NavInputs[ImGuiNavInput_LStickUp]    = (int)entry::Key::;
//		io.NavInputs[ImGuiNavInput_LStickDown]  = (int)entry::Key::;
//		io.NavInputs[ImGuiNavInput_FocusPrev]   = (int)entry::Key::;
//		io.NavInputs[ImGuiNavInput_FocusNext]   = (int)entry::Key::;
//		io.NavInputs[ImGuiNavInput_TweakSlow]   = (int)entry::Key::;
//		io.NavInputs[ImGuiNavInput_TweakFast]   = (int)entry::Key::;
#endif // USE_ENTRY

		bgfx::RendererType::Enum type = bgfx::getRendererType();
		m_program = bgfx::createProgram(
			  bgfx::createEmbeddedShader(s_embeddedShaders, type, "vs_ocornut_imgui")
			, bgfx::createEmbeddedShader(s_embeddedShaders, type, "fs_ocornut_imgui")
			, true
			);

		u_imageLodEnabled = bgfx::createUniform("u_imageLodEnabled", bgfx::UniformType::Vec4);
		m_imageProgram = bgfx::createProgram(
			  bgfx::createEmbeddedShader(s_embeddedShaders, type, "vs_imgui_image")
			, bgfx::createEmbeddedShader(s_embeddedShaders, type, "fs_imgui_image")
			, true
			);

		m_layout
			.begin()
			.add(bgfx::Attrib::Position,  2, bgfx::AttribType::Float)
			.add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
			.add(bgfx::Attrib::Color0,    4, bgfx::AttribType::Uint8, true)
			.end();

		s_tex = bgfx::createUniform("s_tex", bgfx::UniformType::Sampler);

		uint8_t* data;
		int32_t width;
		int32_t height;
		{
			ImFontConfig config;
			config.FontDataOwnedByAtlas = false;
			config.MergeMode = false;
//			config.MergeGlyphCenterV = true;

			const ImWchar* ranges = io.Fonts->GetGlyphRangesCyrillic();
			m_font[ImGui::Font::Regular] = io.Fonts->AddFontFromMemoryTTF( (void*)s_robotoRegularTtf,     sizeof(s_robotoRegularTtf),     _fontSize,      &config, ranges);
			m_font[ImGui::Font::Mono   ] = io.Fonts->AddFontFromMemoryTTF( (void*)s_robotoMonoRegularTtf, sizeof(s_robotoMonoRegularTtf), _fontSize-3.0f, &config, ranges);

			config.MergeMode = true;
			config.DstFont   = m_font[ImGui::Font::Regular];

			for (uint32_t ii = 0; ii < BX_COUNTOF(s_fontRangeMerge); ++ii)
			{
				const FontRangeMerge& frm = s_fontRangeMerge[ii];

				io.Fonts->AddFontFromMemoryTTF( (void*)frm.data
						, (int)frm.size
						, _fontSize-3.0f
						, &config
						, frm.ranges
						);
			}
		}

		io.Fonts->GetTexDataAsRGBA32(&data, &width, &height);

		m_texture = bgfx::createTexture2D(
			  (uint16_t)width
			, (uint16_t)height
			, false
			, 1
			, bgfx::TextureFormat::BGRA8
			, 0
			, bgfx::copy(data, width*height*4)
			);
	*/

	ImGui::InitDockContext();

	imgui.mInitialized = true;
    return true;
}

bool ImGuiWrapper::IsInitialized()
{
    return GetInstance().mInitialized;
}

bool ImGuiWrapper::Release()
{
    ImGuiWrapper& imgui = GetInstance();

    assert(imgui.mInitialized);

    // Destroy ctx
	ImGui::ShutdownDockContext();
	ImGui::DestroyContext(imgui.mContext);

	/*
	bgfx::destroy(s_tex);
	bgfx::destroy(m_texture);

	bgfx::destroy(u_imageLodEnabled);
	bgfx::destroy(m_imageProgram);
	bgfx::destroy(m_program);
	*/

	imgui.mAllocator = nullptr;

	imgui.mInitialized = false;
    return true;
}

void ImGuiWrapper::BeginFrame()
{
	// inputs
	const char inputChar = -1;
	const float windowWidth = 800;
	const float windowHeight = 600;
	const float mousePosX = 400;
	const float mousePosY = 300;
	const bool mouseLeft = false;
	const bool mouseRight = false;
	const bool mouseMiddle = false;
	const int scroll = 10; // No idea what this value should look like yet

	ImGuiWrapper& imgui = GetInstance();
	assert(imgui.mInitialized);
    
	// beginFrame
	imgui.mViewID = 255;

	ImGuiIO& io = ImGui::GetIO();
	if (inputChar >= 0)
	{
		io.AddInputCharacter(inputChar);
	}
	io.DisplaySize = ImVec2(windowWidth, windowHeight);

	const I64 now = bx::getHPCounter();
	const I64 frameTime = now - imgui.mLast;
	imgui.mLast = now;
	const double freq = double(bx::getHPFrequency());
	io.DeltaTime = float(frameTime / freq);

	io.MousePos = ImVec2(mousePosX, mousePosY);
	io.MouseDown[0] = false;
	io.MouseDown[1] = false;
	io.MouseDown[2] = false;
	io.MouseWheel = (float)(scroll - imgui.mLastScroll);
	imgui.mLastScroll = scroll;

	ImGui::NewFrame();
	ImGuizmo::BeginFrame();
}

void ImGuiWrapper::EndFrame()
{
	ImGuiWrapper& imgui = GetInstance();
	assert(imgui.mInitialized);

    // endFrame
	ImGui::Render();

	/*
	const ImGuiIO& io = ImGui::GetIO();
		const float width  = io.DisplaySize.x;
		const float height = io.DisplaySize.y;

		bgfx::setViewName(m_viewId, "ImGui");
		bgfx::setViewMode(m_viewId, bgfx::ViewMode::Sequential);

		const bgfx::Caps* caps = bgfx::getCaps();
		{
			float ortho[16];
			bx::mtxOrtho(ortho, 0.0f, width, height, 0.0f, 0.0f, 1000.0f, 0.0f, caps->homogeneousDepth);
			bgfx::setViewTransform(m_viewId, NULL, ortho);
			bgfx::setViewRect(m_viewId, 0, 0, uint16_t(width), uint16_t(height) );
		}

		// Render command lists
		for (int32_t ii = 0, num = _drawData->CmdListsCount; ii < num; ++ii)
		{
			bgfx::TransientVertexBuffer tvb;
			bgfx::TransientIndexBuffer tib;

			const ImDrawList* drawList = _drawData->CmdLists[ii];
			uint32_t numVertices = (uint32_t)drawList->VtxBuffer.size();
			uint32_t numIndices  = (uint32_t)drawList->IdxBuffer.size();

			if (!checkAvailTransientBuffers(numVertices, m_layout, numIndices) )
			{
				// not enough space in transient buffer just quit drawing the rest...
				break;
			}

			bgfx::allocTransientVertexBuffer(&tvb, numVertices, m_layout);
			bgfx::allocTransientIndexBuffer(&tib, numIndices);

			ImDrawVert* verts = (ImDrawVert*)tvb.data;
			bx::memCopy(verts, drawList->VtxBuffer.begin(), numVertices * sizeof(ImDrawVert) );

			ImDrawIdx* indices = (ImDrawIdx*)tib.data;
			bx::memCopy(indices, drawList->IdxBuffer.begin(), numIndices * sizeof(ImDrawIdx) );

			uint32_t offset = 0;
			for (const ImDrawCmd* cmd = drawList->CmdBuffer.begin(), *cmdEnd = drawList->CmdBuffer.end(); cmd != cmdEnd; ++cmd)
			{
				if (cmd->UserCallback)
				{
					cmd->UserCallback(drawList, cmd);
				}
				else if (0 != cmd->ElemCount)
				{
					uint64_t state = 0
						| BGFX_STATE_WRITE_RGB
						| BGFX_STATE_WRITE_A
						| BGFX_STATE_MSAA
						;

					bgfx::TextureHandle th = m_texture;
					bgfx::ProgramHandle program = m_program;

					if (NULL != cmd->TextureId)
					{
						union { ImTextureID ptr; struct { bgfx::TextureHandle handle; uint8_t flags; uint8_t mip; } s; } texture = { cmd->TextureId };
						state |= 0 != (IMGUI_FLAGS_ALPHA_BLEND & texture.s.flags)
							? BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_SRC_ALPHA, BGFX_STATE_BLEND_INV_SRC_ALPHA)
							: BGFX_STATE_NONE
							;
						th = texture.s.handle;
						if (0 != texture.s.mip)
						{
							const float lodEnabled[4] = { float(texture.s.mip), 1.0f, 0.0f, 0.0f };
							bgfx::setUniform(u_imageLodEnabled, lodEnabled);
							program = m_imageProgram;
						}
					}
					else
					{
						state |= BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_SRC_ALPHA, BGFX_STATE_BLEND_INV_SRC_ALPHA);
					}

					const uint16_t xx = uint16_t(bx::max(cmd->ClipRect.x, 0.0f) );
					const uint16_t yy = uint16_t(bx::max(cmd->ClipRect.y, 0.0f) );
					bgfx::setScissor(xx, yy
						, uint16_t(bx::min(cmd->ClipRect.z, 65535.0f)-xx)
						, uint16_t(bx::min(cmd->ClipRect.w, 65535.0f)-yy)
						);

					bgfx::setState(state);
					bgfx::setTexture(0, s_tex, th);
					bgfx::setVertexBuffer(0, &tvb, 0, numVertices);
					bgfx::setIndexBuffer(&tib, offset, cmd->ElemCount);
					bgfx::submit(m_viewId, program);
				}

				offset += cmd->ElemCount;
			}
		}
	*/
}

ImGuiWrapper& ImGuiWrapper::GetInstance()
{
    static ImGuiWrapper instance;
    return instance;
}

ImGuiWrapper::ImGuiWrapper()
    : mInitialized(false)
{
}

ImGuiWrapper::~ImGuiWrapper()
{
    assert(!mInitialized);
}

void* ImGuiWrapper::MemAlloc(std::size_t size, void* userData)
{
	BX_UNUSED(userData);
	return BX_ALLOC(GetInstance().mAllocator, size);
}

void ImGuiWrapper::MemFree(void* ptr, void* userData)
{
	BX_UNUSED(userData);
	BX_FREE(GetInstance().mAllocator, ptr);
}

} // namespace NAMESPACE_NAME

#endif // ENGINE_IMGUI