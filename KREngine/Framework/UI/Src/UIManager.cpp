#include "UIManager.h"
#include <imgui.h>

namespace KREngine
{
	namespace UI
	{
		void UIManager::Initialize()
		{
			ImGui::CreateContext();
		}

		void UIManager::Terminate()
		{
			ImGui::DestroyContext();
		}

		void UIManager::Update()
		{
			ImGui::NewFrame();
		}

		void UIManager::Render()
		{
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
