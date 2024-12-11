#include "Precompiled.h"
#include "TransformComponent.h"

namespace KREngine
{
    void TransformComponent::DebugUI()
    {
        if (ImGui::CollapsingHeader("Transform"))
        {
            ImGui::DragFloat2("Position", &mPosition.x, 1.0f);
        }
    }
}
