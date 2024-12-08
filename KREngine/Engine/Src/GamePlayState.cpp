#include "Precompiled.h"
#include "GameplayState.h"
#include "Common.h"

namespace KREngine {
    void GameplayState::Initialize() {
        LOG("GameplayState -- Initialized");

        auto& renderService = RenderService::GetInstance();
        sf::CircleShape* circle = new sf::CircleShape(50);
        circle->setFillColor(sf::Color::Green);
        circle->setPosition(300, 200);

        renderService.AddDrawable(circleName, circle);
    }

    void GameplayState::Terminate() {
        LOG("GameplayState -- Terminated");

        auto& renderService = RenderService::GetInstance();
        renderService.RemoveDrawable(circleName);
    }

    void GameplayState::Update() {
        LOG("GameplayState -- Updating");
    }

    void GameplayState::Render() {
        LOG("GameplayState -- Rendering");
        RenderService::GetInstance().Render();
    }
}
