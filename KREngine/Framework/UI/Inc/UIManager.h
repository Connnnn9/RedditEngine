#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <unordered_map>

namespace KREngine
{
	namespace UI
	{
		class UIManager
		{
		public:
			void Initialize(sf::RenderWindow& window); // Initialize ImGui-SFML
			void Terminate();  // Terminate ImGui-SFML
			void Update(sf::RenderWindow& window, sf::Time deltaTime); // Update ImGui-SFML
			void Render(sf::RenderWindow& window); // Render ImGui-SFML

		private:
			std::unordered_map<std::string, sf::Shape*> mShapes; // Shapes in the scene
			std::string mSelectedShape; // Currently selected shape

			void RenderMainMenu();  // Main menu bar
			void RenderHierarchy(); // Sidebar with object list
			void RenderInspector(); // Inspector for selected object
		};
	}
}
