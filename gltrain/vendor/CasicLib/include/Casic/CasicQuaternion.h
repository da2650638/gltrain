#pragma once

#include "CasicConfig.h"
#include "CasicMath.h"

namespace Casic
{
namespace Math
{
	struct Quaternion
	{
		float w;	// NOTE: 实数部分
		float x;	
		float y;	
		float z;	
		Quaternion()
			: w(1.0f), x(0.0f), y(0.0f), z(0.0f) {}

		Quaternion(float w, float x, float y, float z)
			: w(w), x(x), y(y), z(z) {}

		Quaternion(const Quaternion& other)
			: w(other.w), x(other.x), y(other.y), z(other.z) {}

		Quaternion operator-() const
		{
			Quaternion result(-w, -x, -y, -z);
			return result;
		}

		Quaternion operator+(const Quaternion& other) const
		{
			Quaternion result(w + other.w, x + other.x, y + other.y, z + other.z);
			return result;
		}

		Quaternion operator-(const Quaternion& other) const
		{
			return Quaternion(w - other.w, x - other.x, y - other.y, z - other.z);
		}

		Quaternion operator*(float scalar) const
		{
			Quaternion result(w * scalar, x * scalar, y * scalar, z * scalar);
			return result;
		}

		Quaternion operator*(const Quaternion& other) const
		{
			Quaternion result;
			float s1 = w, s2 = other.w;
			Vector3 v1(x, y, z), v2(other.x, other.y, other.z);
			result.w = s1 * s2 - Dot(v1, v2);
			Vector3 resultV(
				s1 * v2 + s2 * v1 + Cross(v1, v2)
			);
			result.x = resultV.x;
			result.y = resultV.y;
			result.z = resultV.z;
			return result;
		}

		Quaternion operator/(float scalar) const
		{
			float invScalar = 1.0f / scalar;
			return (*this) * invScalar;
		}

		Quaternion Conjugate() const
		{
			Quaternion result(w, -x, -y, -z);
			return result;
		}

		float SquaredMagnitude() const
		{
			return w * w + x * x + y * y + z * z;
		}

		float Magnitude() const
		{
			return std::sqrt(SquaredMagnitude());
		}

		Quaternion Inverse() const
		{
			float normSq = SquaredMagnitude();
			if (normSq == 0.0f) 
			{
				throw std::runtime_error("Cannot invert a quaternion with zero magnitude");
			}
			return Conjugate() / normSq;
		}

		Quaternion operator/(const Quaternion& other)
		{
			Quaternion result = (*this) * other.Inverse();
			return result;
		}

		/**
		 * @brief Applies the rotation represented by this quaternion to a vector.
		 *
		 * This function computes the result of applying the quaternion's rotation to the input vector `p`.
		 * The quaternion must be normalized (unit length) before calling this function. If the quaternion
		 * is not normalized, the result will be undefined, and an exception will be thrown.
		 *
		 * @param p The input vector to be rotated.
		 * @return The rotated vector.
		 *
		 * @throws std::runtime_error If the quaternion is not normalized.
		 */
		Vector3 operator*(const Vector3& p) const
		{
			if (!Equals(Magnitude(), 1.0f))
			{
				throw std::runtime_error("Quaternion must be normalized.");
			}
			Vector3 v(x, y, z);
			Vector3 vp = Cross(v, p);
			Vector3 vvp = Cross(v, vp);
			return p + 2 * (w * vp + vvp);
		}

		operator Matrix4() const
		{
			Matrix4 result;
			result.Data.m0 = w;
			result.Data.m1 = x;
			result.Data.m2 = y;
			result.Data.m3 = z;
			result.Data.m4 = -x;
			result.Data.m5 = w;
			result.Data.m6 = x;
			result.Data.m7 = -y;
			result.Data.m8 = -y;
			result.Data.m9 = -z;
			result.Data.m10 = w;
			result.Data.m11 = x;
			result.Data.m12 = -z;
			result.Data.m13 = y;
			result.Data.m14 = -x;
			result.Data.m15 = w;
			return result;
		}
	};

	inline CASICLIB_API Quaternion operator*(float scalar, const Quaternion& quat)
	{
		Quaternion result = quat * scalar;
		return result;
	}

	inline CASICLIB_API Quaternion RotationQuat(float degree, const Vector3& axis)
	{
		Vector3 normAxis = Normalize(axis);
		float sinHalfTheta = std::sin(degreesToRadians(0.5f * degree));
		float cosHalfTheta = std::cos(degreesToRadians(0.5f * degree));
		Quaternion result(cosHalfTheta, sinHalfTheta * normAxis.x, sinHalfTheta * normAxis.y, sinHalfTheta * normAxis.z);
		return result;
	}

	inline CASICLIB_API Vector3 operator*(const Vector3& p, const Quaternion& quat)
	{
		return quat * p;
	}
}
}