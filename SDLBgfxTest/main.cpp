#include "EngineIntegration.hpp"

#include <bx/timer.h>

#include "Math/Matrix3.hpp"
#include "Math/Matrix4.hpp"

#include "Window/SDLWrapper.hpp"
#include "Window/Window.hpp"
#include "Window/Mouse.hpp"
#include "Window/Keyboard.hpp"
#include "Window/Controller.hpp"
#include "Window/EventSystem.hpp"

#include "Graphics/BgfxWrapper.hpp"
#include "Graphics/ImGuiWrapper.hpp"
#include "Graphics/Shader.hpp"
#include "Graphics/Sprite.hpp"
#include "Graphics/Texture.hpp"
#include "Graphics/Camera.hpp"
#include "Graphics/Tileset.hpp"
#include "Graphics/Tilemap.hpp"
#include "Graphics/DebugDraw.hpp"

#include "Game/Cameras.hpp"

using namespace NAMESPACE_NAME;

bool app(Window& window);

void printMtx3(const Matrix3f& m)
{
	printf("%f %f %f\n%f %f %f\n%f %f %f", m[0], m[1], m[2], m[3], m[4], m[5], m[6], m[7], m[8]);
}

void printMtx4(const Matrix4f& m)
{
	printf("%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n", m[0], m[1], m[2], m[3], m[4], m[5], m[6], m[7], m[8], m[9], m[10], m[11], m[12], m[13], m[14], m[15]);
}

int main(int argc, char** argv)
{
    if (!SDLWrapper::Init())
	{
		assert(false);
        return -1;
    }
    {
        Window window;
        if (!window.Create("SDL & bgfx", 800, 600))
        {
			SDLWrapper::Release();
			assert(false);
            return -1;
        }

        if (!BgfxWrapper::Init(window))
		{
            window.Close();
            window.Destroy();
			SDLWrapper::Release();
			assert(false);
			return -1;
        }
#ifdef ENGINE_IMGUI
        if (!ImGuiWrapper::Init())
		{
            window.Close();
			BgfxWrapper::Release();
			window.Destroy();
			SDLWrapper::Release();
			assert(false);
            return -1;
        }
#endif // ENGINE_IMGUI

		// This is the main function without all the init SDL/Window/Bgfx/ImGui
		app(window);

#ifdef ENGINE_IMGUI
        ImGuiWrapper::Release();
#endif // ENGINE_IMGUI
		BgfxWrapper::Release();
	}
	SDLWrapper::Release();
    return 0;
}

bool app(Window& window)
{
	printf("%d joysticks, %d haptics\n", Controller::GetJoystickCount(), Controller::GetHapticCount());

	for (U32 i = 0; i < Controller::GetJoystickCount(); ++i)
	{
		Controller::Rumble(i, 0.25f, 100); // Ensure the controller is working when debugging
		printf("%s name\n%d axes, %d balls, %d buttons, %d hats\n", Controller::GetName(i), Controller::GetAxisCount(i), Controller::GetBallCount(i), Controller::GetButtonCount(i), Controller::GetHatCount(i));
		printf("%d haptic\n", Controller::IsHaptic(i));
		printf("\n");
	}

	Texture textureA;
	const char* textureAFilename = "fieldstone-rgba.dds";
	if (!textureA.Initialize(textureAFilename))
	{
		assert(false);
	}

	Texture textureB;
	const char* textureBFilename = "ship_default.png";
	if (!textureB.Initialize(textureBFilename))
	{
		assert(false);
	}

	Sprite spriteA1;
	spriteA1.SetTexture(textureA);
	Matrix4f spriteA1Transform = Matrix4f::Translation(0.0f, 1.0f, 0.0f);
	Sprite spriteA2;
	spriteA2.SetTexture(textureA);
	Matrix4f spriteA2Transform = Matrix4f::Translation(1.0f, 2.0f, 0.0f);

	Sprite spriteB;
	spriteB.SetTexture(textureB);
	Matrix4f spriteBTransform = Matrix4f::Translation(2.0f, 2.0f, 0.0f);

	Tileset tileset;
	tileset.SetGridSize({ 2,2 });
	tileset.SetTileSize({ 256, 256 });
	tileset.SetTexture(textureA);
	Tilemap tilemap;
	tilemap.SetTileset(tileset);
	tilemap.SetSize({ 4,4 });
	tilemap.SetTile({ 1,1 }, 0);
	tilemap.SetTile({ 2,1 }, 1);
	tilemap.SetTile({ 2,2 }, 2);
	tilemap.SetTile({ 1,2 }, 3);
	Matrix4f tilemapTransform = Matrix4f::RotationX(90.0f);

	Camera camera;
	camera.InitializePerspective(80.0f, F32(window.GetWidth()) / F32(window.GetHeight()), 0.1f, 100.0f);
	camera.InitializeView(Vector3f(0.0f, 1.0f, 5.0f), Vector3f(0.0f, 0.0f, -1.0f));

	// TODO : Fix me
	Frustum frustum = camera.CreateFrustum();
	printf("(0,0,0) %d==1\n", frustum.Contains(Vector3f::Zero()));
	printf("(0,0,10) %d==0\n", frustum.Contains(Vector3f(0, 0, 10)));

	DebugDraw debugDraw;

	I64 lastTime = bx::getHPCounter();
	const double frequency = double(bx::getHPFrequency());

	const bgfx::ViewId mainViewId = 0;
	const bgfx::ViewId imguiViewId = 250;

	// Create button event using generic way 
	EventSystem::AddButton("moveForward", EventSystem::EventButton::Type::KeyboardKey, static_cast<U32>(Keyboard::Key::W), static_cast<U32>(Keyboard::Modifier::None), EventSystem::EventButton::ActionType::Hold);
	// Create button event using specific helpers
	EventSystem::AddKeyButton("moveLeft", Keyboard::Key::A, EventSystem::EventButton::ActionType::Hold);
	EventSystem::AddKeyButton("moveBackward", Keyboard::Key::S, EventSystem::EventButton::ActionType::Hold);
	EventSystem::AddKeyButton("moveRight", Keyboard::Key::D, EventSystem::EventButton::ActionType::Hold);
	EventSystem::AddKeyButton("action", Keyboard::Key::E, EventSystem::EventButton::ActionType::Pressed);
	EventSystem::AddJoystickButton("jactionP1", 0, 0, EventSystem::EventButton::ActionType::Pressed);
	EventSystem::AddJoystickButton("jactionP2", 1, 0, EventSystem::EventButton::ActionType::Pressed);
	// Keep the hash for faster lookups (and no error in the strings)
	const U32 toggleGraphStats = EventSystem::AddKeyButton("toggleGraphStats", Keyboard::Key::F3, EventSystem::EventButton::ActionType::Pressed, static_cast<U32>(Keyboard::Modifier::Control));

	while (!window.ShouldClose())
	{
		EventSystem::Update();

		if (EventSystem::ShouldClose())
		{
			window.Close();
		}

		// ImGui
#ifdef ENGINE_IMGUI
		ImGuiWrapper::BeginFrame(imguiViewId);
		ImGui::Begin("HeyImGui");
		ImGui::Text("HelloWorld");
		ImGui::End();
		ImGuiIO& io = ImGui::GetIO();
		ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
		ImGuizmo::Manipulate(
			camera.GetViewMatrix().GetData(),
			camera.GetProjectionMatrix().GetData(),
			ImGuizmo::TRANSLATE,
			ImGuizmo::WORLD,
			spriteA2Transform.GetData()
		);
		ImGuiWrapper::EndFrame();
#endif // ENGINE_DEBUG

		if (EventSystem::IsButtonActive("action") || EventSystem::IsButtonActive("jactionP1"))
		{
			printf("Action!\n");
			if (!Controller::Rumble(0, 0.25f, 100))
			{
				printf("Can't rumble : %s\n", SDLWrapper::GetError());
			}
		}
		
		for (U32 i = 0; i <= 1; ++i)
		{
			if (Controller::IsHatHold(i, 0, Controller::HatValue::Left))
			{
				printf("Left %d\n", i);
			}
			if (Controller::IsHatPressed(i, 0, Controller::HatValue::Right))
			{
				printf("Right %d\n", i);
			}
			if (Controller::IsHatReleased(i, 0, Controller::HatValue::Up))
			{
				printf("Up %d\n", i);
			}
			if (Controller::IsHatReleased(i, 0, Controller::HatValue::Down))
			{
				printf("Down %d\n", i);
			}
			if (Controller::IsButtonPressed(i, 0))
			{
				Controller::Rumble(i, 0.25f, 100);
			}
			if (Controller::IsButtonReleased(i, 1))
			{
				Controller::Rumble(i, 0.75f, 100);
			}
			if (Controller::IsButtonHold(i, 2))
			{
				Controller::Rumble(i, 0.5f, 100);
			}
			for (U32 j = 0; j < 6; ++j)
			{
				if (Controller::HasAxisMoved(i, j))
				{
					printf("C%d A%d moved : %f\n", i, j, Controller::GetAxis(i, j));
				}
			}
		}

		// Compute dt
		const I64 now = bx::getHPCounter();
		const I64 frameTime = now - lastTime;
		lastTime = now;
		const F32 dt = F32(frameTime / frequency);

		spriteBTransform *= Matrix4f::RotationY(180.0f * dt);

		Cameras::CameraUpdateMaya(camera, dt);

		// Toggle debug stats
#ifdef ENGINE_DEBUG
		if (EventSystem::IsButtonActive(toggleGraphStats))
		{
			BgfxWrapper::ToggleDisplayStats();
		}
#endif // ENGINE_DEBUG

		debugDraw.DrawLine(Vector3f::Zero(), camera.GetPosition() - Vector3f::UnitY());
		debugDraw.DrawCross(Vector3f::Zero());
		debugDraw.DrawBox({ 1.0f, 0.5f, 1.0f }, { 2.0f, 1.5f, 2.0f }, Colors::Red);

		// Render
		{
			bgfx::touch(mainViewId);

			// Display mouse pos
			bgfx::dbgTextClear();
			const Vector2i dbgMousePos = Mouse::GetPositionCurrentWindow();
			const Vector2i dbgMouseDeltaPos = Mouse::GetMouseMovement();
			const I32 dbgMouseWheel = Mouse::GetWheel();
			bgfx::dbgTextPrintf(0, 0, 0x0f, "Mouse: (%d, %d) (%d, %d) (%d)", dbgMousePos.x, dbgMousePos.y, dbgMouseDeltaPos.x, dbgMouseDeltaPos.y, dbgMouseWheel);

			camera.Apply(mainViewId);

			bgfx::setTransform(spriteA1Transform.GetData());
			spriteA1.Render(mainViewId);

			bgfx::setTransform(spriteA2Transform.GetData());
			spriteA2.Render(mainViewId);

			bgfx::setTransform(spriteBTransform.GetData());
			spriteB.Render(mainViewId);

			bgfx::setTransform(tilemapTransform.GetData());
			tilemap.Render(mainViewId);

			debugDraw.Render(mainViewId);

			bgfx::frame();
		}
	}

    return true;
}