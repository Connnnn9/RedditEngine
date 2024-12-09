#pragma once

#include "AppState.h"
#include "RenderService.h"
#include "InputService.h"

namespace KREngine
{
    class GameplayState : public AppState
    {
    public:
        void Initialize() override;
        void Terminate() override;
        void Update() override;
        void Render() override;

    private:
        std::vector<sf::Drawable*> shapes;
        int currentShapeIndex = -1;
        float speed = 10.0f;

        void AddCircle();
        void AddTriangle();
        void DeleteCurrentShape();
        void SelectNextShape();
    };
}

