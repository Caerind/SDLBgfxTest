#include "../EngineIntegration.hpp"

#include "../Math/Matrix3.hpp"
#include "../Math/Matrix4.hpp"

#include "../Window/Mouse.hpp"
#include "../Window/Keyboard.hpp"
#include "../Window/EventSystem.hpp"

#include "../Graphics/Camera.hpp"

namespace NAMESPACE_NAME
{

class Cameras
{
public:
	static void CameraUpdateTotalWar(Camera& camera, F32 deltaTime)
	{

		Vector3f movement;
		bool moved = false;

		const F32 cameraSpeedYWheel = 1.0f;
		if (Mouse::GetWheel() != 0)
		{
			movement.y += cameraSpeedYWheel * Mouse::GetWheel();
			moved = true;
		}

		const F32 cameraSpeedZ = 2.0f;
		const F32 mvtZ = deltaTime * cameraSpeedZ;
		if (Keyboard::IsHold(Keyboard::Key::Up))
		{
			movement.z -= mvtZ;
			moved = true;
		}
		if (Keyboard::IsHold(Keyboard::Key::Down))
		{
			movement.z += mvtZ;
			moved = true;
		}

		const F32 cameraSpeedX = 3.0f;
		const F32 mvtX = deltaTime * cameraSpeedX;
		if (Keyboard::IsHold(Keyboard::Key::Right))
		{
			movement.x += mvtX;
			moved = true;
		}
		if (Keyboard::IsHold(Keyboard::Key::Left))
		{
			movement.x -= mvtX;
			moved = true;
		}

		if (moved)
		{
			camera.Move(movement);
		}
	}

	static void CameraUpdateOldFps(Camera& camera, F32 deltaTime)
	{
		Vector3f direction = camera.GetDirection();

		Vector3f movement;
		bool moved = false;
		if (Keyboard::IsHold(Keyboard::Key::Up))
		{
			movement += 2.0f * direction * deltaTime;
			moved = true;
		}
		if (Keyboard::IsHold(Keyboard::Key::Down))
		{
			movement -= 2.0f * direction * deltaTime;
			moved = true;
		}
		if (moved)
		{
			camera.Move(movement);
		}

		bool rotated = false;
		if (Keyboard::IsHold(Keyboard::Key::Right))
		{
			direction = Matrix3f::RotationY(90.0f * deltaTime).TransformDirection(direction);
			rotated = true;
		}
		if (Keyboard::IsHold(Keyboard::Key::Left))
		{
			direction = Matrix3f::RotationY(-90.0f * deltaTime).TransformDirection(direction);
			rotated = true;
		}
		if (rotated)
		{
			camera.SetDirection(direction);
		}
	}

	static void CameraUpdateNewFps(Camera& camera, F32 deltaTime)
	{
		Vector3f direction = camera.GetDirection();
		Vector3f mvtUnit = direction;
		mvtUnit.y = 0.0f;
		mvtUnit.Normalize();

		Vector3f movement;
		bool moved = false;
		if (Keyboard::IsHold(Keyboard::Key::W))
		{
			movement += 2.0f * mvtUnit * deltaTime;
			moved = true;
		}
		if (Keyboard::IsHold(Keyboard::Key::S))
		{
			movement -= 2.0f * mvtUnit * deltaTime;
			moved = true;
		}
		if (Keyboard::IsHold(Keyboard::Key::A))
		{
			movement -= 2.0f * mvtUnit.CrossProduct(Vector3f::UnitY()) * deltaTime;
			moved = true;
		}
		if (Keyboard::IsHold(Keyboard::Key::D))
		{
			movement += 2.0f * mvtUnit.CrossProduct(Vector3f::UnitY()) * deltaTime;
			moved = true;
		}
		if (moved)
		{
			camera.Move(movement);
		}

		bool rotated = false;

		F32 yaw = static_cast<F32>(Mouse::GetMouseMovement().x);
		F32 pitch = static_cast<F32>(Mouse::GetMouseMovement().y);
		if (!Math::Equals(yaw, 0.0f))
		{
			yaw *= 20.0f * deltaTime;
			direction = Matrix3f::RotationY(yaw).TransformDirection(direction);
			rotated = true;
		}
		if (!Math::Equals(pitch, 0.0f))
		{
			pitch *= 15.0f * deltaTime;
			direction = Matrix3f::RotationX(pitch).TransformDirection(direction);
			rotated = true;
		}

		if (!Keyboard::IsAltHold())
		{
			Mouse::SetPositionCurrentWindow({ 400, 300 });

			if (rotated)
			{
				camera.SetDirection(direction);
			}
		}
	}

	static void CameraUpdateMaya(Camera& camera, F32 deltaTime)
	{
		if (Keyboard::IsAltHold())
		{
			Vector3f direction = camera.GetDirection();
			Vector3f mvtUnit = direction;
			mvtUnit.y = 0.0f;
			mvtUnit.Normalize();

			Vector3f movement;
			bool moved = false;
			if (Keyboard::IsHold(Keyboard::Key::W))
			{
				movement += 2.0f * mvtUnit * deltaTime;
				moved = true;
			}
			if (Keyboard::IsHold(Keyboard::Key::S))
			{
				movement -= 2.0f * mvtUnit * deltaTime;
				moved = true;
			}
			if (Keyboard::IsHold(Keyboard::Key::A))
			{
				movement -= 2.0f * mvtUnit.CrossProduct(Vector3f::UnitY()) * deltaTime;
				moved = true;
			}
			if (Keyboard::IsHold(Keyboard::Key::D))
			{
				movement += 2.0f * mvtUnit.CrossProduct(Vector3f::UnitY()) * deltaTime;
				moved = true;
			}
			if (moved)
			{
				camera.Move(movement);
			}

			bool rotated = false;

			F32 yaw = static_cast<F32>(Mouse::GetMouseMovement().x);
			F32 pitch = static_cast<F32>(Mouse::GetMouseMovement().y);
			if (!Math::Equals(yaw, 0.0f))
			{
				yaw *= 20.0f * deltaTime;
				direction = Matrix3f::RotationY(yaw).TransformDirection(direction);
				rotated = true;
			}
			if (!Math::Equals(pitch, 0.0f))
			{
				pitch *= 15.0f * deltaTime;
				direction = Matrix3f::RotationX(pitch).TransformDirection(direction);
				rotated = true;
			}

			if (rotated)
			{
				camera.SetDirection(direction);
			}
		}
	}
};

} // namespace NAMESPACE_NAME