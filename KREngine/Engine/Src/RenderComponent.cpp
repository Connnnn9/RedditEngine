#include "Precompiled.h"
#include "RenderComponent.h"

namespace KREngine
{
    void RenderComponent::DebugUI()
    {
        if (ImGui::CollapsingHeader("Render"))
        {
            if (mDrawable)
                ImGui::Text("Drawable is set.");
            else
                ImGui::Text("No drawable set.");
        }
    }
}
