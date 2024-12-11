#include "Precompiled.h"
#include "GameObject.h"
#include "Component.h"

namespace KREngine
{
    GameObject::~GameObject()
    {
        Terminate();
    }

    void GameObject::Initialize()
    {
        for (auto& [type, component] : mComponents)
        {
            component->Initialize();
        }
    }

    void GameObject::Terminate()
    {
        for (auto& [type, component] : mComponents)
        {
            component->Terminate();
        }
        mComponents.clear();
    }
}
