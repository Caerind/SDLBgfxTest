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

void Camera::InitializePerspective(F32 fov, F32 nearPlane, F32 farPlane, F32 ratio)
{
	mFov = fov;
	mNearPlane = nearPlane;
	mFarPlane = farPlane;
	mRatio = ratio;
	mProjectionDirty = true;
}

void Camera::SetFOV(F32 fov)
{
	mFov = fov;
	mProjectionDirty = true;
}

F32 Camera::GetFOV() const
{
	return mFov;
}

void Camera::SetNearPlane(F32 nearPlane)
{
	mNearPlane = nearPlane;
	mProjectionDirty = true;
}

F32 Camera::GetNearPlane() const
{
	return mNearPlane;
}

void Camera::SetFarPlane(F32 farPlane)
{
	mFarPlane = farPlane;
	mProjectionDirty = true;
}

F32 Camera::GetFarPlane() const
{
	return mFarPlane;
}

void Camera::SetRatio(F32 ratio)
{
	mRatio = ratio;
	mProjectionDirty = true;
}

F32 Camera::GetRatio() const
{
	return mRatio;
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

void Camera::UpdateProjectionMatrix() const
{
	static constexpr bool homogenousDepth = true; // bgfx::getCaps()->homogeneousDepth
	bx::mtxProj(mProjectionMatrix, mFov, mRatio, mNearPlane, mFarPlane, homogenousDepth, bx::Handness::Right);
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