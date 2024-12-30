#pragma once

#include "CasicMath.h"

namespace Casic
{
namespace Math
{
	typedef struct CASICLIB_API Matrix4 {
		typedef struct {
			// Column major memory layout: [m0, m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, m12, m13, m14, m15]
			// Row major memory layout:	   [m0, m4, m8, m12, m1, m5, m9, m13, m2, m6, m10, m14, m3, m7, m11, m15]
			float m0, m4, m8, m12;	// 1st row
			float m1, m5, m9, m13;  // 2nd row
			float m2, m6, m10, m14;  // 3rd row
			float m3, m7, m11, m15;  // 4th row
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