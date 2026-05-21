#pragma once

#include "Common.h"
#include "TypeId.h"

namespace KREngine
{
    class GameWorld;

    class Service
    {
    public:
        Service() = default;
        virtual ~Service() = default;

        Service(const Service&) = delete;
        Service(Service&&) = delete;
        Service& operator=(const Service&) = delete;
        Service& operator=(Service&&) = delete;

        virtual uint32_t GetTypeId() const = 0;

        virtual void Initialize() {}
        virtual void Terminate() {}
        virtual void Update(float deltaTime) {}
        virtual void Render() {}
        virtual void DebugUI() {}

        GameWorld& GetWorld();
        const GameWorld& GetWorld() const;

    private:
        friend class GameWorld;
        GameWorld* mWorld = nullptr;
    };
}
