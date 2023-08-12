#pragma once

#if __cplusplus >= 201703L || (_MSC_VER && _MSVC_LANG >= 201703L)
#include <optional>
namespace flub {
    template<typename T>
    using Option = std::optional<T>;
}
// if cpp version less than 17
#else
#include "../../../third-part/optional.h"
namespace flub {
    template<typename T>
    using Option = tl::optional<T>;
}
#endif
