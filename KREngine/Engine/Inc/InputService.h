#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <map>
#include "Common.h"

namespace KREngine
{
    class InputService
    {
    public:
        static InputService& GetInstance();

        void Initialize(sf::RenderWindow* window);
        void Update();
        bool IsKeyPressed(sf::Keyboard::Key key) const;
        bool IsMouseButtonPressed(sf::Mouse::Button button) const;
        sf::Vector2i GetMousePosition() const;

    private:
        InputService() = default;
        sf::RenderWindow* window = nullptr;
        std::map<sf::Keyboard::Key, bool> keyStates;
    };
}