#include <SFML/Graphics.hpp>
#include "UIManager.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "ImGui-SFML Example");
    KREngine::UI::UIManager uiManager;

    uiManager.Initialize(window);

    sf::Clock clock;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        sf::Time deltaTime = clock.restart();

        uiManager.Update(window, deltaTime);

        window.clear();
        uiManager.Render(window);
        window.display();
    }

    uiManager.Terminate();

    return 0;
}


