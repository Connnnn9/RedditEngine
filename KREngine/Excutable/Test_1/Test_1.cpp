#include "App.h"
#include "GameplayState.h"

int main()
{
    KREngine::SetLogFile("test_1.log");

    KREngine::App engine;

    engine.AddState<KREngine::GameplayState>("Gameplay");

    KREngine::AppConfig config;
    config.appName = L"Test_1: Engine Showcase";
    config.winWidth = 1200;
    config.winHeight = 800;

    engine.Run(config);

    return 0;
}
