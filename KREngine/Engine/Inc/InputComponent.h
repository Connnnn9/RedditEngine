#pragma once

#include "Component.h"
#include <SFML/Window/Event.hpp>
#include <functional>
#include <unordered_map>

namespace KREngine
{
    class InputComponent : public Component
    {
    public:
        uint32_t GetTypeId() const override { return 3; } // Unique ID for InputComponent

        using InputCallback = std::function<void()>;

        void Initialize() override {}
        void Terminate() override {}
        void Update(float deltaTime) override {}

        void BindKey(sf::Keyboard::Key key, InputCallback callback);
        void ProcessInput(const sf::Event& event);

    private:
        using InputCallback = std::function<void()>;
        std::unordered_map<sf::Keyboard::Key, InputCallback> mKeyBindings;
    };
}
