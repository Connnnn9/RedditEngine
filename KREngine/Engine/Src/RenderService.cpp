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
    }

    void RenderService::Render()
    {
        if (window)
        {
            window->clear(sf::Color::Black);

            for (auto& [name, drawable] : drawables)
            {
                window->draw(*drawable);
            }

            window->display();
        }
    }

    void RenderService::Terminate()
    {
        window = nullptr;
        drawables.clear();
    }

    void RenderService::AddDrawable(const std::string& name, sf::Drawable* drawable)
    {
        if (drawables.find(name) != drawables.end())
        {
            LOG("Drawable with name %s already exists, overwriting.", name.c_str());
        }
        drawables[name] = drawable;
        LOG("Drawable added: %s", name.c_str());
    }

    void RenderService::RemoveDrawable(const std::string& name)
    {
        auto it = drawables.find(name);
        if (it != drawables.end())
        {
            drawables.erase(it);
            LOG("Drawable removed: %s", name.c_str());
        }
        else
        {
            LOG("Drawable not found: %s", name.c_str());
        }
    }
}


