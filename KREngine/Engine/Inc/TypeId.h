#pragma once

#include <cstdint>

namespace KREngine
{
    enum class ComponentId : uint32_t
    {
        Invalid = 0,
        Transform,
        Render,
        Input,
        Physics,
    };

    enum class ServiceId : uint32_t
    {
        Invalid = 0,
        UI,
        Render,
        Input,
        Physics,
    };
}
