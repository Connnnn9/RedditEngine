#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <map>

namespace KREngine {
    class InputService {
    public:
        static InputService& GetInstance();

        void Initialize(sf::RenderWindow* window);
        void Update();
        bool IsKeyPressed(sf::Keyboard::Key key);
        bool IsMouseButtonPressed(sf::Mouse::Button button);
        sf::Vector2i GetMousePosition();

    private:
        InputService() = default;
        sf::RenderWindow* window = nullptr;
        std::map<sf::Keyboard::Key, bool> keyStates;
    };
}
