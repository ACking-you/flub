#pragma once
#if __cplusplus >= 201703L || (_MSC_VER && _MSVC_LANG >= 201703L)
#include <string_view>
#else
#include <string_view.h>
#endif

namespace flub {
#if __cplusplus >= 201703L || (_MSC_VER && _MSVC_LANG >= 201703L)
using StrRef = std::string_view;
#else
using StrRef = nonstd::string_view;
#endif
}   // namespace flub