#include "Precompiled.h"
#include "UIService.h"
#include <iostream>

namespace KREngine
{
    UIService& UIService::GetInstance()
    {
        static UIService instance;
        return instance;
    }

    void UIService::Initialize(sf::RenderWindow& window)
    {
        std::cout << "[LOG] Initializing UIService..." << std::endl;
        mUIManager.Initialize(window);
        std::cout << "[LOG] UIService initialized." << std::endl;
    }

    void UIService::Terminate()
    {
        std::cout << "[LOG] Terminating UIService..." << std::endl;
        mUIManager.Terminate();
        std::cout << "[LOG] UIService terminated." << std::endl;
    }

    void UIService::Update(sf::RenderWindow& window, sf::Time deltaTime)
    {
        mUIManager.Update(window, deltaTime);
    }

    void UIService::Render(sf::RenderWindow& window)
    {
        mUIManager.Render(window);
    }

    UI::UIManager& UIService::GetUIManager()
    {
        return mUIManager;
    }
}
