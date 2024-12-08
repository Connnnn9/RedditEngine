#include "Precompiled.h"
#include "GameplayState.h"
#include "Common.h"

namespace KREngine
{
    void GameplayState::Initialize()
    {
        LOG("GameplayState -- Initializing...");

        auto& renderService = RenderService::GetInstance();

        LOG("GameplayState -- Ready for user input to add shapes.");
    }

    void GameplayState::Terminate()
    {
        LOG("GameplayState -- Terminating...");

        auto& renderService = RenderService::GetInstance();

        for (size_t i = 0; i < shapes.size(); ++i)
        {
            renderService.RemoveDrawable("shape" + std::to_string(i));
            delete shapes[i];
        }
        shapes.clear();
    }

    void GameplayState::Update()
    {
        LOG("GameplayState -- Updating...");
        auto& input = InputService::GetInstance();

        if (input.IsKeyPressed(sf::Keyboard::Num1))
        {
            AddCircle();
        }
        if (input.IsKeyPressed(sf::Keyboard::Num2))
        {
            AddTriangle();
        }

        if (currentShapeIndex >= 0 && currentShapeIndex < static_cast<int>(shapes.size()))
        {
            auto* drawable = shapes[currentShapeIndex];

            auto* shape = dynamic_cast<sf::Shape*>(drawable);
            if (shape)
            {
                sf::Vector2f position = shape->getPosition();
                LOG("Current Shape Position: %f, %f", position.x, position.y);

                float deltaTime = 0.016f;
                if (input.IsKeyPressed(sf::Keyboard::Up))
                {
                    position.y -= speed * deltaTime;
                }
                if (input.IsKeyPressed(sf::Keyboard::Down))
                {
                    position.y += speed * deltaTime;
                }
                if (input.IsKeyPressed(sf::Keyboard::Left))
                {
                    position.x -= speed * deltaTime;
                }
                if (input.IsKeyPressed(sf::Keyboard::Right))
                {
                    position.x += speed * deltaTime;
                }

                shape->setPosition(position);
                LOG("Updated Shape Position: %f, %f", position.x, position.y);
            }
            else
            {
                LOG("Selected shape is not movable.");
            }
        }

        if (input.IsKeyPressed(sf::Keyboard::Tab))
        {
            SelectNextShape();
        }
    }


    void GameplayState::Render()
    {
        LOG("GameplayState -- Rendering...");
        RenderService::GetInstance().Render();
    }

    void GameplayState::AddCircle()
    {
        LOG("Adding a Circle...");
        auto* circle = new sf::CircleShape(50);
        circle->setFillColor(sf::Color::Green);
        circle->setPosition(300, 200);

        shapes.push_back(circle);
        RenderService::GetInstance().AddDrawable("shape" + std::to_string(shapes.size() - 1), circle);
    }

    void GameplayState::AddTriangle()
    {
        LOG("Adding a Triangle...");
        auto* triangle = new sf::ConvexShape(3);
        triangle->setPoint(0, sf::Vector2f(0, 0));
        triangle->setPoint(1, sf::Vector2f(50, 100));
        triangle->setPoint(2, sf::Vector2f(100, 0));
        triangle->setFillColor(sf::Color::Blue);
        triangle->setPosition(300, 200);

        shapes.push_back(triangle);
        RenderService::GetInstance().AddDrawable("shape" + std::to_string(shapes.size() - 1), triangle);
    }

    void GameplayState::SelectNextShape()
    {
        if (shapes.empty())
        {
            LOG("No shapes to select.");
            return;
        }

        currentShapeIndex = (currentShapeIndex + 1) % shapes.size();
        LOG("Selected next shape: %d", currentShapeIndex);
    }
}


