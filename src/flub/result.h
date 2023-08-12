#pragma once
#include "../../../third-part/expected.h"

#define FLUB_RESULT_GET(var, ret, err_type)           \
    if (auto &&tmp = ret; tmp) var = std::move(*tmp); \
    else                                              \
        return flub::Err<err_type>(ret.error());

namespace flub {

    template<typename T, typename E>
    using Result = tl::expected<T, E>;

    template<typename E>
    using Err = tl::unexpected<E>;

}// namespace flub