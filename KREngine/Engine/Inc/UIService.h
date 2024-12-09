#pragma once

#include "UIManager.h"

namespace KREngine
{
    class UIService
    {
    public:
        static UIService& GetInstance();

        void Initialize();
        void Terminate();
        void Update();
        void Render();

        UIManager& GetUIManager();

    private:
        UIManager mUIManager;
    };
}
