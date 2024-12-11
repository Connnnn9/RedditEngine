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
        ASSERT(renderWindow, "InputService: Render window is null during initialization!");
        window = renderWindow;
        LOG("InputService -- Initialized");
    }

    void InputService::Update()
    {
        ASSERT(window, "InputService: Render window is null during update!");

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

    bool InputService::IsKeyPressed(sf::Keyboard::Key key) const
    {
        auto it = keyStates.find(key);
        return it != keyStates.end() && it->second;
    }

    bool InputService::IsMouseButtonPressed(sf::Mouse::Button button) const
    {
        return sf::Mouse::isButtonPressed(button);
    }

    sf::Vector2i InputService::GetMousePosition() const
    {
        ASSERT(window, "InputService: Render window is null for mouse position!");
        return sf::Mouse::getPosition(*window);
    }
}
