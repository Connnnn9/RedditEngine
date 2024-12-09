#pragma once

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <vector>
#include <string>

namespace KREngine
{
	namespace UI
	{
		class UIManager
		{
		public:
			void Initialize();
			void Terminate();
			void Update(sf::RenderWindow& window);
			void Render();

			void AddOption(const std::string& optionName);
			const std::vector<std::string>& GetOptions() const;
			void UpdateDisplaySize(float width, float height);

		private:
			sf::Texture fontTexture;
			std::vector<std::string> mOptions;
		};
	}
}
