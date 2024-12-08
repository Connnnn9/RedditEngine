#pragma once

#include "AppState.h"
#include "RenderService.h"

namespace KREngine {
    class GameplayState : public AppState 
    {
    public:
        void Initialize() override;
        void Terminate() override;
        void Update() override;
        void Render() override;

    private:
        std::string circleName = "circle";
    };
}
