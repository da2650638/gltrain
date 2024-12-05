#pragma once

// 判断编译器版本
#if __cplusplus >= 202002L
	#define CASIC_USE_STDFORMAT
	#define CASIC_USE_STDNUMBERS
#endif

// 方法导出设置
#ifdef CASICLIB_EXPORTS
#define CASICLIB_API __declspec(dllexport)  // 导出符号
#else
#define CASICLIB_API __declspec(dllimport)  // 导入符号
#endif

#ifndef CASIC_USE_STDNUMBERS
#define CASIC_PI 3.14159265358979323846
#endif