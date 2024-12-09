#include "Precompiled.h"
#include "UIManager.h"
#include <imgui.h>

#define LOG(message) std::cout << "[LOG] " << message << std::endl;

namespace KREngine
{
	namespace UI
	{
		void UIManager::Initialize()
		{
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO();

			io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

			unsigned char* pixels;
			int width, height;
			io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

			fontTexture.create(width, height);
			fontTexture.update(pixels);

			unsigned int textureID = fontTexture.getNativeHandle();
			io.Fonts->TexID = textureID;

		}

		void UIManager::Terminate()
		{
			fontTexture.~Texture();
			ImGui::DestroyContext();

		}

		void UIManager::UpdateDisplaySize(float width, float height)
		{
			ImGuiIO& io = ImGui::GetIO();
			io.DisplaySize = ImVec2(width, height);
			LOG("DisplaySize set to: " + std::to_string(width) + "x" + std::to_string(height));
		}

		void UIManager::Update(sf::RenderWindow& window)
		{
			UpdateDisplaySize(static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y));
			ImGui::NewFrame();
		}

		void UIManager::Render()
		{
			ImGui::Begin("KREngine");
			ImGui::Text("WElCOME");
			ImGui::End();

			ImGui::Render();
		}

		void UIManager::AddOption(const std::string& optionName)
		{
			mOptions.push_back(optionName);
		}

		const std::vector<std::string>& UIManager::GetOptions() const
		{
			return mOptions;
		}
	}
}
