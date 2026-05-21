#pragma once

#include <cstdint>

namespace KREngine
{
    class GameObject;
    class GameWorld;

    class GameObjectHandle
    {
    public:
        bool IsValid() const;
        GameObject* Get() const;

    private:
        friend class GameWorld;

        GameWorld* mWorld = nullptr;
        uint32_t mIndex = 0;
        uint32_t mGeneration = 0;
    };
}
