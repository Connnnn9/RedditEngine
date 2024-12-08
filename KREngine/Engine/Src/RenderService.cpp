#include "Precompiled.h"
#include "RenderService.h"
#include "Common.h"

namespace KREngine 
{

    RenderService& RenderService::GetInstance() 
    {
        static RenderService instance;
        return instance;
    }

    void RenderService::Initialize(sf::RenderWindow* renderWindow) 
    {
        window = renderWindow;
        LOG("RenderService -- Initialized");
    }

    void RenderService::Render() 
    {
        if (window) {
            window->clear(sf::Color::Black);

            for (auto& [name, drawable] : drawables) {
                window->draw(*drawable);
            }

            window->display();
        }
    }

    void RenderService::Terminate() 
    {
        LOG("RenderService -- Terminated");
        window = nullptr;
        drawables.clear();
    }

    void RenderService::AddDrawable(const std::string& name, sf::Drawable* drawable) 
    {
        drawables[name] = drawable;
    }

    void RenderService::RemoveDrawable(const std::string& name) 
    {
        drawables.erase(name);
    }
}
