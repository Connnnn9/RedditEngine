#include "Precompiled.h"
#include "GameWorld.h"
#include "Component.h"

#include <numeric>

namespace KREngine
{
    GameWorld::~GameWorld()
    {
        Terminate();
    }

    void GameWorld::Initialize(uint32_t capacity)
    {
        ASSERT(!mInitialized, "GameWorld: already initialized.");

        mGameObjectSlots.resize(capacity);
        mFreeSlots.resize(capacity);
        std::iota(mFreeSlots.begin(), mFreeSlots.end(), 0);

        for (auto& service : mServices)
        {
            service->Initialize();
        }

        mInitialized = true;
    }

    void GameWorld::Terminate()
    {
        if (!mInitialized && mServices.empty() && mGameObjectSlots.empty())
        {
            return;
        }

        for (Slot& slot : mGameObjectSlots)
        {
            if (slot.gameObject != nullptr)
            {
                slot.gameObject->Terminate();
                slot.gameObject.reset();
                ++slot.generation;
            }
        }

        for (auto& service : mServices)
        {
            service->Terminate();
        }

        mServices.clear();
        mGameObjectSlots.clear();
        mFreeSlots.clear();
        mToBeDestroyed.clear();
        mInitialized = false;
    }

    void GameWorld::Update(float deltaTime)
    {
        ASSERT(mInitialized, "GameWorld: not initialized.");

        for (auto& service : mServices)
        {
            service->Update(deltaTime);
        }

        for (Slot& slot : mGameObjectSlots)
        {
            if (slot.gameObject != nullptr)
            {
                slot.gameObject->Update(deltaTime);
            }
        }

        ProcessDestroyList();
    }

    void GameWorld::Render()
    {
        ASSERT(mInitialized, "GameWorld: not initialized.");

        for (auto& service : mServices)
        {
            service->Render();
        }
    }

    void GameWorld::DebugUI()
    {
        ASSERT(mInitialized, "GameWorld: not initialized.");

        for (Slot& slot : mGameObjectSlots)
        {
            if (slot.gameObject != nullptr)
            {
                slot.gameObject->DebugUI();
            }
        }

        for (auto& service : mServices)
        {
            service->DebugUI();
        }
    }

    GameObject* GameWorld::CreateGameObject(const std::string& name)
    {
        ASSERT(mInitialized, "GameWorld: must be initialized before creating objects.");
        ASSERT(!mFreeSlots.empty(), "GameWorld: no free object slots left.");

        const uint32_t slotIndex = mFreeSlots.back();
        mFreeSlots.pop_back();

        Slot& slot = mGameObjectSlots[slotIndex];
        slot.gameObject = std::make_unique<GameObject>();

        GameObject* gameObject = slot.gameObject.get();
        gameObject->mWorld = this;
        gameObject->mName = name.empty() ? "GameObject_" + std::to_string(slotIndex) : name;
        gameObject->mHandle.mWorld = this;
        gameObject->mHandle.mIndex = slotIndex;
        gameObject->mHandle.mGeneration = slot.generation;
        return gameObject;
    }

    GameObject* GameWorld::GetGameObject(const std::string& name)
    {
        for (Slot& slot : mGameObjectSlots)
        {
            if (slot.gameObject != nullptr && slot.gameObject->GetName() == name)
            {
                return slot.gameObject.get();
            }
        }
        return nullptr;
    }

    GameObject* GameWorld::GetGameObject(const GameObjectHandle& handle)
    {
        return IsValid(handle) ? mGameObjectSlots[handle.mIndex].gameObject.get() : nullptr;
    }

    const GameObject* GameWorld::GetGameObject(const GameObjectHandle& handle) const
    {
        return IsValid(handle) ? mGameObjectSlots[handle.mIndex].gameObject.get() : nullptr;
    }

    void GameWorld::DestroyGameObject(const GameObjectHandle& handle)
    {
        if (!IsValid(handle))
        {
            return;
        }

        Slot& slot = mGameObjectSlots[handle.mIndex];
        ++slot.generation;
        mToBeDestroyed.push_back(handle.mIndex);
    }

    bool GameWorld::IsValid(const GameObjectHandle& handle) const
    {
        if (handle.mWorld != this || handle.mIndex >= mGameObjectSlots.size())
        {
            return false;
        }

        const Slot& slot = mGameObjectSlots[handle.mIndex];
        return slot.gameObject != nullptr && slot.generation == handle.mGeneration;
    }

    void GameWorld::ProcessDestroyList()
    {
        for (uint32_t index : mToBeDestroyed)
        {
            Slot& slot = mGameObjectSlots[index];
            if (slot.gameObject != nullptr)
            {
                slot.gameObject->Terminate();
                slot.gameObject.reset();
                mFreeSlots.push_back(index);
            }
        }

        mToBeDestroyed.clear();
    }
}
