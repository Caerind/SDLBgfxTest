#pragma once

#include "EngineIntegration.hpp"

#include <bgfx/bgfx.h>

#include "Math/Vector3.hpp"
#include "Math/Matrix4.hpp"

namespace NAMESPACE_NAME
{

class Camera
{
public:
	Camera();

	void Apply(bgfx::ViewId viewId) const;

    // Projection

    enum class ProjectionMode
    {
        Perspective,
        Orthographic
    };
    void SetProjection(ProjectionMode projection);
    ProjectionMode GetProjection() const;

    void InitializePerspective(F32 fov, F32 ratio, F32 nearPlane, F32 farPlane);
	void InitializeOrthographic(F32 left, F32 top, F32 right, F32 bottom, F32 nearPlane, F32 farPlane);

	void SetNearPlane(F32 nearPlane);
	F32 GetNearPlane() const;

	void SetFarPlane(F32 farPlane);
	F32 GetFarPlane() const;

	void SetFOV(F32 fov);
	F32 GetFOV() const;

	void SetRatio(F32 ratio);
	F32 GetRatio() const;

	void SetLeft(F32 left);
	F32 GetLeft() const;

	void SetTop(F32 top);
	F32 GetTop() const;

	void SetRight(F32 right);
	F32 GetRight() const;

	void SetBottom(F32 bottom);
	F32 GetBottom() const;

    const Matrix4f& GetProjectionMatrix() const;

    // View

    void InitializeView(const Vector3f& pos, const Vector3f& at);

	void LookAt(const Vector3f& at);
    
    const Matrix4f& GetViewMatrix() const;

	void SetPosition(const Vector3f& position);
	const Vector3f& GetPosition() const;
	void Move(const Vector3f& movement);

	void SetDirection(const Vector3f& direction);
	const Vector3f& GetDirection() const;

private:
    void UpdateProjectionMatrix() const;
    void UpdateViewMatrix() const;

private:
    struct PerspectiveData
	{
		F32 nearPlane;
		F32 farPlane;
		F32 fov;
		F32 ratio;
	};

	struct OrthographicData
	{
		F32 nearPlane;
		F32 farPlane;
		F32 left;
		F32 top;
		F32 right;
		F32 bottom;
	};

	mutable Matrix4f mViewMatrix;
    mutable Matrix4f mProjectionMatrix;
	Vector3f mPosition;
	Vector3f mDirection;
	union
	{
		PerspectiveData perspective;
		OrthographicData orthographic;
	};
	ProjectionMode mProjectionMode;
	mutable bool mProjectionDirty;
	mutable bool mViewDirty;
};

} // namespace NAMESPACE_NAME