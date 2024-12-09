#include "UIService.h"

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

    void UIService::Update()
    {
        mUIManager.Update();
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
