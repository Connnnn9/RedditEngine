#pragma once

#include "Common.h"
#include "GameObject.h"
#include "GameObjectHandle.h"
#include "Service.h"

#include <type_traits>

namespace KREngine
{
    class GameWorld final
    {
    public:
        GameWorld() = default;
        ~GameWorld();

        GameWorld(const GameWorld&) = delete;
        GameWorld(GameWorld&&) = delete;
        GameWorld& operator=(const GameWorld&) = delete;
        GameWorld& operator=(GameWorld&&) = delete;

        void Initialize(uint32_t capacity = 128);
        void Terminate();

        void Update(float deltaTime);
        void Render();
        void DebugUI();

        GameObject* CreateGameObject(const std::string& name = {});
        GameObject* GetGameObject(const std::string& name);
        GameObject* GetGameObject(const GameObjectHandle& handle);
        const GameObject* GetGameObject(const GameObjectHandle& handle) const;
        void DestroyGameObject(const GameObjectHandle& handle);

        bool IsInitialized() const { return mInitialized; }
        bool IsValid(const GameObjectHandle& handle) const;

        template<class ServiceType, class... Args>
        ServiceType* AddService(Args&&... args)
        {
            static_assert(std::is_base_of_v<Service, ServiceType>, "GameWorld: ServiceType must inherit from Service.");
            ASSERT(!mInitialized, "GameWorld: services must be added before Initialize().");

            auto& service = mServices.emplace_back(std::make_unique<ServiceType>(std::forward<Args>(args)...));
            service->mWorld = this;
            return static_cast<ServiceType*>(service.get());
        }

        template<class ServiceType>
        ServiceType* GetService()
        {
            static_assert(std::is_base_of_v<Service, ServiceType>, "GameWorld: ServiceType must inherit from Service.");
            for (auto& service : mServices)
            {
                if (service->GetTypeId() == ServiceType::StaticGetTypeId())
                {
                    return static_cast<ServiceType*>(service.get());
                }
            }
            return nullptr;
        }

        template<class ServiceType>
        const ServiceType* GetService() const
        {
            return const_cast<GameWorld*>(this)->GetService<ServiceType>();
        }

    private:
        struct Slot
        {
            std::unique_ptr<GameObject> gameObject;
            uint32_t generation = 0;
        };

        void ProcessDestroyList();

        std::vector<std::unique_ptr<Service>> mServices;
        std::vector<Slot> mGameObjectSlots;
        std::vector<uint32_t> mFreeSlots;
        std::vector<uint32_t> mToBeDestroyed;
        bool mInitialized = false;
    };
}
