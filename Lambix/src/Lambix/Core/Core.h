#pragma once

#include <memory>
// 断言
#ifdef LB_ENABLE_ASSERTS
#define LB_CLIENT_ASSERT(x, ...)                                   \
    {                                                              \
        if (!(x))                                                  \
        {                                                          \
            LB_CLIENT_ERROR("Assertion Failed: {0}", __VA_ARGS__); \
            __debugbreak();                                        \
        }                                                          \
    }
#define LB_CORE_ASSERT(x, ...)                                   \
    {                                                            \
        if (!(x))                                                \
        {                                                        \
            LB_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); \
            __debugbreak();                                      \
        }                                                        \
    }
#else
#define LB_CLIENT_ASSERT(x, ...)
#define LB_CORE_ASSERT(x, ...)
#endif

// 处理事件大类枚举，使得可以组合
#define BIT(x) (1 << x)

// 绑定事件函数
#define LB_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

namespace Lambix
{
    template<typename T>
    using Scope = std::unique_ptr<T>;

    template<typename T>
    using Ref = std::shared_ptr<T>;
}