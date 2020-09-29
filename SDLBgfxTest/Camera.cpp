#include "Camera.hpp"

#include <bx/math.h>

namespace NAMESPACE_NAME
{

Camera::Camera()
{
	// TODO : Init members
}

void Camera::Apply(bgfx::ViewId viewId) const
{
	bgfx::setViewTransform(viewId, GetViewMatrix(), GetProjectionMatrix());
}

void Camera::SetProjection(ProjectionMode projection)
{
	mProjectionMode = projection;
	mProjectionDirty = true;
}

Camera::ProjectionMode Camera::GetProjection() const
{
	return mProjectionMode;
}

void Camera::InitializePerspective(F32 fov, F32 ratio, F32 nearPlane, F32 farPlane)
{
	mProjectionMode = ProjectionMode::Perspective;
	perspective.nearPlane = nearPlane;
	perspective.farPlane = farPlane;
	perspective.fov = fov;
	perspective.ratio = ratio;
	mProjectionDirty = true;
}

void Camera::InitializeOrthographic(F32 left, F32 top, F32 right, F32 bottom, F32 nearPlane, F32 farPlane)
{
	mProjectionMode = ProjectionMode::Orthographic;
	orthographic.nearPlane = nearPlane;
	orthographic.farPlane = farPlane;
	orthographic.left = left;
	orthographic.top = top;
	orthographic.right = right;
	orthographic.bottom = bottom;
	mProjectionDirty = true;
}

void Camera::SetNearPlane(F32 nearPlane)
{
	if (mProjectionMode == ProjectionMode::Perspective)
	{
		perspective.nearPlane = nearPlane;
	}
	else
	{
		orthographic.nearPlane = nearPlane;
	}
	mProjectionDirty = true;
}

F32 Camera::GetNearPlane() const
{
	if (mProjectionMode == ProjectionMode::Perspective)
	{
		return perspective.nearPlane;
	}
	else
	{
		return orthographic.nearPlane;
	}
}

void Camera::SetFarPlane(F32 farPlane)
{
	if (mProjectionMode == ProjectionMode::Perspective)
	{
		perspective.nearPlane = farPlane;
	}
	else
	{
		orthographic.nearPlane = farPlane;
	}
	mProjectionDirty = true;
}

F32 Camera::GetFarPlane() const
{
	if (mProjectionMode == ProjectionMode::Perspective)
	{
		return perspective.farPlane;
	}
	else
	{
		return orthographic.farPlane;
	}
}

void Camera::SetFOV(F32 fov)
{
	assert(mProjectionMode == ProjectionMode::Perspective);
	perspective.fov = fov;
	mProjectionDirty = true;
}

F32 Camera::GetFOV() const
{
	assert(mProjectionMode == ProjectionMode::Perspective);
	return perspective.fov;
}

void Camera::SetRatio(F32 ratio)
{
	assert(mProjectionMode == ProjectionMode::Perspective);
	perspective.ratio = ratio;
	mProjectionDirty = true;
}

F32 Camera::GetRatio() const
{
	assert(mProjectionMode == ProjectionMode::Perspective);
	return perspective.ratio;
}

void Camera::SetLeft(F32 left)
{
	assert(mProjectionMode == ProjectionMode::Orthographic);
	orthographic.left = left;
	mProjectionDirty = true;
}

F32 Camera::GetLeft() const
{
	assert(mProjectionMode == ProjectionMode::Orthographic);
	return orthographic.left;
}

void Camera::SetTop(F32 top)
{
	assert(mProjectionMode == ProjectionMode::Orthographic);
	orthographic.top = top;
	mProjectionDirty = true;
}

F32 Camera::GetTop() const
{
	assert(mProjectionMode == ProjectionMode::Orthographic);
	return orthographic.top;
}

void Camera::SetRight(F32 right)
{
	assert(mProjectionMode == ProjectionMode::Orthographic);
	orthographic.right = right;
	mProjectionDirty = true;
}

F32 Camera::GetRight() const
{
	assert(mProjectionMode == ProjectionMode::Orthographic);
	return orthographic.right;
}

void Camera::SetBottom(F32 bottom)
{
	assert(mProjectionMode == ProjectionMode::Orthographic);
	orthographic.bottom = bottom;
	mProjectionDirty = true;
}

F32 Camera::GetBottom() const
{
	assert(mProjectionMode == ProjectionMode::Orthographic);
	return orthographic.bottom;
}

const F32* Camera::GetProjectionMatrix() const
{
	if (mProjectionDirty)
	{
		UpdateProjectionMatrix();
	}
	return mProjectionMatrix;
}

void Camera::InitializeView(const Vector3f& pos, const Vector3f& at, const Vector3f& up)
{
	mPosition = pos;
	mLookAt = at;
	mUpVector = up;
	mViewDirty = true;
}

void Camera::SetPosition(const Vector3f& pos)
{
	mPosition = pos;
	mViewDirty = true;
}

const Vector3f& Camera::GetPosition() const
{
	return mPosition;
}

void Camera::SetLookAt(const Vector3f& at)
{
	mLookAt = at;
	mViewDirty = true;
}

const Vector3f& Camera::GetLookAt() const
{
	return mLookAt;
}

void Camera::SetUpVector(const Vector3f& up)
{
	mUpVector = up;
	mViewDirty = true;
}

const Vector3f& Camera::GetUpVector() const
{
	return mUpVector;
}

const F32* Camera::GetViewMatrix() const
{
	if (mViewDirty)
	{
		UpdateViewMatrix();
	}
	return mViewMatrix;
}

void Camera::Move(const Vector3f& movement)
{
	mPosition += movement;
	mLookAt += movement;
	mViewDirty = true;
}

void Camera::UpdateProjectionMatrix() const
{
	static constexpr bool homogenousDepth = true; // bgfx::getCaps()->homogeneousDepth
	if (mProjectionMode == ProjectionMode::Perspective)
	{
		bx::mtxProj(mProjectionMatrix, perspective.fov, perspective.ratio, perspective.nearPlane, perspective.farPlane, homogenousDepth, bx::Handness::Right);
	}
	else
	{
		static constexpr F32 offset = 0.0f;
		bx::mtxOrtho(mProjectionMatrix, orthographic.left, orthographic.right, orthographic.bottom, orthographic.top, orthographic.nearPlane, orthographic.farPlane, offset, homogenousDepth, bx::Handness::Right);
	}
	mProjectionDirty = false;
}

void Camera::UpdateViewMatrix() const
{
	const bx::Vec3 eye(mPosition.x, mPosition.y, mPosition.z);
	const bx::Vec3 at(mLookAt.x, mLookAt.y, mLookAt.z);
	const bx::Vec3 up(0.0f, 1.0f, 0.0f);
	bx::mtxLookAt(mViewMatrix, eye, at, up, bx::Handness::Right);
	mViewDirty = false;
}

} // namespace NAMESPACE_NAME