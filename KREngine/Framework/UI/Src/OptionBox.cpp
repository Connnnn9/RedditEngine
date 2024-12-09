#include "Precompiled.h"
#include "OptionBox.h"
#include <imgui.h>

namespace KREngine
{
    namespace UI
    {
        OptionBox::OptionBox(const std::string& name)
            : mName(name)
        {
        }

        void OptionBox::Render()
        {
            ImGui::Begin(mName.c_str());
            ImGui::Text("Option Box Content");
            ImGui::End();
        }
    }
}
