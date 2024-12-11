#pragma once

#include "Component.h"
#include <SFML/System/Vector2.hpp>

namespace KREngine
{
    class TransformComponent : public Component
    {
    public:
        uint32_t GetTypeId() const override { return 1; } // Unique ID for TransformComponent

        void Initialize() override {}
        void Terminate() override {}
        void DebugUI() override;
        void Update(float deltaTime) override {}

        void SetPosition(const sf::Vector2f& position) { mPosition = position; }
        const sf::Vector2f& GetPosition() const { return mPosition; }

    private:
        sf::Vector2f mPosition = { 0.0f, 0.0f };
    };
}
