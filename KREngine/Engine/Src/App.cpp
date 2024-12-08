#include "Precompiled.h"
#include "App.h"
#include "AppState.h"

namespace KREngine {

    void App::ChangeState(const std::string& stateName) {
        auto iter = mAppStates.find(stateName);
        if (iter != mAppStates.end()) {
            LOG("App -- Changing state to: %s", stateName.c_str());
            mNextState = iter->second.get();
        }
        else {
            LOG("App -- State '%s' not found.", stateName.c_str());
        }
    }

    void App::Run(const AppConfig& config) {
        LOG("App -- Starting application: %ls", config.appName.c_str());
        mRunning = true;

        // Initialize the current state
        if (mCurrentState) {
            mCurrentState->Initialize();
        }

        // Main loop
        while (mRunning) {
            if (mNextState) {
                // Terminate the current state
                if (mCurrentState) {
                    mCurrentState->Terminate();
                }

                // Switch to the next state
                mCurrentState = mNextState;
                mNextState = nullptr;
                mCurrentState->Initialize();
            }

            // Update and render the current state
            if (mCurrentState) {
                mCurrentState->Update();
                mCurrentState->Render();
            }
        }

        // Clean up
        if (mCurrentState) {
            mCurrentState->Terminate();
        }
        LOG("App -- Application exited.");
    }

    void App::Quit() {
        LOG("App -- Quitting application.");
        mRunning = false;
    }
}
