#include "Precompiled.h"
#include "UIService.h"
#include <imgui.h>
#include <imgui-SFML.h>

namespace KREngine
{
	void UIService::Initialize(sf::RenderWindow& window)
	{
		ImGui::SFML::Init(window);
		ApplyCustomStyle();
		AddLog("[INFO] UIService Initialized.");
	}

	void UIService::Terminate()
	{
		ImGui::SFML::Shutdown();
		mShapes.clear();
		mLogs.clear();
		AddLog("[INFO] UIService Terminated.");
	}

	void UIService::Update(sf::RenderWindow& window, sf::Time deltaTime)
	{
		ImGui::SFML::Update(window, deltaTime);
	}

	void UIService::Render(sf::RenderWindow& window)
	{
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Save"))
					LOG("[DEBUG] Save clicked.");
				if (ImGui::MenuItem("Exit"))
					LOG("[DEBUG] Exit clicked.");
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Edit"))
			{
				if (ImGui::MenuItem("Undo"))
					LOG("[DEBUG] Undo clicked.");
				if (ImGui::MenuItem("Redo"))
					LOG("[DEBUG] Redo clicked.");
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}

		// Reserve areas for different UI sections
		ImGui::SetNextWindowPos(ImVec2(0, 20));
		ImGui::SetNextWindowSize(ImVec2(300, ImGui::GetIO().DisplaySize.y - 20));
		ImGui::Begin("Hierarchy", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
		RenderHierarchy();
		ImGui::End();

		ImGui::SetNextWindowPos(ImVec2(300, 20));
		ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x - 600, ImGui::GetIO().DisplaySize.y - 20));
		ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
		RenderViewport(window);
		ImGui::End();

		ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x - 300, 20));
		ImGui::SetNextWindowSize(ImVec2(300, ImGui::GetIO().DisplaySize.y - 20));
		ImGui::Begin("Inspector", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
		RenderInspector();
		ImGui::End();

		ImGui::SetNextWindowPos(ImVec2(0, ImGui::GetIO().DisplaySize.y - 150));
		ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x, 150));
		ImGui::Begin("Console", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
		RenderConsole();
		ImGui::End();

		ImGui::SFML::Render(window);
	}

	void UIService::RenderToolbar()
	{
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Save")) AddLog("[INFO] Save clicked.");
				if (ImGui::MenuItem("Exit")) AddLog("[INFO] Exit clicked.");
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Edit"))
			{
				if (ImGui::MenuItem("Toggle Theme"))
				{
					mThemeDark = !mThemeDark;
					ApplyCustomStyle();
					AddLog("[INFO] Theme toggled.");
				}
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}
	}

	void UIService::RenderHierarchy()
	{
		ImGui::Begin("Hierarchy");
		if (ImGui::Button("Circle")) mDraggedShapeType = "Circle";
		if (ImGui::Button("Rectangle")) mDraggedShapeType = "Rectangle";

		for (const auto& [name, shape] : mShapes)
			if (ImGui::Selectable(name.c_str(), name == mSelectedShape))
				SelectShape(name);

		ImGui::End();
	}

	void UIService::RenderInspector()
	{
		if (mSelectedShape.empty()) return;

		ImGui::Begin("Inspector");

		auto& shape = mShapes[mSelectedShape];
		sf::Vector2f position = shape->getPosition();
		float size = shape->getScale().x;
		float rotation = shape->getRotation();
		sf::Color color = shape->getFillColor();
		float colorAsFloat[3] = { color.r / 255.0f, color.g / 255.0f, color.b / 255.0f };

		ImGui::Text("Selected: %s", mSelectedShape.c_str());
		if (ImGui::DragFloat2("Position", &position.x, 1.0f)) shape->setPosition(position);
		if (ImGui::DragFloat("Size", &size, 0.1f, 0.1f, 10.0f)) shape->setScale(size, size);
		if (ImGui::DragFloat("Rotation", &rotation, 1.0f, 0.0f, 360.0f)) shape->setRotation(rotation);
		if (ImGui::ColorEdit3("Color", colorAsFloat))
		{
			color.r = static_cast<sf::Uint8>(colorAsFloat[0] * 255);
			color.g = static_cast<sf::Uint8>(colorAsFloat[1] * 255);
			color.b = static_cast<sf::Uint8>(colorAsFloat[2] * 255);
			shape->setFillColor(color);
		}

		if (ImGui::Button("Delete Shape")) DeleteSelectedShape();

		ImGui::End();
	}

	void UIService::RenderConsole()
	{
		ImGui::Begin("Console");
		for (const std::string& log : mLogs) ImGui::TextWrapped(log.c_str());
		if (ImGui::Button("Clear Console")) mLogs.clear();
		ImGui::End();
	}

	void UIService::RenderViewport(sf::RenderWindow& window)
	{
		if (!mDraggedShapeType.empty() && sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
			std::unique_ptr<sf::Shape> shape;

			if (mDraggedShapeType == "Circle")
			{
				shape = std::make_unique<sf::CircleShape>(50.f);
				shape->setFillColor(sf::Color::Green);
			}
			else if (mDraggedShapeType == "Rectangle")
			{
				shape = std::make_unique<sf::RectangleShape>(sf::Vector2f(100.f, 50.f));
				shape->setFillColor(sf::Color::Blue);
			}

			if (shape)
			{
				shape->setPosition(mousePos);
				CreateShape(mDraggedShapeType + std::to_string(mShapes.size()), std::move(shape));
				mDraggedShapeType.clear();
			}
		}
	}

	void UIService::CreateShape(const std::string& name, std::unique_ptr<sf::Shape> shape)
	{
		if (!shape) { AddLog("[ERROR] Null shape passed to CreateShape."); return; }
		mShapes[name] = std::move(shape);
		AddLog("[DEBUG] Created shape: " + name);
	}

	void UIService::SelectShape(const std::string& name)
	{
		if (mShapes.find(name) != mShapes.end())
		{
			mSelectedShape = name;
			AddLog("[DEBUG] Selected shape: " + name);
		}
	}

	void UIService::DeleteSelectedShape()
	{
		if (!mSelectedShape.empty())
		{
			mShapes.erase(mSelectedShape);
			mSelectedShape.clear();
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
		style.TabRounding = 5.0f;

		ImVec4* colors = style.Colors;
		colors[ImGuiCol_WindowBg] = ImVec4(0.12f, 0.12f, 0.12f, 1.0f);
		colors[ImGuiCol_TitleBg] = ImVec4(0.2f, 0.2f, 0.4f, 1.0f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.3f, 0.3f, 0.6f, 1.0f);
		colors[ImGuiCol_Button] = ImVec4(0.3f, 0.3f, 0.5f, 1.0f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.4f, 0.4f, 0.7f, 1.0f);
		colors[ImGuiCol_ButtonActive] = ImVec4(0.5f, 0.5f, 0.8f, 1.0f);
		colors[ImGuiCol_Tab] = ImVec4(0.2f, 0.2f, 0.4f, 1.0f);
		colors[ImGuiCol_TabHovered] = ImVec4(0.3f, 0.3f, 0.6f, 1.0f);
		colors[ImGuiCol_TabActive] = ImVec4(0.4f, 0.4f, 0.7f, 1.0f);
		colors[ImGuiCol_FrameBg] = ImVec4(0.15f, 0.15f, 0.25f, 1.0f);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.2f, 0.2f, 0.3f, 1.0f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.25f, 0.25f, 0.4f, 1.0f);
		colors[ImGuiCol_CheckMark] = ImVec4(0.5f, 0.5f, 0.9f, 1.0f);

	}

	void UIService::AddLog(const std::string& message)
	{
		time_t now = time(0);
		tm localtm;
		localtime_s(&localtm, &now);
		std::string timestamp = "[" + std::to_string(localtm.tm_hour) + ":" +
			std::to_string(localtm.tm_min) + ":" + std::to_string(localtm.tm_sec) + "] ";
		mLogs.push_back(timestamp + message);
	}
}