#pragma once

#include <unordered_map>
#include <typeindex>
#include <memory>
#include <cassert>
#include <string>
#include <utility>

#include "GameObjectHandle.h"

namespace KREngine
{
    class Component;
    class GameWorld;

    class GameObject
    {
    public:
        GameObject() = default;
        ~GameObject();

        GameObject(const GameObject&) = delete;
        GameObject(GameObject&&) = delete;
        GameObject& operator=(const GameObject&) = delete;
        GameObject& operator=(GameObject&&) = delete;

        template <typename T, typename... Args>
        T& AddComponent(Args&&... args)
        {
            auto type = std::type_index(typeid(T));
            assert(mComponents.find(type) == mComponents.end() && "Component already exists!");

            auto component = std::make_unique<T>(std::forward<Args>(args)...);
            component->mOwner = this;

            T& reference = *component;
            mComponents[type] = std::move(component);
            return reference;
        }

        template <typename T>
        T* GetComponent() const
        {
            auto type = std::type_index(typeid(T));
            auto it = mComponents.find(type);
            if (it != mComponents.end())
            {
                return static_cast<T*>(it->second.get());
            }
            return nullptr;
        }

        void Initialize();
        void Terminate();
        void Update(float deltaTime);
        void DebugUI();

        const std::string& GetName() const { return mName; }
        void SetName(std::string name) { mName = std::move(name); }

        const GameObjectHandle& GetHandle() const { return mHandle; }
        GameWorld* GetWorld() { return mWorld; }
        const GameWorld* GetWorld() const { return mWorld; }

    private:
        friend class GameWorld;

        std::unordered_map<std::type_index, std::unique_ptr<Component>> mComponents;
        std::string mName = "GameObject";
        GameObjectHandle mHandle;
        GameWorld* mWorld = nullptr;
    };
}
