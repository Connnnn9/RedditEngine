#include "Precompiled.h"
#include "UIService.h"
#include <iostream>

namespace KREngine
{
    void UIService::Initialize(sf::RenderWindow& window)
    {
        ImGui::SFML::Init(window);
        ApplyCustomStyle();
    }

    void UIService::Terminate()
    {
        ImGui::SFML::Shutdown();
        mGameObjects.clear();
        mShapes.clear();
        mLogs.clear();
        LOG("UIService -- Terminated");
    }

    void UIService::Update(sf::RenderWindow& window, sf::Time deltaTime)
    {
        ImGui::SFML::Update(window, deltaTime);
    }

    void UIService::Render(sf::RenderWindow& window)
    {

        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x, 50));
        ImGui::Begin("Toolbar", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
        RenderToolbar();
        ImGui::End();


        ImGui::SetNextWindowPos(ImVec2(0, 50));
        ImGui::SetNextWindowSize(ImVec2(300, ImGui::GetIO().DisplaySize.y - 50));
        ImGui::Begin("Hierarchy", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
        RenderHierarchy();
        ImGui::End();

        ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x - 300, 50));
        ImGui::SetNextWindowSize(ImVec2(300, ImGui::GetIO().DisplaySize.y - 50));
        ImGui::Begin("Inspector", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
        RenderInspector();
        ImGui::End();

        RenderConsole();

        ImGui::SFML::Render(window);
    }

    void UIService::RenderToolbar()
    {
        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Save"))
                {
                    LOG("[DEBUG] Save clicked.");
                }
                if (ImGui::MenuItem("Exit"))
                {
                    LOG("[DEBUG] Exit clicked.");
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Edit"))
            {
                if (ImGui::MenuItem("Undo"))
                {
                    LOG("[DEBUG] Undo clicked.");
                }
                if (ImGui::MenuItem("Redo"))
                {
                    LOG("[DEBUG] Redo clicked.");
                }
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }
    }

    void UIService::RenderHierarchy()
    {
        ImGui::Begin("Hierarchy");
        for (const auto& [name, shape] : mShapes)
        {
            if (ImGui::Selectable(name.c_str(), name == mSelectedShape))
            {
                mSelectedShape = name;
                LOG("[DEBUG] Selected shape: " + name);
            }
        }
        ImGui::End();
    }

    void UIService::RenderInspector()
    {
        ImGui::Begin("Inspector");
        if (!mSelectedShape.empty())
        {
            auto& shape = mShapes[mSelectedShape];
            sf::Vector2f position = shape->getPosition();
            float size = shape->getScale().x;
            float rotation = shape->getRotation();
            sf::Color color = shape->getFillColor();

            float colorAsFloat[3] = { color.r / 255.0f, color.g / 255.0f, color.b / 255.0f };

            ImGui::Text("Selected: %s", mSelectedShape.c_str());

            if (ImGui::DragFloat2("Position", &position.x, 1.0f))
            {
                shape->setPosition(position);
            }
            if (ImGui::DragFloat("Size", &size, 0.1f, 0.1f, 10.0f))
            {
                shape->setScale(size, size);
            }
            if (ImGui::DragFloat("Rotation", &rotation, 1.0f, 0.0f, 360.0f))
            {
                shape->setRotation(rotation);
            }
            if (ImGui::ColorEdit3("Color", colorAsFloat))
            {
                color.r = static_cast<sf::Uint8>(colorAsFloat[0] * 255);
                color.g = static_cast<sf::Uint8>(colorAsFloat[1] * 255);
                color.b = static_cast<sf::Uint8>(colorAsFloat[2] * 255);
                shape->setFillColor(color);
            }

            if (ImGui::Button("Delete Shape"))
            {
                DeleteSelectedShape();
            }
        }
        ImGui::End();
    }


    void UIService::DeleteSelectedShape()
    {
        if (!mSelectedShape.empty())
        {
            mShapes.erase(mSelectedShape);
            mSelectedShape.clear();
        }
    }

    void UIService::RenderConsole()
    {
        ImGui::Begin("Console");
        for (const std::string& log : mLogs)
        {
            ImGui::TextWrapped(log.c_str());
        }
        if (ImGui::Button("Clear Console"))
        {
            mLogs.clear();
        }
        ImGui::End();
    }

    void UIService::AddGameObject(std::unique_ptr<GameObject> gameObject)
    {
        mGameObjects.push_back(std::move(gameObject));
    }

    const std::vector<std::unique_ptr<GameObject>>& UIService::GetGameObjects() const
    {
        return mGameObjects;
    }

    void UIService::CreateShape(const std::string& name, std::unique_ptr<sf::Shape> shape)
    {
        if (!shape)
        {
            LOG("[ERROR] Null shape passed to CreateShape.");
            return;
        }

        mShapes[name] = std::move(shape);
        LOG("[DEBUG] Created shape: " + name);
    }

    void UIService::SelectShape(const std::string& name)
    {
        if (mShapes.find(name) != mShapes.end())
        {
            mSelectedShape = name;
            LOG("[DEBUG] Selected shape: " + name);
        }
    }

    const std::unordered_map<std::string, std::unique_ptr<sf::Shape>>& UIService::GetShapes() const
    {
        return mShapes;
    }

    void UIService::ApplyCustomStyle()
    {
        ImGuiStyle& style = ImGui::GetStyle();
        style.WindowRounding = 5.0f;
        style.FrameRounding = 5.0f;
        style.GrabRounding = 5.0f;

        ImVec4* colors = style.Colors;
        colors[ImGuiCol_WindowBg] = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);
        colors[ImGuiCol_TitleBg] = ImVec4(0.2f, 0.2f, 0.5f, 1.0f);
        colors[ImGuiCol_TitleBgActive] = ImVec4(0.3f, 0.3f, 0.7f, 1.0f);
        colors[ImGuiCol_Button] = ImVec4(0.4f, 0.4f, 0.8f, 1.0f);
        colors[ImGuiCol_ButtonHovered] = ImVec4(0.5f, 0.5f, 0.9f, 1.0f);
        colors[ImGuiCol_ButtonActive] = ImVec4(0.6f, 0.6f, 1.0f, 1.0f);
    }
}
