#include "Precompiled.h"
#include "InputComponent.h"

namespace KREngine
{
    void InputComponent::BindKey(sf::Keyboard::Key key, InputCallback callback)
    {
        mKeyBindings[key] = std::move(callback);
    }

    void InputComponent::ProcessInput(const sf::Event& event)
    {
        if (event.type == sf::Event::KeyPressed)
        {
            auto it = mKeyBindings.find(event.key.code);
            if (it != mKeyBindings.end())
            {
                it->second();
            }
        }
    }
}
