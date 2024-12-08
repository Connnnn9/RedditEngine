#include "App.h"
#include "GameplayState.h"
#include "RenderService.h"
#include "Common.h"
int main() {
    KREngine::SetLogFile("test_1.log"); // Redirect logs to a file for debugging

    // Create the engine app
    KREngine::App engine;

    // Add the gameplay state to the app
    engine.AddState<KREngine::GameplayState>("Gameplay");

    // Configure and run the app
    KREngine::AppConfig config;
    config.appName = L"Test_1: Engine Showcase";
    config.winWidth = 800;
    config.winHeight = 600;

    engine.Run(config);

    return 0;
}
