#include "Precompiled.h"
#include "Service.h"
#include "GameWorld.h"

namespace KREngine
{
    GameWorld& Service::GetWorld()
    {
        ASSERT(mWorld != nullptr, "Service: world is null.");
        return *mWorld;
    }

    const GameWorld& Service::GetWorld() const
    {
        ASSERT(mWorld != nullptr, "Service: world is null.");
        return *mWorld;
    }
}
