#pragma once

#include "../EngineIntegration.hpp"

#include "Vector3.hpp"
#include "Plane.hpp"

namespace NAMESPACE_NAME 
{
	
class Frustum
{
	public:
		enum class Planes { Near = 0, Far, Top, Bottom, Right, Left };
		enum class Corners { FTL = 0, FTR, FBR, FBL, NTL, NTR, NBR, NBL };
		
		constexpr Frustum() = default;
		constexpr Frustum(F32 fov, F32 ratio, F32 zNear, F32 zFar, const Vector3f& eye, const Vector3f& target, const Vector3f& up)
			: Frustum()
		{
			Build(fov, ratio, zNear, zFar, eye, target, up);
		}

		constexpr void Build(F32 fov, F32 ratio, F32 zNear, F32 zFar, const Vector3f& eye, const Vector3f& target, const Vector3f& up)
		{
			// TODO : Fixme : Handle Handedness & Homogeneous
	
			const F32 tangent = Math::Tan(fov * 0.5f);
			const F32 nearH = zNear * tangent;
			const F32 nearW = nearH * ratio;
			const F32 farH = zFar * tangent;
			const F32 farW = farH * ratio;

			const Vector3f f((target - eye).Normalized());
			const Vector3f s(up.CrossProduct(f).Normalized());
			const Vector3f u(f.CrossProduct(s));

			const Vector3f nc = eye + f * zNear;
			const Vector3f fc = eye + f * zFar;

			mCorners[static_cast<U32>(Corners::FTL)] = fc + u * farH - s * farW;
			mCorners[static_cast<U32>(Corners::FTR)] = fc + u * farH + s * farW;
			mCorners[static_cast<U32>(Corners::FBR)] = fc - u * farH + s * farW;
			mCorners[static_cast<U32>(Corners::FBL)] = fc - u * farH - s * farW;
			mCorners[static_cast<U32>(Corners::NTL)] = nc + u * nearH - s * nearW;
			mCorners[static_cast<U32>(Corners::NTR)] = nc + u * nearH + s * nearW;
			mCorners[static_cast<U32>(Corners::NBR)] = nc - u * nearH + s * nearW;
			mCorners[static_cast<U32>(Corners::NBL)] = nc - u * nearH - s * nearW;

			mPlanes[static_cast<U32>(Planes::Near)].Set(mCorners[static_cast<U32>(Corners::NTL)], mCorners[static_cast<U32>(Corners::NTR)], mCorners[static_cast<U32>(Corners::NBR)]);
			mPlanes[static_cast<U32>(Planes::Far)].Set(mCorners[static_cast<U32>(Corners::FTR)], mCorners[static_cast<U32>(Corners::FTL)], mCorners[static_cast<U32>(Corners::FBL)]);
			mPlanes[static_cast<U32>(Planes::Top)].Set(mCorners[static_cast<U32>(Corners::NTL)], mCorners[static_cast<U32>(Corners::FTL)], mCorners[static_cast<U32>(Corners::FTR)]);
			mPlanes[static_cast<U32>(Planes::Bottom)].Set(mCorners[static_cast<U32>(Corners::FBR)], mCorners[static_cast<U32>(Corners::NBR)], mCorners[static_cast<U32>(Corners::NBL)]);
			mPlanes[static_cast<U32>(Planes::Right)].Set(mCorners[static_cast<U32>(Corners::FBR)], mCorners[static_cast<U32>(Corners::NBR)], mCorners[static_cast<U32>(Corners::NTR)]);
			mPlanes[static_cast<U32>(Planes::Left)].Set(mCorners[static_cast<U32>(Corners::NBL)], mCorners[static_cast<U32>(Corners::FBL)], mCorners[static_cast<U32>(Corners::FTL)]);
		}

		constexpr bool Contains(const Vector3f& point) const
		{
			for (U32 i = 0; i < 6; i++)
			{
				if (mPlanes[i].GetSide(point) == Plane::Side::Negative)
				{
					return false;
				}
			}
			return true;
		}

		constexpr const Plane& GetPlane(U32 index) const { return mPlanes[index]; }
		constexpr const Vector3f& GetCorner(U32 index) const { return mCorners[index]; }

	private:
		Plane mPlanes[6];
		Vector3f mCorners[8];
};

} // namespace NAMESPACE_NAME
