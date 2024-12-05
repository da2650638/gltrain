#pragma once

// �жϱ������汾
#if __cplusplus >= 202002L
	#define CASIC_USE_STDFORMAT
	#define CASIC_USE_STDNUMBERS
#endif

// ������������
#ifdef CASICLIB_EXPORTS
#define CASICLIB_API __declspec(dllexport)  // ��������
#else
#define CASICLIB_API __declspec(dllimport)  // �������
#endif

#ifndef CASIC_USE_STDNUMBERS
#define CASIC_PI 3.14159265358979323846
#endif