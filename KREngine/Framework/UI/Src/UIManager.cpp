#include "Precompiled.h"
#include "UIManager.h"
#include <imgui.h>
#include <imgui-SFML.h>
#include <iostream>

namespace KREngine
{
    namespace UI
    {
        void UIManager::Initialize(sf::RenderWindow& window)
        {
            ImGui::SFML::Init(window);
            std::cout << "[LOG] UIManager initialized." << std::endl;

            // Example: Adding a circle to the scene
            auto* circle = new sf::CircleShape(50.0f);
            circle->setFillColor(sf::Color::Green);
            circle->setPosition(200.0f, 200.0f);
            mShapes["Circle"] = circle;
        }

        void UIManager::Terminate()
        {
            for (auto& pair : mShapes)
            {
                delete pair.second;
            }
            mShapes.clear();

            ImGui::SFML::Shutdown();
            std::cout << "[LOG] UIManager terminated." << std::endl;
        }

        void UIManager::Update(sf::RenderWindow& window, sf::Time deltaTime)
        {
            ImGui::SFML::Update(window, deltaTime);
        }

        void UIManager::Render(sf::RenderWindow& window)
        {
            // Render UI elements
            RenderMainMenu();
            RenderHierarchy();
            RenderInspector();

            // Draw shapes
            for (const auto& pair : mShapes)
            {
                sf::Shape* shape = pair.second;
                if (shape)
                {
                    window.draw(*shape);
                }
            }

            ImGui::SFML::Render(window);
        }

        void UIManager::RenderMainMenu()
        {
            if (ImGui::BeginMainMenuBar())
            {
                if (ImGui::BeginMenu("File"))
                {
                    if (ImGui::MenuItem("Exit"))
                    {
                        std::exit(0);
                    }
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("Edit"))
                {
                    if (ImGui::MenuItem("Add Circle"))
                    {
                        auto* circle = new sf::CircleShape(50.0f);
                        circle->setFillColor(sf::Color::Blue);
                        circle->setPosition(100.0f, 100.0f);
                        mShapes["Circle " + std::to_string(mShapes.size() + 1)] = circle;
                    }
                    if (ImGui::MenuItem("Add Square"))
                    {
                        auto* square = new sf::RectangleShape(sf::Vector2f(100.0f, 100.0f));
                        square->setFillColor(sf::Color::Red);
                        square->setPosition(300.0f, 100.0f);
                        mShapes["Square " + std::to_string(mShapes.size() + 1)] = square;
                    }
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("Help"))
                {
                    ImGui::Text("This is a simple UI demo.");
                    ImGui::EndMenu();
                }
                ImGui::EndMainMenuBar();
            }
        }

        void UIManager::RenderHierarchy()
        {
            ImGui::Begin("Hierarchy");

            for (auto& pair : mShapes)
            {
                const std::string& name = pair.first; // Access the name
                if (ImGui::Selectable(name.c_str(), name == mSelectedShape))
                {
                    mSelectedShape = name;
                    std::cout << "[LOG] Selected shape: " << name << std::endl;
                }
            }

            ImGui::End();
        }

        void UIManager::RenderInspector()
        {
            ImGui::Begin("Inspector");

            if (!mSelectedShape.empty())
            {
                auto* shape = mShapes[mSelectedShape];

                ImGui::Text("Selected: %s", mSelectedShape.c_str());

                sf::Vector2f position = shape->getPosition();
                if (ImGui::DragFloat2("Position", &position.x, 1.0f))
                {
                    shape->setPosition(position);
                }

                sf::Color color = shape->getFillColor();
                float colorArr[4] = { color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f };
                if (ImGui::ColorEdit4("Color", colorArr))
                {
                    shape->setFillColor(sf::Color(colorArr[0] * 255, colorArr[1] * 255, colorArr[2] * 255, colorArr[3] * 255));
                }
            }
            else
            {
                ImGui::Text("No shape selected.");
            }

            ImGui::End();
        }
    }
}
