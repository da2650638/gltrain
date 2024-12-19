#pragma once

#include "CasicConfig.h"
#include "CasicMath.h"

#include <algorithm>

namespace Casic
{
namespace Graphics
{
	typedef struct CASICLIB_API OpenGLMatrix4 {
		float v[16];
	} OpenGLMatrix4;

	static inline OpenGLMatrix4 ToOpenGLMatrix4(const Math::Matrix4& mat) {
		OpenGLMatrix4 result;
		result.v[0] = mat.Data.m0;
		result.v[1] = mat.Data.m1;
		result.v[2] = mat.Data.m2;
		result.v[3] = mat.Data.m3;
		result.v[4] = mat.Data.m4;
		result.v[5] = mat.Data.m5;
		result.v[6] = mat.Data.m6;
		result.v[7] = mat.Data.m7;
		result.v[8] = mat.Data.m8;
		result.v[9] = mat.Data.m9;
		result.v[10] = mat.Data.m10;
		result.v[11] = mat.Data.m11;
		result.v[12] = mat.Data.m12;
		result.v[13] = mat.Data.m13;
		result.v[14] = mat.Data.m14;
		result.v[15] = mat.Data.m15;
		return result;
	}

	typedef struct CASICLIB_API Color {
		unsigned char r; 
		unsigned char g; 
		unsigned char b; 
		unsigned char a;

		Color()
			: r(0), g(0), b(0), a(255) {}

		Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255)
			: r(r), g(g), b(b), a(a) {}

		bool operator==(const Color& other) const {
			return r == other.r && g == other.g && b == other.b && a == other.a;
		}

		bool operator!=(const Color& other) const {
			return !(*this == other);
		}

		operator Math::Vector4() const {
			Math::Vector4 result{ r / 255.f, g / 255.f, b / 255.f, a / 255.f };
			return result;
		}
	} Color;

	static inline Math::Vector4 ColorToVec4(Color color) {
		return Math::Vector4(color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f);
	}

	// Rectangle, 4 components
	struct Rectangle {
		float x;
		float y;
		float width;
		float height;
	};
}
}