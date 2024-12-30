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

#include "CasicVector.h"
#include "CasicMatrix.h"

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
