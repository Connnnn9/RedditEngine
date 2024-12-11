#pragma once

#include <SFML/Graphics.hpp>
#include "GameObject.h"

namespace KREngine
{
	class UIService
	{
	public:
		void Initialize(sf::RenderWindow& window);
		void Terminate();
		void Update(sf::RenderWindow& window, sf::Time deltaTime);
		void Render(sf::RenderWindow& window);

		void AddGameObject(std::unique_ptr<GameObject> gameObject);
		const std::vector<std::unique_ptr<GameObject>>& GetGameObjects() const;

		void CreateShape(const std::string& name, std::unique_ptr<sf::Shape> shape);
		void SelectShape(const std::string& name);
		void DeleteSelectedShape();
		const std::unordered_map<std::string, std::unique_ptr<sf::Shape>>& GetShapes() const;

	private:
		void RenderHierarchy();
		void RenderInspector();
		void RenderToolbar();
		void RenderConsole();

		void ApplyCustomStyle();

		std::vector<std::unique_ptr<GameObject>> mGameObjects;
		std::unordered_map<std::string, std::unique_ptr<sf::Shape>> mShapes;
		std::string mSelectedShape;
		std::vector<std::string> mLogs;
	};
}
