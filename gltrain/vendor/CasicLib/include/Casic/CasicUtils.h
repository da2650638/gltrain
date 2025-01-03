#pragma once

#include "CasicConfig.h"

#include <cstdlib>
#include <type_traits>
#include <exception>
#include <tuple>

namespace Casic
{
namespace Util
{
	// TODO: 不能传void，必须解决这个问题
	template <typename _Ty, int _DefaultSize = sizeof(std::remove_cv_t<std::remove_pointer_t<_Ty>>)>
	typename std::remove_cv_t<std::remove_pointer_t<_Ty>>* Allocate(size_t size = _DefaultSize)
	{
		static_assert(!std::is_reference<_Ty>::value, "Type _Ty cannot be a reference");
		using BaseType = typename std::remove_cv_t<std::remove_pointer_t<_Ty>>;

		void* raw = ::malloc(size);
		if (!raw)
		{
			throw std::bad_alloc();
		}

		return static_cast <BaseType*>(raw);
	}

	CASICLIB_API void Deallocate(void* ptr);

	CASICLIB_API std::tuple<unsigned char*, int> LoadFileData(const char* filename);
	CASICLIB_API void UnloadFileData(unsigned char* filedata); 
}
}