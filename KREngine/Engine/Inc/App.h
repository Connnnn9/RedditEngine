#pragma once

#include "Common.h"
#include "AppState.h"

namespace KREngine
{
    struct AppConfig
    {
        std::wstring appName = L"KREngine App";
        uint32_t winWidth = 800;
        uint32_t winHeight = 600;
    };

    class App
    {
    public:
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

        void Run(const AppConfig& config);
        void Quit();

        sf::RenderWindow* GetWindow()
        {
            return window;
        }

    private:
        std::map<std::string, std::unique_ptr<AppState>> mAppStates;
        AppState* mCurrentState = nullptr;
        AppState* mNextState = nullptr;
        bool mRunning = false;
        sf::RenderWindow* window = nullptr;

        void Initialize(const AppConfig& config);
        void Terminate();
    };
}

