#pragma once

#include "Common.h"
#include <map>
#include <memory>
#include <string>

namespace KREngine 
{
    class AppState;

    struct AppConfig 
    {
        std::wstring appName = L"KREngine App";
        uint32_t winWidth = 1280;
        uint32_t winHeight = 720;
        uint32_t debugDrawLimit = 10000; // Maximum debug objects to draw
    };

    class App final {
    public:
        // Add a state to the app
        template <class StateType>
        void AddState(const std::string& stateName) 
        {
            static_assert(std::is_base_of_v<AppState, StateType>, "StateType must inherit from AppState");

            auto [iter, success] = mAppStates.try_emplace(stateName, nullptr);
            if (success) 
            {
                auto& ptr = iter->second;
                ptr = std::make_unique<StateType>();
                if (!mCurrentState) 
                {
                    LOG("App -- Setting initial state: %s", stateName.c_str());
                    mCurrentState = ptr.get();
                }
            }
        }

        // Change the active state
        void ChangeState(const std::string& stateName);

        // Run the application
        void Run(const AppConfig& config);

        // Quit the application
        void Quit();

    private:
        using AppStateMap = std::map<std::string, std::unique_ptr<AppState>>;

        AppStateMap mAppStates;  // Stores all app states
        AppState* mCurrentState = nullptr;
        AppState* mNextState = nullptr;
        bool mRunning = false;
    };
}
