#pragma once

#include <cstdint>
#include <cassert>

namespace KREngine
{
    class GameObject;

    class Component
    {
    public:
        Component() = default;
        virtual ~Component() = default;

        Component(const Component&) = delete;
        Component(Component&&) = delete;
        Component& operator=(const Component&) = delete;
        Component& operator=(Component&&) = delete;

        virtual void Initialize() {}
        virtual void Terminate() {}
        virtual void Update(float deltaTime) {}
        virtual void DebugUI() {}

        virtual uint32_t GetTypeId() const = 0;

        GameObject& GetOwner()
        {
            assert(mOwner && "Component: Owner is null!");
            return *mOwner;
        }

        const GameObject& GetOwner() const
        {
            assert(mOwner && "Component: Owner is null!");
            return *mOwner;
        }

    private:
        friend class GameObject;
        GameObject* mOwner = nullptr;
    };
}
