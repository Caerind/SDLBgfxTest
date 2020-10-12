#pragma once

#include <Enlivengine/Math/Vector3.hpp>

namespace en 
{

class Sphere
{
public:
	constexpr Sphere() : mCenter(), mRadius(1.0f) {}
	constexpr Sphere(const Vector3f& center, F32 radius = 1.0f) : mCenter(center), mRadius(radius) {}

	constexpr const Vector3f& GetCenter() const { return mCenter; }
	constexpr void SetCenter(const Vector3f& center) { mCenter = center; }

	constexpr F32 GetRadius() const { return mRadius; }
	constexpr void SetRadius(F32 radius) { mRadius = radius; }

	constexpr bool operator==(const Sphere& other) const { return mRadius == other.mRadius && mCenter == other.mCenter; }
	constexpr bool operator!=(const Sphere& other) const { return !operator==(other); }

	inline F32 GetDistance(const Vector3f& point) const
	{
		return (mCenter - point).GetLength() - mRadius;
	}

	constexpr bool Contains(const Vector3f& point) const
	{
		return (mCenter - point).GetSquaredLength() <= mRadius * mRadius;
	}

	constexpr bool Contains(const Sphere& sphere) const
	{
		if (mRadius < sphere.mRadius || !sphere.Contains(mCenter))
		{
			return false;
		}
		const Vector3f d = mCenter - sphere.mCenter;
		return (sphere.mRadius - d.GetLength() - mRadius > 0.0f);
	}

	constexpr bool Intersects(const Sphere& sphere) const
	{
		const F32 rr = (mRadius + sphere.mRadius);
		return (mCenter - sphere.mCenter).GetSquaredLength() <= rr * rr;
	}

private:
	Vector3f mCenter;
	F32 mRadius;
};

} // namespace en

#ifdef ENLIVE_ENABLE_META
ENLIVE_META_CLASS_BEGIN(en::Sphere)
	ENLIVE_META_CLASS_MEMBER("center", &en::Sphere::GetCenter, &en::Sphere::SetCenter),
	ENLIVE_META_CLASS_MEMBER("radius", &en::Sphere::GetRadius, &en::Sphere::SetRadius)
ENLIVE_META_CLASS_END()
#endif // ENLIVE_ENABLE_META
