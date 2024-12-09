#include "Precompiled.h"
#include "UIService.h"

using namespace KREngine::UI;

namespace KREngine
{
    UIService& UIService::GetInstance()
    {
        static UIService instance;
        return instance;
    }

    void UIService::Initialize()
    {
        mUIManager.Initialize();
    }

    void UIService::Terminate()
    {
        mUIManager.Terminate();
    }

    void UIService::Update(sf::RenderWindow& window)
    {
        mUIManager.Update(window);
    }

    void UIService::Render()
    {
        mUIManager.Render();
    }

    UIManager& UIService::GetUIManager()
    {
        return mUIManager;
    }
}
