#pragma once
#include "../../third-part/string_view.h"

namespace flub {

#if __cplusplus >= 201703L || (_MSC_VER && _MSVC_LANG >= 201703L)
    using StrRef = std::string_view;
#else
    using StrRef = nonstd::string_view;
#endif
}// namespace flub