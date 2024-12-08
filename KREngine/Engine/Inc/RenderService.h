#pragma once

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>

namespace KREngine {
    class RenderService {
    public:
        static RenderService& GetInstance();

        void Initialize(sf::RenderWindow* window);
        void Render();
        void Terminate();

        void AddDrawable(const std::string& name, sf::Drawable* drawable);
        void RemoveDrawable(const std::string& name);

    private:
        RenderService() = default;
        ~RenderService() = default;

        sf::RenderWindow* window = nullptr;
        std::unordered_map<std::string, sf::Drawable*> drawables;
    };
}
