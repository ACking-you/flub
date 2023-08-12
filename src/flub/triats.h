#pragma once
#include <type_traits>

namespace flub {
    // checks if T has to_string method
    template<typename T, typename... Args>
    struct has_to_string {
    private:
        template<typename U>
        static auto Check(int)
                -> decltype(std::declval<U>().to_string(std::declval<Args>()...),
                            std::true_type()// 1
                );
        template<typename U>
        static std::false_type Check(...);// 2

    public:
        enum {
            value = std::is_same<decltype(Check<T>(0)), std::true_type>::value// 3
        };
    };
}// namespace flub