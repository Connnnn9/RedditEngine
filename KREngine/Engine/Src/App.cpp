#include "Precompiled.h"
#include "App.h"
#include "RenderService.h"
#include "InputService.h"

namespace KREngine
{
    void App::Initialize(const AppConfig& config)
    {
        LOG("Initializing KREngine...");

        mWindow = std::make_unique<sf::RenderWindow>(sf::VideoMode(config.winWidth, config.winHeight), config.appName);

        RenderService::GetInstance().Initialize(mWindow.get());
        InputService::GetInstance().Initialize(mWindow.get());

        mRunning = true;

        LOG("KREngine initialized successfully.");
    }

    void App::Run(const AppConfig& config)
    {
        Initialize(config);

        if (mCurrentState)
        {
            mCurrentState->Initialize();
        }

        while (mRunning)
        {
            InputService::GetInstance().Update();
            if (!mWindow || !mWindow->isOpen())
            {
                Quit();
                continue;
            }

            // Terminate the app when Escape is pressed
            if (InputService::GetInstance().IsKeyPressed(sf::Keyboard::Escape))
            {
                Quit();
            }

            if (mNextState)
            {
                if (mCurrentState)
                {
                    mCurrentState->Terminate();
                }
                mCurrentState = mNextState;
                mNextState = nullptr;
                mCurrentState->Initialize();
            }

            if (mCurrentState)
            {
                mCurrentState->Update();
                mCurrentState->Render();
            }
        }

        if (mCurrentState)
        {
            mCurrentState->Terminate();
        }

        Terminate();
    }


    void App::Terminate()
    {
        LOG("Terminating KREngine...");

        InputService::GetInstance().Terminate();
        RenderService::GetInstance().Terminate();
        mWindow.reset();

        LOG("KREngine terminated successfully.");
    }

    void App::Quit()
    {
        LOG("App -- Quitting application.");
        mRunning = false;
    }
}
