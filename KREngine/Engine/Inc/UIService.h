#pragma once

#include "../Framework/UI/Inc/UIManager.h"
#include <SFML/Graphics/RenderWindow.hpp>

namespace KREngine
{
    class UIService
    {
    public:
        static UIService& GetInstance();

        void Initialize();
        void Terminate();
        void Update(sf::RenderWindow& window);
        void Render();

        UI::UIManager& GetUIManager();

    private:
        UI::UIManager mUIManager;
    };
}
