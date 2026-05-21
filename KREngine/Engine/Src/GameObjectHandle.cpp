#include "Precompiled.h"
#include "GameObjectHandle.h"
#include "GameWorld.h"

namespace KREngine
{
    bool GameObjectHandle::IsValid() const
    {
        return mWorld != nullptr && mWorld->IsValid(*this);
    }

    GameObject* GameObjectHandle::Get() const
    {
        return mWorld != nullptr ? mWorld->GetGameObject(*this) : nullptr;
    }
}
