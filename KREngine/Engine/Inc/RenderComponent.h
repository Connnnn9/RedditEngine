#pragma once

#include "Component.h"
#include <SFML/Graphics/Drawable.hpp>

namespace KREngine
{
    class RenderComponent : public Component
    {
    public:
        uint32_t GetTypeId() const override { return 2; } // Unique ID for RenderComponent

        void Initialize() override {}
        void Terminate() override {}
        void DebugUI() override;
        void Update(float deltaTime) override {}

        void SetDrawable(sf::Drawable* drawable) { mDrawable = drawable; }
        sf::Drawable* GetDrawable() const { return mDrawable; }

    private:
        sf::Drawable* mDrawable = nullptr;
    };
}
