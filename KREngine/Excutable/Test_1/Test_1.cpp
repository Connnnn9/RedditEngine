#include "imgui.h"
#include "UIService.h"
#include <SFML/Graphics.hpp>

int main()
{
    sf::RenderWindow window(sf::VideoMode(1200, 800), "Test Application");
    auto& uiService = KREngine::UIService::GetInstance();

    uiService.Initialize();

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        uiService.Update(window);

        window.clear();
        uiService.Render();
        window.display();
    }

    uiService.Terminate();
    return 0;
}


