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

		void CreateShape(const std::string& name, std::unique_ptr<sf::Shape> shape);
		void SelectShape(const std::string& name);
		void DeleteSelectedShape();

		const std::unordered_map<std::string, std::unique_ptr<sf::Shape>>& GetShapes() const;

	private:
		void RenderToolbar();
		void RenderHierarchy();
		void RenderInspector();
		void RenderConsole();
		void RenderViewport(sf::RenderWindow& window);
		void ApplyCustomStyle();
		void AddLog(const std::string& message);

		std::unordered_map<std::string, std::unique_ptr<sf::Shape>> mShapes;
		std::string mSelectedShape;
		std::vector<std::string> mLogs;
		std::string mDraggedShapeType;
		bool mThemeDark = true;
	};
}
