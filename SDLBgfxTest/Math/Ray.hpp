#pragma once

#include "../EngineIntegration.hpp"

#include "Vector3.hpp"

namespace NAMESPACE_NAME
{
	
class Ray
{
public:
	constexpr Ray() : mOrigin(), mDirection(Vector3f::UnitX()) {}
	constexpr Ray(const Vector3f& origin, const Vector3f& direction) : mOrigin(origin), mDirection(direction) {}

	constexpr const Vector3f& GetOrigin() const { return mOrigin; }
	constexpr void SetOrigin(const Vector3f& origin) { mOrigin = origin; }
	constexpr void SetOrigin(F32 x, F32 y, F32 z) { mOrigin.Set(x, y, z); }

	constexpr const Vector3f& GetDirection() const { return mDirection; }
	constexpr void SetDirection(const Vector3f& direction) { mDirection = direction; }
	constexpr void SetDirection(F32 x, F32 y, F32 z) { mDirection.Set(x, y, z); }

	constexpr Vector3f GetPoint(F32 t) const { return mOrigin + mDirection * t; }
	constexpr Vector3f operator*(F32 t) const { return mOrigin + mDirection * t; }

	constexpr bool operator==(const Ray& other) const { return mOrigin == other.mOrigin && mDirection == other.mDirection; }
	constexpr bool operator!=(const Ray& other) const { return !operator==(other); }

private:
	Vector3f mOrigin;
	Vector3f mDirection;
};

} // namespace NAMESPACE_NAME
