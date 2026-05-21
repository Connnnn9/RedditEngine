#include "Precompiled.h"
#include "UIService.h"
#include <imgui.h>
#include <imgui-SFML.h>
#include <algorithm>
#include <ctime>

namespace KREngine
{
	void UIService::Initialize(sf::RenderWindow& window)
	{
		CameraService::GetInstance().Initialize();
		ImGui::SFML::Init(window);
		ApplyCustomStyle();
		AddLog("[INFO] UIService Initialized.");
	}

	void UIService::Terminate()
	{
		AddLog("[INFO] UIService Terminated.");
		mShapes.clear();
		mPrimitives3D.clear();
		mLogs.clear();
		ImGui::SFML::Shutdown();
	}

	void UIService::Update(sf::RenderWindow& window, sf::Time deltaTime)
	{
		ImGui::SFML::Update(window, deltaTime);
		UpdateCameraControls(window, deltaTime);

		if (Is3DScene() && mAutoRotate3D)
		{
			const float rotationDelta = mAutoRotateSpeed * deltaTime.asSeconds();
			for (auto& [name, primitive] : mPrimitives3D)
			{
				primitive.rotation.y += rotationDelta;
			}
		}
	}

	void UIService::Render(sf::RenderWindow& window)
	{
		RenderToolbar(window);

		const ImVec2 displaySize = ImGui::GetIO().DisplaySize;
		const float menuHeight = 20.0f;
		const float leftWidth = 280.0f;
		const float rightWidth = 320.0f;
		const float consoleHeight = mShowConsole ? std::min(160.0f, std::max(0.0f, displaySize.y - menuHeight - 140.0f)) : 0.0f;
		const float workHeight = std::max(120.0f, displaySize.y - menuHeight - consoleHeight);
		const float viewportWidth = std::max(220.0f, displaySize.x - leftWidth - rightWidth);

		ImGui::SetNextWindowPos(ImVec2(0, menuHeight));
		ImGui::SetNextWindowSize(ImVec2(leftWidth, workHeight));
		ImGui::Begin("Hierarchy", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
		RenderHierarchy();
		ImGui::End();

		ImGui::SetNextWindowPos(ImVec2(leftWidth, menuHeight));
		ImGui::SetNextWindowSize(ImVec2(viewportWidth, workHeight));
		ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground);
		RenderViewport(window);
		ImGui::End();

		ImGui::SetNextWindowPos(ImVec2(leftWidth + viewportWidth, menuHeight));
		ImGui::SetNextWindowSize(ImVec2(rightWidth, workHeight));
		ImGui::Begin("Inspector", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
		RenderInspector();
		ImGui::End();

		if (mShowConsole)
		{
			ImGui::SetNextWindowPos(ImVec2(0, displaySize.y - consoleHeight));
			ImGui::SetNextWindowSize(ImVec2(displaySize.x, consoleHeight));
			ImGui::Begin("Console", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
			RenderConsole();
			ImGui::End();
		}

		ImGui::SFML::Render(window);
	}

	SceneMode UIService::GetSceneMode() const
	{
		return mSceneMode;
	}

	bool UIService::ShouldRender2DShapes() const
	{
		return Is2DScene();
	}

	void UIService::ApplyCustomStyle()
	{
		ImGuiStyle& style = ImGui::GetStyle();
		style.WindowRounding = 4.0f;
		style.FrameRounding = 4.0f;
		style.GrabRounding = 4.0f;
		style.TabRounding = 4.0f;
		style.WindowPadding = ImVec2(12.0f, 10.0f);
		style.FramePadding = ImVec2(8.0f, 5.0f);
		style.ItemSpacing = ImVec2(8.0f, 8.0f);

		if (!mThemeDark)
		{
			ImGui::StyleColorsLight();
			return;
		}

		ImVec4* colors = style.Colors;
		colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.11f, 0.12f, 1.0f);
		colors[ImGuiCol_TitleBg] = ImVec4(0.13f, 0.15f, 0.18f, 1.0f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.18f, 0.22f, 0.26f, 1.0f);
		colors[ImGuiCol_MenuBarBg] = ImVec4(0.12f, 0.13f, 0.15f, 1.0f);
		colors[ImGuiCol_Button] = ImVec4(0.18f, 0.28f, 0.34f, 1.0f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.25f, 0.39f, 0.47f, 1.0f);
		colors[ImGuiCol_ButtonActive] = ImVec4(0.33f, 0.50f, 0.58f, 1.0f);
		colors[ImGuiCol_FrameBg] = ImVec4(0.15f, 0.17f, 0.19f, 1.0f);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.20f, 0.24f, 0.27f, 1.0f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.24f, 0.31f, 0.35f, 1.0f);
		colors[ImGuiCol_CheckMark] = ImVec4(0.38f, 0.72f, 0.70f, 1.0f);
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
