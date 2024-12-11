#include "Precompiled.h"
#include "RenderService.h"

namespace KREngine
{
    RenderService& RenderService::GetInstance()
    {
        static RenderService instance;
        return instance;
    }

    void RenderService::Initialize(sf::RenderWindow* renderWindow)
    {
        ASSERT(renderWindow, "RenderService: Render window is null during initialization!");
        window = renderWindow;
        LOG("RenderService -- Initialized");
    }

    void RenderService::Render()
    {
        ASSERT(window, "RenderService: Render window is null during rendering!");

        window->clear(sf::Color::Black);

        for (auto& [name, drawable] : drawables)
        {
            ASSERT(drawable, "RenderService: Drawable is null!");
            window->draw(*drawable);
        }

        window->display();
    }

    void RenderService::Terminate()
    {
        window = nullptr;
        drawables.clear();
        LOG("RenderService -- Terminated");
    }

    void RenderService::AddDrawable(const std::string& name, sf::Drawable* drawable)
    {
        ASSERT(drawable, "RenderService: Cannot add a null drawable!");
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
