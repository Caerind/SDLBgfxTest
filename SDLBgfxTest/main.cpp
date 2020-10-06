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

	Sprite spriteA;
	spriteA.SetTexture(textureA);
	Sprite spriteA2;
	spriteA2.SetTexture(textureA);
	Matrix4f spriteATransform = Matrix4f::Translation(1.0f, 1.0f, 0.0f);

	Sprite spriteB;
	spriteB.SetTexture(textureB);
	Matrix4f spriteBTransform = Matrix4f::Translation(2.0f, 2.0f, 0.0f);

	Camera camera;
	camera.InitializePerspective(80.0f, F32(window.GetWidth()) / F32(window.GetHeight()), 0.1f, 100.0f);
	camera.InitializeView(Vector3f(0.0f, 1.0f, 5.0f), Vector3f(0.0f, 0.0f, 0.0f));

	// TODO : Fix me
	Frustum frustum = camera.CreateFrustum();
	printf("(0,0,0) %d==1\n", frustum.Contains(Vector3f::Zero()));
	printf("(0,0,10) %d==0\n", frustum.Contains(Vector3f(0, 0, 10)));

	I64 lastTime = bx::getHPCounter();
	const double frequency = double(bx::getHPFrequency());

	const bgfx::ViewId mainViewId = 0;
	const bgfx::ViewId imguiViewId = 250;

	EventSystem::AddKey("moveForward", Keyboard::Key::W, EventSystem::ButtonActionType::Hold);
	EventSystem::AddKey("moveLeft", Keyboard::Key::A, EventSystem::ButtonActionType::Hold);
	EventSystem::AddKey("moveBackward", Keyboard::Key::S, EventSystem::ButtonActionType::Hold);
	EventSystem::AddKey("moveRight", Keyboard::Key::D, EventSystem::ButtonActionType::Hold);
	EventSystem::AddKey("action", Keyboard::Key::E, EventSystem::ButtonActionType::Pressed);
	EventSystem::AddJoystickButton("jactionP1", 0, 0, EventSystem::ButtonActionType::Pressed);
	EventSystem::AddJoystickButton("jactionP2", 1, 0, EventSystem::ButtonActionType::Pressed);
	U32 toggleGraphStats = EventSystem::AddKey("toggleGraphStats", Keyboard::Key::F3, EventSystem::ButtonActionType::Pressed, static_cast<U32>(Keyboard::Modifier::Control));

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
		ImGuiIO& io = ImGui::GetIO();
		ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
		ImGuizmo::Manipulate(
			camera.GetViewMatrix().GetData(),
			camera.GetProjectionMatrix().GetData(),
			ImGuizmo::TRANSLATE,
			ImGuizmo::WORLD,
			spriteATransform.GetData()
		);
		ImGuiWrapper::EndFrame();
#endif // ENGINE_DEBUG

		if (EventSystem::IsKeyActive("action") || EventSystem::IsJoystickButtonActive("jactionP1"))
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

		Cameras::CameraUpdateOldFps(camera, dt);

		// Toggle debug stats
#ifdef ENGINE_DEBUG
		if (EventSystem::IsKeyActive(toggleGraphStats))
		{
			BgfxWrapper::ToggleDisplayStats();
		}
#endif // ENGINE_DEBUG

		// Render
		{
			bgfx::touch(mainViewId);

			// Display mouse pos
			bgfx::dbgTextClear();
			const Vector2i dbgMousePos = Mouse::GetPositionCurrentWindow();
			const Vector2i dbgMouseDeltaPos = Mouse::GetDeltaPosition();
			const I32 dbgMouseWheel = Mouse::GetWheel();
			bgfx::dbgTextPrintf(0, 0, 0x0f, "Mouse: (%d, %d) (%d, %d) (%d)", dbgMousePos.x, dbgMousePos.y, dbgMouseDeltaPos.x, dbgMouseDeltaPos.y, dbgMouseWheel);

			camera.Apply(mainViewId);

			spriteA.Render(mainViewId);

			bgfx::setTransform(spriteATransform.GetData());
			spriteA2.Render(mainViewId);

			bgfx::setTransform(spriteBTransform.GetData());
			spriteB.Render(mainViewId);

			bgfx::frame();
		}
	}

    return true;
}