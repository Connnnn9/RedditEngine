#include "UIService.h"
#include <SFML/Graphics.hpp>
#include <imgui.h>

int main()
{
    sf::RenderWindow window(sf::VideoMode(1200, 800), "Minimal ImGui Test");
    ImGui::CreateContext();

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        ImGui::NewFrame();
        ImGui::Begin("Test Window");
        ImGui::Text("Hello, ImGui!");
        ImGui::End();
        ImGui::Render();

        window.clear();
        window.display();
    }

    ImGui::DestroyContext();
    return 0;
}
