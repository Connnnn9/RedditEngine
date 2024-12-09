#include "Precompiled.h"
#include "GameplayState.h"
#include "Common.h"

namespace KREngine
{
    void GameplayState::Initialize()
    {
    }

    void GameplayState::Terminate()
    {

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
            auto* shape = dynamic_cast<sf::Shape*>(shapes[currentShapeIndex]);
            if (shape)
            {
                sf::Vector2f position = shape->getPosition();
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

        currentShapeIndex = shapes.size() - 1;
        LOG("Circle added with index: %d", currentShapeIndex);
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

        currentShapeIndex = shapes.size() - 1;
        LOG("Triangle added with index: %d", currentShapeIndex);
    }


    void GameplayState::DeleteCurrentShape()
    {
        if (currentShapeIndex < 0 || currentShapeIndex >= static_cast<int>(shapes.size()))
        {
            LOG("No shape selected to delete.");
            return;
        }

        LOG("Deleting shape at index: %d", currentShapeIndex);

        auto& renderService = RenderService::GetInstance();

        // Remove the drawable from RenderService
        std::string shapeName = "shape" + std::to_string(currentShapeIndex);
        renderService.RemoveDrawable(shapeName);

        delete shapes[currentShapeIndex];

        shapes.erase(shapes.begin() + currentShapeIndex);

        for (size_t i = currentShapeIndex; i < shapes.size(); ++i)
        {
            std::string oldName = "shape" + std::to_string(i + 1);
            std::string newName = "shape" + std::to_string(i);
            renderService.RemoveDrawable(oldName);
            renderService.AddDrawable(newName, shapes[i]);
        }

        if (shapes.empty())
        {
            currentShapeIndex = -1;
        }
        else
        {
            currentShapeIndex = std::min(currentShapeIndex, static_cast<int>(shapes.size()) - 1);
        }

        LOG("Updated shape count: %d", static_cast<int>(shapes.size()));
        if (currentShapeIndex >= 0)
        {
            LOG("New current shape index: %d", currentShapeIndex);
        }
        else
        {
            LOG("No shapes left to select.");
        }
    }



    void GameplayState::SelectNextShape()
    {
        if (shapes.empty())
        {
            return;
        }

        currentShapeIndex = (currentShapeIndex + 1) % shapes.size();
    }
}



