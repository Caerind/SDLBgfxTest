#pragma once

#include "EngineIntegration.hpp"

#include <bgfx/bgfx.h>

namespace NAMESPACE_NAME
{

class Camera
{
public:
	Camera();

	void Apply(bgfx::ViewId viewId) const;

    // Projection

    void InitializePerspective(F32 fov, F32 nearPlane, F32 farPlane, F32 ratio);

    void SetFOV(F32 fov);
    F32 GetFOV() const;

    void SetNearPlane(F32 nearPlane);
    F32 GetNearPlane() const;

    void SetFarPlane(F32 farPlane);
    F32 GetFarPlane() const;

    void SetRatio(F32 ratio);
    F32 GetRatio() const;

    const F32* GetProjectionMatrix() const;

    // View

    void InitializeView(const Vector3f& pos, const Vector3f& at, const Vector3f& up);

    void SetPosition(const Vector3f& pos);
    const Vector3f& GetPosition() const;

    void SetLookAt(const Vector3f& at);
    const Vector3f& GetLookAt() const;

    void SetUpVector(const Vector3f& up);
    const Vector3f& GetUpVector() const;
    
    const F32* GetViewMatrix() const;

private:
    void UpdateProjectionMatrix() const;
    void UpdateViewMatrix() const;

private:
    // TODO : Reorder elements
    mutable F32 mProjectionMatrix[16];
    F32 mFov;
    F32 mNearPlane;
    F32 mFarPlane;
    F32 mRatio;
    mutable bool mProjectionDirty;

    mutable F32 mViewMatrix[16];
    Vector3f mPosition;
    Vector3f mLookAt;
    Vector3f mUpVector;
    mutable bool mViewDirty;
};

} // namespace NAMESPACE_NAME