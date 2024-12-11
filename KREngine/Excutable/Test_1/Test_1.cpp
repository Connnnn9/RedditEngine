#include <SFML/Graphics.hpp>
#include "UIService.h"
#include "imgui-SFML.h"
int main()
{
    sf::RenderWindow window(sf::VideoMode(1600, 800), "KREngine: Shape Manager");
    KREngine::UIService uiService;

    uiService.Initialize(window);

    sf::Clock clock;

    auto circle = std::make_unique<sf::CircleShape>(50.f);
    circle->setFillColor(sf::Color::Green);
    circle->setPosition(100.f, 100.f);
    uiService.CreateShape("Circle", std::move(circle));

    auto rectangle = std::make_unique<sf::RectangleShape>(sf::Vector2f(120.f, 60.f));
    rectangle->setFillColor(sf::Color::Blue);
    rectangle->setPosition(300.f, 200.f);
    uiService.CreateShape("Rectangle", std::move(rectangle));

    auto triangle = std::make_unique<sf::CircleShape>(60.f, 3);
    triangle->setFillColor(sf::Color::Red);
    triangle->setPosition(500.f, 300.f);
    uiService.CreateShape("Triangle", std::move(triangle));

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);

            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        sf::Time deltaTime = clock.restart();
        uiService.Update(window, deltaTime);

        window.clear(sf::Color::Black);

        for (const auto& [name, shape] : uiService.GetShapes())
        {
            if (shape)
            {
                window.draw(*shape);
            }
        }

        uiService.Render(window);

        // Display the window
        window.display();
    }

    // Terminate the UIService
    uiService.Terminate();

    return 0;
}
