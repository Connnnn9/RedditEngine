#include "Precompiled.h"
#include "InputService.h"
#include "Common.h"

namespace KREngine
{
    InputService& InputService::GetInstance()
    {
        static InputService instance;
        return instance;
    }

    void InputService::Initialize(sf::RenderWindow* renderWindow)
    {
        window = renderWindow;
        LOG("InputService -- Initialized");
    }

    void InputService::Update()
    {
        if (!window) return;

        sf::Event event;
        while (window->pollEvent(event))
        {
            if (event.type == sf::Event::KeyPressed)
            {
                keyStates[event.key.code] = true;
            }
            else if (event.type == sf::Event::KeyReleased)
            {
                keyStates[event.key.code] = false;
            }
            else if (event.type == sf::Event::Closed)
            {
                window->close();
            }
        }
    }

    bool InputService::IsKeyPressed(sf::Keyboard::Key key)
    {
        return keyStates[key];
    }

    bool InputService::IsMouseButtonPressed(sf::Mouse::Button button)
    {
        return sf::Mouse::isButtonPressed(button);
    }

    sf::Vector2i InputService::GetMousePosition()
    {
        if (window)
        {
            return sf::Mouse::getPosition(*window);
        }
        return { 0, 0 };
    }
}
