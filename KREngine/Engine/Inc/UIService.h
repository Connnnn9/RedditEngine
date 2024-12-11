#pragma once

#include "UIManager.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Time.hpp>

namespace KREngine
{
    class UIService
    {
    public:
        static UIService& GetInstance();

        void Initialize(sf::RenderWindow& window);
        void Terminate();
        void Update(sf::RenderWindow& window, sf::Time deltaTime);
        void Render(sf::RenderWindow& window);

        UI::UIManager& GetUIManager();

    private:
        UIService() = default;
        ~UIService() = default;

        UI::UIManager mUIManager;
    };
}
