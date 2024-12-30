#pragma once

#include "CasicConfig.h"

#include <algorithm>
#include <cmath>
#include <iostream>

namespace Casic
{
namespace Math
{
	CASICLIB_API bool Equals(float f1, float f2, float epsilon = 1e-6f);
	CASICLIB_API float degreesToRadians(float degree);

	class Vector2;
	class Vector3;
	class Vector4;

	typedef struct CASICLIB_API Vector2 {
		union {
			float x;
			float s;
		};
		union {
			float y;
			float t;
		};

		Vector2()
			: x(0.0f), y(0.0f) {}

		Vector2(float x, float y) {
			this->x = x;
			this->y = y;
		}

		Vector2(const Vector2& other) {
			x = other.x;
			y = other.y;
		}

		Vector2 operator+(const Vector2& other) const {
			Vector2 result;
			result.x = x + other.x;
			result.y = y + other.y;
			return result;
		}

		Vector2 operator-(const Vector2& other) const {
			Vector2 result;
			result.x = x - other.x;
			result.y = y - other.y;
			return result;
		}

		Vector2 operator*(float value) const {
			Vector2 result;
			result.x *= value;
			result.y *= value;
			return result;
		}

		Vector2 operator/(float value) const {
			Vector2 result;
			float inv = 1.0f / value;
			result.x *= inv;
			result.y *= inv;
			return result;
		}

		bool operator==(const Vector2& other) {
			return this->Equals(other);
		}

		bool operator!=(const Vector2& other) {
			return !this->Equals(other);
		}

		bool Equals(const Vector2& other, float epsilon = 1e-6f) const {
			return std::fabs(x - other.x) < epsilon && std::fabs(y - other.y) < epsilon;
		}

		float LengthSquare() const {
			return x * x + y * y;
		}

		float Length() const {
			return std::sqrt(LengthSquare());
		}

		Vector2& Normalize() {
			float invLength = 1.0f / Length();
			x *= invLength;
			y *= invLength;
			return *this;
		}
	}Vector2;

	inline CASICLIB_API float Dot(const Vector2& v1, const Vector2& v2) {
		return v1.x * v2.x + v1.y * v2.y;
	}

	inline CASICLIB_API Vector2 Normalize(const Vector2& v) {
		float invLength = 1.0f / v.Length();
		Vector2 result;
		result.x = v.x * invLength;
		result.y = v.y * invLength;
		return result;
	}

	inline CASICLIB_API Vector2 Lerp(const Vector2& v1, const Vector2& v2, float t) {
		return v1 + (v2 - v1) * std::clamp(t, 0.0f, 1.0f);
	}

	typedef struct CASICLIB_API Vector3 {
		union {
			float x;
			float s;
		};
		union {
			float y;
			float t;
		};
		union {
			float z;
			float p;
		};

		Vector3()
			: x(0.0f), y(0.0f), z(0.0f) {}

		Vector3(float x, float y, float z) {
			this->x = x;
			this->y = y;
			this->z = z;
		}

		Vector3(const Vector3& other)
			: x(other.x), y(other.y), z(other.z)
		{}

		Vector3(const Vector4& other);

		Vector3 operator+(const Vector3& other) const {
			Vector3 result;
			result.x = x + other.x;
			result.y = y + other.y;
			result.z = z + other.z;
			return result;
		}

		Vector3 operator-(const Vector3& other) const {
			Vector3 result;
			result.x = x - other.x;
			result.y = y - other.y;
			result.z = z - other.z;
			return result;
		}

		Vector3 operator*(float value) const {
			Vector3 result;
			result.x = x * value;
			result.y = y * value;
			result.z = z * value;
			return result;
		}

		Vector3 operator/(float value) const {
			Vector3 result;
			float inv = 1.0f / value;
			result.x = x * inv;
			result.y = y * inv;
			result.z = z * inv;
			return result;
		}

		bool operator==(const Vector3& other) const {
			return this->Equals(other);
		}

		bool operator!=(const Vector3& other) const {
			return !this->Equals(other);
		}

		bool Equals(const Vector3& other, float epsilon = 1e-6f) const {
			return std::fabs(x - other.x) < epsilon && std::fabs(y - other.y) < epsilon && std::fabs(z - other.z) < epsilon;
		}

		float LengthSquare() const {
			return x * x + y * y + z * z;
		}

		float Length() const {
			return std::sqrt(LengthSquare());
		}

		Vector3& Normalize() {
			float length = Length();
			float invLength = 1.0f / length;
			x *= invLength;
			y *= invLength;
			z *= invLength;
			return *this;
		}

		Vector3& RotateAroundXAxis(float angle);

		Vector3& RotateAroundYAxis(float angle);

		Vector3& RotateAroundZAxis(float angle);

		//---------------------------------------------------------
		// 实现经典的围绕任意轴旋转的罗格里格兹旋转公式
		//---------------------------------------------------------
		Vector3& RotateAroundAxis(float angle, Vector3 axis);
	}Vector3;

	inline CASICLIB_API Vector3 operator*(float scale, Vector3 v) {
		Vector3 result{ scale * v.x, scale * v.y, scale * v.z };
		return result;
	}

	inline CASICLIB_API float Dot(const Vector3& v1, const Vector3& v2) {
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}

	inline CASICLIB_API Vector3 Cross(const Vector3& v1, const Vector3& v2) {
		Vector3 result;
		result.x = v1.y * v2.z - v1.z * v2.y;
		result.y = v1.z * v2.x - v1.x * v2.z;
		result.z = v1.x * v2.y - v1.y * v2.x;
		return result;
	}

	inline CASICLIB_API Vector3 Normalize(const Vector3& v) {
		float invLength = 1.0f / v.Length();
		Vector3 result;
		result.x = v.x * invLength;
		result.y = v.y * invLength;
		result.z = v.z * invLength;
		return result;
	}

	inline CASICLIB_API Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t) {
		return v1 + (v2 - v1) * std::clamp(t, 0.0f, 1.0f);
	}

	typedef struct CASICLIB_API Vector4 {
		// TODO: Vector4 应该有哪些函数该补全呢？
		union {
			float r;
			float x;
			float s;
		};
		union {
			float g;
			float y;
			float t;
		};
		union {
			float b;
			float z;
			float p;
		};
		union {
			float a;
			float w;
			float q;
		};

		Vector4()
			: x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}

		Vector4(float x, float y, float z, float w) {
			this->x = x;
			this->y = y;
			this->z = z;
			this->w = w;
		}

		Vector4(const Vector4& other)
			: x(other.x), y(other.y), z(other.z), w(other.w) {}

		Vector4(const Vector3& other, float wValue = 1.0f)
			: x(other.x), y(other.y), z(other.z), w(wValue) {}

		Vector4 operator+(const Vector4& other) const {
			return Vector4(x + other.x, y + other.y, z + other.z, w + other.w);
		}

		Vector4 operator-(const Vector4& other) const {
			return Vector4(x - other.x, y - other.y, z - other.z, w - other.w);
		}

		Vector4 operator*(float value) const {
			return Vector4(x * value, y * value, z * value, w * value);
		}

		Vector4 operator/(float value) const {
			float inv = 1.0f / value;
			return Vector4(x * inv, y * inv, z * inv, w * inv);
		}

		bool operator==(const Vector4& other) const {
			return Equals(other);
		}

		bool operator!=(const Vector4& other) const {
			return !Equals(other);
		}

		operator Vector3() const {
			Vector3 result{ x, y, z };
			return result;
		}

		bool Equals(const Vector4& other, float epsilon = 1e-6f) const {
			return std::fabs(x - other.x) < epsilon &&
				std::fabs(y - other.y) < epsilon &&
				std::fabs(z - other.z) < epsilon &&
				std::fabs(w - other.w) < epsilon;
		}

		float LengthSquare() const {
			return x * x + y * y + z * z + w * w;
		}

		float Length() const {
			return std::sqrt(LengthSquare());
		}

		Vector4& Normalize() {
			float invLength = 1.0f / Length();
			x *= invLength;
			y *= invLength;
			z *= invLength;
			w *= invLength;
			return *this;
		}
	};

	inline CASICLIB_API float Dot(const Vector4& v1, const Vector4& v2) {
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
	}

	inline CASICLIB_API Vector4 Normalize(const Vector4& v) {
		float invLength = 1.0f / v.Length();
		return Vector4(v.x * invLength, v.y * invLength, v.z * invLength, v.w * invLength);
	}

	inline CASICLIB_API Vector4 Lerp(const Vector4& v1, const Vector4& v2, float t) {
		return v1 + (v2 - v1) * std::clamp(t, 0.0f, 1.0f);
	}
}
}