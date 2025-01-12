#pragma once

#include "CasicConfig.h"
#include "CasicMath.h"

namespace Casic
{
namespace Math
{
	struct CASICLIB_API Quaternion
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

		Quaternion operator-() const;

		Quaternion operator+(const Quaternion& other) const;

		Quaternion operator-(const Quaternion& other) const;

		Quaternion operator*(float scalar) const;

		Quaternion operator*(const Quaternion& other) const;

		Quaternion operator/(float scalar) const;

		Quaternion Conjugate() const;

		float SquaredMagnitude() const;

		float Magnitude() const;

		Quaternion Inverse() const;

		Quaternion operator/(const Quaternion& other);

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
		Vector3 operator*(const Vector3& p) const;

		operator Matrix4() const;
	};

	CASICLIB_API Quaternion operator*(float scalar, const Quaternion& quat);

	CASICLIB_API Quaternion RotationQuat(float degree, const Vector3& axis);

	CASICLIB_API Vector3 operator*(const Vector3& p, const Quaternion& quat);
}
}