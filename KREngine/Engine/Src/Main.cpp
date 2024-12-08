#include "Precompiled.h"
#include "App.h"
#include "GameplayState.h"

int main() 
{
    KREngine::App engine;

    // Add a gameplay state
    engine.AddState<KREngine::GameplayState>("Gameplay");

    // Run the application
    KREngine::AppConfig config;
    config.appName = L"KREngine Test Application";
    engine.Run(config);

    return 0;
}
