#pragma once
#include "CasicConfig.h"

#include <algorithm>
#include <cmath>
#include <iostream>

#ifdef CASIC_USE_STDFORMAT
#include <format>
#else 
#include <iomanip>
#endif

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

		Vector3 operator+(const Vector3& other) const{
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

	typedef struct CASICLIB_API Matrix4 {
		typedef struct {
			// Column major memory layout: [m0, m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, m12, m13, m14, m15]
			// Row major memory layout:	   [m0, m4, m8, m12, m1, m5, m9, m13, m2, m6, m10, m14, m3, m7, m11, m15]
			float m0, m4, m8, m12;	// 1st row
			float m1, m5, m9, m13;  // 2nd row
			float m2, m6, m10,m14;  // 3rd row
			float m3, m7, m11,m15;  // 4th row
		}Matrix4Internal;
		Matrix4Internal Data;

		Matrix4() {
			Data.m0 = Data.m4 = Data.m8 = Data.m12 = 0.0f;
			Data.m1 = Data.m5 = Data.m9 = Data.m13 = 0.0f;
			Data.m2 = Data.m6 = Data.m10 = Data.m14 = 0.0f;
			Data.m3 = Data.m7 = Data.m11 = Data.m15 = 0.0f;
		}

		Matrix4(const Matrix4& other) {
			this->Data.m0 = other.Data.m0;
			this->Data.m1 = other.Data.m1;
			this->Data.m2 = other.Data.m2;
			this->Data.m3 = other.Data.m3;
			this->Data.m4 = other.Data.m4;
			this->Data.m5 = other.Data.m5;
			this->Data.m6 = other.Data.m6;
			this->Data.m7 = other.Data.m7;
			this->Data.m8 = other.Data.m8;
			this->Data.m9 = other.Data.m9;
			this->Data.m10 = other.Data.m10;
			this->Data.m11 = other.Data.m11;
			this->Data.m12 = other.Data.m12;
			this->Data.m13 = other.Data.m13;
			this->Data.m14 = other.Data.m14;
			this->Data.m15 = other.Data.m15;
		}

		Matrix4& operator=(const Matrix4& other) {
			this->Data.m0 =  other.Data.m0 ;
			this->Data.m1 =  other.Data.m1 ;
			this->Data.m2 =  other.Data.m2 ;
			this->Data.m3 =  other.Data.m3 ;
			this->Data.m4 =  other.Data.m4 ;
			this->Data.m5 =  other.Data.m5 ;
			this->Data.m6 =  other.Data.m6 ;
			this->Data.m7 =  other.Data.m7 ;
			this->Data.m8 =  other.Data.m8 ;
			this->Data.m9 =  other.Data.m9 ;
			this->Data.m10 = other.Data.m10;
			this->Data.m11 = other.Data.m11;
			this->Data.m12 = other.Data.m12;
			this->Data.m13 = other.Data.m13;
			this->Data.m14 = other.Data.m14;
			this->Data.m15 = other.Data.m15;
			return *this;
		}

		Matrix4 Transpose() {
			Matrix4 temp(*this);
			this->Data.m0 = temp.Data.m0;
			this->Data.m1 = temp.Data.m4;
			this->Data.m2 = temp.Data.m8;
			this->Data.m3 = temp.Data.m12;
			this->Data.m4 = temp.Data.m1;
			this->Data.m5 = temp.Data.m5;
			this->Data.m6 = temp.Data.m9;
			this->Data.m7 = temp.Data.m13;
			this->Data.m8 = temp.Data.m2;
			this->Data.m9 = temp.Data.m6;
			this->Data.m10 = temp.Data.m10;
			this->Data.m11 = temp.Data.m14;
			this->Data.m12 = temp.Data.m3;
			this->Data.m13 = temp.Data.m7;
			this->Data.m14 = temp.Data.m11;
			this->Data.m15 = temp.Data.m15;
			return temp;
		}

		Matrix4 operator*(const Matrix4& right) {
			Matrix4 result;

			// first column
			result.Data.m0 = this->Data.m0 * right.Data.m0 + this->Data.m4 * right.Data.m1 + this->Data.m8 * right.Data.m2 + this->Data.m12 * right.Data.m3;
			result.Data.m1 = this->Data.m1 * right.Data.m0 + this->Data.m5 * right.Data.m1 + this->Data.m9 * right.Data.m2 + this->Data.m13 * right.Data.m3;
			result.Data.m2 = this->Data.m2 * right.Data.m0 + this->Data.m6 * right.Data.m1 + this->Data.m10 * right.Data.m2 + this->Data.m14 * right.Data.m3;
			result.Data.m3 = this->Data.m3 * right.Data.m0 + this->Data.m7 * right.Data.m1 + this->Data.m11 * right.Data.m2 + this->Data.m15 * right.Data.m3;
			// second column 
			result.Data.m4 = this->Data.m0 * right.Data.m4 + this->Data.m4 * right.Data.m5 + this->Data.m8 * right.Data.m6 + this->Data.m12 * right.Data.m7;
			result.Data.m5 = this->Data.m1 * right.Data.m4 + this->Data.m5 * right.Data.m5 + this->Data.m9 * right.Data.m6 + this->Data.m13 * right.Data.m7;
			result.Data.m6 = this->Data.m2 * right.Data.m4 + this->Data.m6 * right.Data.m5 + this->Data.m10 * right.Data.m6 + this->Data.m14 * right.Data.m7;
			result.Data.m7 = this->Data.m3 * right.Data.m4 + this->Data.m7 * right.Data.m5 + this->Data.m11 * right.Data.m6 + this->Data.m15 * right.Data.m7;
			// third column 
			result.Data.m8 = this->Data.m0 * right.Data.m8 + this->Data.m4 * right.Data.m9 + this->Data.m8 * right.Data.m10 + this->Data.m12 * right.Data.m11;
			result.Data.m9 = this->Data.m1 * right.Data.m8 + this->Data.m5 * right.Data.m9 + this->Data.m9 * right.Data.m10 + this->Data.m13 * right.Data.m11;
			result.Data.m10 = this->Data.m2 * right.Data.m8 + this->Data.m6 * right.Data.m9 + this->Data.m10 * right.Data.m10 + this->Data.m14 * right.Data.m11;
			result.Data.m11 = this->Data.m3 * right.Data.m8 + this->Data.m7 * right.Data.m9 + this->Data.m11 * right.Data.m10 + this->Data.m15 * right.Data.m11;
			// fourth column 
			result.Data.m12 = this->Data.m0 * right.Data.m12 + this->Data.m4 * right.Data.m13 + this->Data.m8 * right.Data.m14 + this->Data.m12 * right.Data.m15;
			result.Data.m13 = this->Data.m1 * right.Data.m12 + this->Data.m5 * right.Data.m13 + this->Data.m9 * right.Data.m14 + this->Data.m13 * right.Data.m15;
			result.Data.m14 = this->Data.m2 * right.Data.m12 + this->Data.m6 * right.Data.m13 + this->Data.m10 * right.Data.m14 + this->Data.m14 * right.Data.m15;
			result.Data.m15 = this->Data.m3 * right.Data.m12 + this->Data.m7 * right.Data.m13 + this->Data.m11 * right.Data.m14 + this->Data.m15 * right.Data.m15;
			
			return result;
		}

		Vector4 operator*(const Vector4& vec) {
			Vector4 result;

			result.x = vec.x * this->Data.m0 + vec.y * this->Data.m4 + vec.z * this->Data.m8 + vec.w * this->Data.m12;
			result.y = vec.x * this->Data.m1 + vec.y * this->Data.m5 + vec.z * this->Data.m9 + vec.w * this->Data.m13;
			result.z = vec.x * this->Data.m2 + vec.y * this->Data.m6 + vec.z * this->Data.m10 + vec.w * this->Data.m14;
			result.w = vec.x * this->Data.m3 + vec.y * this->Data.m7 + vec.z * this->Data.m11 + vec.w * this->Data.m15;

			return result;
		}

		bool Equals(const Matrix4& other, float epsilon = 1e-6f) const
		{
			return std::fabs(Data.m0 - other.Data.m0) < epsilon &&
				std::fabs(Data.m1 - other.Data.m1) < epsilon &&
				std::fabs(Data.m2 - other.Data.m2) < epsilon &&
				std::fabs(Data.m3 - other.Data.m3) < epsilon &&
				std::fabs(Data.m4 - other.Data.m4) < epsilon &&
				std::fabs(Data.m5 - other.Data.m5) < epsilon &&
				std::fabs(Data.m6 - other.Data.m6) < epsilon &&
				std::fabs(Data.m7 - other.Data.m7) < epsilon &&
				std::fabs(Data.m8 - other.Data.m8) < epsilon &&
				std::fabs(Data.m9 - other.Data.m9) < epsilon &&
				std::fabs(Data.m10 - other.Data.m10) < epsilon &&
				std::fabs(Data.m11 - other.Data.m11) < epsilon &&
				std::fabs(Data.m12 - other.Data.m12) < epsilon &&
				std::fabs(Data.m13 - other.Data.m13) < epsilon &&
				std::fabs(Data.m14 - other.Data.m14) < epsilon &&
				std::fabs(Data.m15 - other.Data.m15) < epsilon;
		}

		static Matrix4 Identity() {
			Matrix4 mat;
			mat.Data.m0 = mat.Data.m5 = mat.Data.m10 = mat.Data.m15 = 1.0f;
			return mat;
		}

		static Matrix4 Transpose(const Matrix4& mat) {
			Matrix4 temp(mat), result;
			result.Data.m0 = temp.Data.m0;
			result.Data.m1 = temp.Data.m4;
			result.Data.m2 = temp.Data.m8;
			result.Data.m3 = temp.Data.m12;
			result.Data.m4 = temp.Data.m1;
			result.Data.m5 = temp.Data.m5;
			result.Data.m6 = temp.Data.m9;
			result.Data.m7 = temp.Data.m13;
			result.Data.m8 = temp.Data.m2;
			result.Data.m9 = temp.Data.m6;
			result.Data.m10 = temp.Data.m10;
			result.Data.m11 = temp.Data.m14;
			result.Data.m12 = temp.Data.m3;
			result.Data.m13 = temp.Data.m7;
			result.Data.m14 = temp.Data.m11;
			result.Data.m15 = temp.Data.m15;
			return result;
		}
	};
}
}

// 重载<<操作符来打印Matrix4
inline CASICLIB_API std::ostream& operator<<(std::ostream& os, const Casic::Math::Matrix4& mat) {
#ifdef CASIC_USE_STDFORMAT
	os << "Matrix4x4: " << std::endl;
	os << std::format("[ {:8.4f} {:8.4f} {:8.4f} {:8.4f} ]\n", mat.Data.m0, mat.Data.m4, mat.Data.m8, mat.Data.m12);
	os << std::format("[ {:8.4f} {:8.4f} {:8.4f} {:8.4f} ]\n", mat.Data.m1, mat.Data.m5, mat.Data.m9, mat.Data.m13);
	os << std::format("[ {:8.4f} {:8.4f} {:8.4f} {:8.4f} ]\n", mat.Data.m2, mat.Data.m6, mat.Data.m10, mat.Data.m14);
	os << std::format("[ {:8.4f} {:8.4f} {:8.4f} {:8.4f} ]\n", mat.Data.m3, mat.Data.m7, mat.Data.m11, mat.Data.m15);
#else
	std::ios old_state(nullptr);
	old_state.copyfmt(os);
	os << std::fixed << std::setprecision(4);
	os << "Matrix4x4: " << std::endl;
	os << "[ " << std::setw(8) << mat.Data.m0 << ", " << std::setw(8) << mat.Data.m4 << ", " << std::setw(8) << mat.Data.m8 << ", " << std::setw(8) << mat.Data.m12 << " ]" << std::endl;
	os << "[ " << std::setw(8) << mat.Data.m1 << ", " << std::setw(8) << mat.Data.m5 << ", " << std::setw(8) << mat.Data.m9 << ", " << std::setw(8) << mat.Data.m13 << " ]" << std::endl;
	os << "[ " << std::setw(8) << mat.Data.m2 << ", " << std::setw(8) << mat.Data.m6 << ", " << std::setw(8) << mat.Data.m10 << ", " << std::setw(8) << mat.Data.m14 << " ]" << std::endl;
	os << "[ " << std::setw(8) << mat.Data.m3 << ", " << std::setw(8) << mat.Data.m7 << ", " << std::setw(8) << mat.Data.m11 << ", " << std::setw(8) << mat.Data.m15 << " ]";
	os.copyfmt(old_state);
#endif
	return os;
}

// 重载 << 操作符来打印 Vector2
inline CASICLIB_API std::ostream& operator<<(std::ostream& os, const Casic::Math::Vector2& vec2) {
#ifdef CASIC_USE_STDFORMAT
	os << "Vector2: " << std::endl;
	os << std::format("[ {:8.4f} {:8.4f} ]\n", vec2.x, vec2.y);
#else
	std::ios old_state(nullptr);
	old_state.copyfmt(os);
	os << std::fixed << std::setprecision(4);
	os << "Vector2: " << std::endl;
	os << "[ " << std::setw(8) << vec2.x << ", " << std::setw(8) << vec2.y << " ]";
	os.copyfmt(old_state);
#endif
	return os;
}


// 重载 << 操作符来打印 Vector3
inline CASICLIB_API std::ostream& operator<<(std::ostream& os, const Casic::Math::Vector3& vec3) {
#ifdef CASIC_USE_STDFORMAT
	os << "Vector3: " << std::endl;
	os << std::format("[ {:8.4f} {:8.4f} {:8.4f} ]\n", vec3.x, vec3.y, vec3.z);
#else
	std::ios old_state(nullptr);
	old_state.copyfmt(os);
	os << std::fixed << std::setprecision(4);
	os << "Vector3: " << std::endl;
	os << "[ " << std::setw(8) << vec3.x << ", " << std::setw(8) << vec3.y << ", " << std::setw(8) << vec3.z << " ]";
	os.copyfmt(old_state);
#endif
	return os;
}

// 重载 << 操作符来打印 Vector4
inline CASICLIB_API std::ostream& operator<<(std::ostream& os, const Casic::Math::Vector4& vec4) {
#ifdef CASIC_USE_STDFORMAT
	os << "Vector4: " << std::endl;
	os << std::format("[ {:8.4f} {:8.4f} {:8.4f} {:8.4f} ]\n", vec4.x, vec4.y, vec4.z, vec4.w);
#else
	std::ios old_state(nullptr);
	old_state.copyfmt(os);
	os << std::fixed << std::setprecision(4);
	os << "Vector4: " << std::endl;
	os << "[ " << std::setw(8) << vec4.x << ", " << std::setw(8) << vec4.y << ", " << std::setw(8) << vec4.z << ", " << std::setw(8) << vec4.w << " ]";
	os.copyfmt(old_state);
#endif
	return os;
}
