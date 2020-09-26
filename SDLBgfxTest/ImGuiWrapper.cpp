#include "ImGuiWrapper.hpp"

#ifdef ENGINE_IMGUI

#include <bx/math.h>
#include <bx/timer.h>

#include <bgfx/embedded_shader.h>

#include "fonts/roboto_regular.ttf.h"
#include "fonts/robotomono_regular.ttf.h"
#include "fonts/icons_kenney.ttf.h"
#include "fonts/icons_font_awesome.ttf.h"

#include "embedded_shaders/vs_ocornut_imgui.bin.h"
#include "embedded_shaders/fs_ocornut_imgui.bin.h"
#include "embedded_shaders/vs_imgui_image.bin.h"
#include "embedded_shaders/fs_imgui_image.bin.h"

namespace NAMESPACE_NAME
{

static const bgfx::EmbeddedShader s_embeddedShaders[] =
{
	BGFX_EMBEDDED_SHADER(vs_ocornut_imgui),
	BGFX_EMBEDDED_SHADER(fs_ocornut_imgui),
	BGFX_EMBEDDED_SHADER(vs_imgui_image),
	BGFX_EMBEDDED_SHADER(fs_imgui_image),

	BGFX_EMBEDDED_SHADER_END()
};

struct FontRangeMerge
{
	const void* data;
	size_t size;
	ImWchar ranges[3];
};

static FontRangeMerge s_fontRangeMerge[] =
{
	{ s_iconsKenneyTtf,      sizeof(s_iconsKenneyTtf),      { ICON_MIN_KI, ICON_MAX_KI, 0 } },
	{ s_iconsFontAwesomeTtf, sizeof(s_iconsFontAwesomeTtf), { ICON_MIN_FA, ICON_MAX_FA, 0 } },
};

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

	// Style
	const float fontSize = 18.0f;
	ImGuiStyle& style = ImGui::GetStyle();
	ImGui::StyleColorsDark(&style);
	style.FrameRounding = 4.0f;
	style.WindowBorderSize = 0.0f;

	// TODO : Keys
	/*
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
	//io.NavInputs[ImGuiNavInput_Input]       = (int)entry::Key::;
	//io.NavInputs[ImGuiNavInput_Menu]        = (int)entry::Key::;
	io.NavInputs[ImGuiNavInput_DpadLeft]    = (int)entry::Key::GamepadLeft;
	io.NavInputs[ImGuiNavInput_DpadRight]   = (int)entry::Key::GamepadRight;
	io.NavInputs[ImGuiNavInput_DpadUp]      = (int)entry::Key::GamepadUp;
	io.NavInputs[ImGuiNavInput_DpadDown]    = (int)entry::Key::GamepadDown;
	//io.NavInputs[ImGuiNavInput_LStickLeft]  = (int)entry::Key::;
	//io.NavInputs[ImGuiNavInput_LStickRight] = (int)entry::Key::;
	//io.NavInputs[ImGuiNavInput_LStickUp]    = (int)entry::Key::;
	//io.NavInputs[ImGuiNavInput_LStickDown]  = (int)entry::Key::;
	//io.NavInputs[ImGuiNavInput_FocusPrev]   = (int)entry::Key::;
	//io.NavInputs[ImGuiNavInput_FocusNext]   = (int)entry::Key::;
	//io.NavInputs[ImGuiNavInput_TweakSlow]   = (int)entry::Key::;
	//io.NavInputs[ImGuiNavInput_TweakFast]   = (int)entry::Key::;
	*/

	bgfx::RendererType::Enum type = bgfx::getRendererType();
	imgui.mProgram = bgfx::createProgram(
		bgfx::createEmbeddedShader(s_embeddedShaders, type, "vs_ocornut_imgui")
		, bgfx::createEmbeddedShader(s_embeddedShaders, type, "fs_ocornut_imgui")
		, true
	);
	imgui.mImageProgram = bgfx::createProgram(
		bgfx::createEmbeddedShader(s_embeddedShaders, type, "vs_imgui_image")
		, bgfx::createEmbeddedShader(s_embeddedShaders, type, "fs_imgui_image")
		, true
	);

	imgui.mVertexLayout
		.begin()
		.add(bgfx::Attrib::Position, 2, bgfx::AttribType::Float)
		.add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
		.add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
		.end();

	imgui.mImageLodEnabled = bgfx::createUniform("u_imageLodEnabled", bgfx::UniformType::Vec4);
	imgui.mSamplerTexture = bgfx::createUniform("s_tex", bgfx::UniformType::Sampler);

	uint8_t* data;
	int32_t width;
	int32_t height;
	{
		ImFontConfig config;
		config.FontDataOwnedByAtlas = false;
		config.MergeMode = false;

		const ImWchar* ranges = io.Fonts->GetGlyphRangesCyrillic();
		imgui.mFonts[ImGui::Font::Regular] = io.Fonts->AddFontFromMemoryTTF((void*)s_robotoRegularTtf, sizeof(s_robotoRegularTtf), fontSize, &config, ranges);
		imgui.mFonts[ImGui::Font::Mono] = io.Fonts->AddFontFromMemoryTTF((void*)s_robotoMonoRegularTtf, sizeof(s_robotoMonoRegularTtf), fontSize - 3.0f, &config, ranges);

		config.MergeMode = true;
		config.DstFont = imgui.mFonts[ImGui::Font::Regular];

		for (uint32_t ii = 0; ii < BX_COUNTOF(s_fontRangeMerge); ++ii)
		{
			const FontRangeMerge& frm = s_fontRangeMerge[ii];
			io.Fonts->AddFontFromMemoryTTF((void*)frm.data, (int)frm.size, fontSize - 3.0f, &config, frm.ranges);
		}
	}
	io.Fonts->GetTexDataAsRGBA32(&data, &width, &height);
	imgui.mTexture = bgfx::createTexture2D((uint16_t)width, (uint16_t)height, false, 1, bgfx::TextureFormat::BGRA8, 0, bgfx::copy(data, width * height * 4));

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

	ImGui::ShutdownDockContext();
	ImGui::DestroyContext(imgui.mContext);

	bgfx::destroy(imgui.mTexture);
	bgfx::destroy(imgui.mSamplerTexture);
	bgfx::destroy(imgui.mImageLodEnabled);
	bgfx::destroy(imgui.mProgram);
	bgfx::destroy(imgui.mImageProgram);

	imgui.mAllocator = nullptr;

	imgui.mInitialized = false;
    return true;
}

void ImGuiWrapper::BeginFrame()
{
	// TODO : Forward inputs
	const bgfx::ViewId viewID = 123;
	const char inputChar = -1;
	const float windowWidth = 800;
	const float windowHeight = 600;
	const float mousePosX = 400;
	const float mousePosY = 300;
	const bool mouseLeft = false;
	const bool mouseRight = false;
	const bool mouseMiddle = false;
	const int scroll = 10; // No idea what this value should look like yet
	const bool keyShift = false;
	const bool keyControl = false;
	const bool keyAlt = false;
	// + keys states

	ImGuiWrapper& imgui = GetInstance();
	assert(imgui.mInitialized);
    
	imgui.mViewID = viewID;

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
	io.MouseDown[0] = mouseLeft;
	io.MouseDown[1] = mouseRight;
	io.MouseDown[2] = mouseMiddle;
	io.MouseWheel = (float)(scroll - imgui.mLastScroll);
	imgui.mLastScroll = scroll;

	io.KeyShift = keyShift;
	io.KeyCtrl = keyControl;
	io.KeyAlt = keyAlt;

	// TODO : Keys
	/*
	for (int32_t ii = 0; ii < (int32_t)entry::Key::Count; ++ii)
	{
		io.KeysDown[ii] = inputGetKeyState(entry::Key::Enum(ii));
	}
	*/

	ImGui::NewFrame();
	ImGuizmo::BeginFrame();
}

void ImGuiWrapper::EndFrame()
{
	ImGuiWrapper& imgui = GetInstance();
	assert(imgui.mInitialized);

	ImGui::Render();

	const ImGuiIO& io = ImGui::GetIO();
	const float width = io.DisplaySize.x;
	const float height = io.DisplaySize.y;

	bgfx::setViewName(imgui.mViewID, "ImGui");
	bgfx::setViewMode(imgui.mViewID, bgfx::ViewMode::Sequential);

	const bgfx::Caps* caps = bgfx::getCaps();
	{
		float ortho[16];
		bx::mtxOrtho(ortho, 0.0f, width, height, 0.0f, 0.0f, 1000.0f, 0.0f, caps->homogeneousDepth);
		bgfx::setViewTransform(imgui.mViewID, NULL, ortho);
		bgfx::setViewRect(imgui.mViewID, 0, 0, uint16_t(width), uint16_t(height));
	}

	// Render command lists
	ImDrawData* drawData = ImGui::GetDrawData();
	for (int32_t ii = 0, num = drawData->CmdListsCount; ii < num; ++ii)
	{
		bgfx::TransientVertexBuffer tvb;
		bgfx::TransientIndexBuffer tib;

		const ImDrawList* drawList = drawData->CmdLists[ii];
		uint32_t numVertices = (uint32_t)drawList->VtxBuffer.size();
		uint32_t numIndices  = (uint32_t)drawList->IdxBuffer.size();

		// TODO : CheckAvailableTransientBuffers
		/*
		if (!checkAvailTransientBuffers(numVertices, m_layout, numIndices))
		{
			// Not enough space in transient buffer just quit drawing the rest...
			break;
		}
		*/

		bgfx::allocTransientVertexBuffer(&tvb, numVertices, imgui.mVertexLayout);
		bgfx::allocTransientIndexBuffer(&tib, numIndices);

		ImDrawVert* verts = (ImDrawVert*)tvb.data;
		bx::memCopy(verts, drawList->VtxBuffer.begin(), numVertices * sizeof(ImDrawVert));

		ImDrawIdx* indices = (ImDrawIdx*)tib.data;
		bx::memCopy(indices, drawList->IdxBuffer.begin(), numIndices * sizeof(ImDrawIdx));

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

				bgfx::TextureHandle th = imgui.mTexture;
				bgfx::ProgramHandle program = imgui.mProgram;

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
						bgfx::setUniform(imgui.mImageLodEnabled, lodEnabled);
						program = imgui.mImageProgram;
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
				bgfx::setTexture(0, imgui.mSamplerTexture , th);
				bgfx::setVertexBuffer(0, &tvb, 0, numVertices);
				bgfx::setIndexBuffer(&tib, offset, cmd->ElemCount);
				bgfx::submit(imgui.mViewID, program);
			}

			offset += cmd->ElemCount;
		}
	}
}

ImGuiWrapper& ImGuiWrapper::GetInstance()
{
    static ImGuiWrapper instance;
    return instance;
}

ImGuiWrapper::ImGuiWrapper()
    : mInitialized(false)
	// TODO : Initialize members
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

class ImGuiWrapperAllocatorAccess
{
public:
	static void* MemAlloc(std::size_t size, void* userData)
	{
		return ImGuiWrapper::GetInstance().MemAlloc(size, userData);
	}

	static void MemFree(void* ptr, void* userData)
	{
		ImGuiWrapper::GetInstance().MemFree(ptr, userData);
	}
};

} // namespace NAMESPACE_NAME

namespace ImGui
{

void PushFont(Font::Enum font)
{
	PushFont(NAMESPACE_NAME::ImGuiWrapper::GetInstance().mFonts[font]);
}

} // namespace ImGui

BX_PRAGMA_DIAGNOSTIC_IGNORED_MSVC(4505); // error C4505: '' : unreferenced local function has been removed
BX_PRAGMA_DIAGNOSTIC_IGNORED_CLANG_GCC("-Wunused-function"); // warning: 'int rect_width_compare(const void*, const void*)' defined but not used
BX_PRAGMA_DIAGNOSTIC_PUSH();
BX_PRAGMA_DIAGNOSTIC_IGNORED_CLANG("-Wunknown-pragmas")
BX_PRAGMA_DIAGNOSTIC_IGNORED_CLANG_GCC("-Wtype-limits"); // warning: comparison is always true due to limited range of data type
#define STBTT_malloc(size, userData) NAMESPACE_NAME::ImGuiWrapperAllocatorAccess::MemAlloc(size, userData)
#define STBTT_free(ptr, userData) NAMESPACE_NAME::ImGuiWrapperAllocatorAccess::MemFree(ptr, userData)
#define STB_RECT_PACK_IMPLEMENTATION
#include <stb/stb_rect_pack.h>
#define STB_TRUETYPE_IMPLEMENTATION
#include <stb/stb_truetype.h>
BX_PRAGMA_DIAGNOSTIC_POP();

#endif // ENGINE_IMGUI