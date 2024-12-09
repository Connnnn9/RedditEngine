#include "App.h"
#include "GameplayState.h"

using namespace KREngine;
using namespace std;
int main()
{
	cout << "you should see this right?";
	
	App engine;
	engine.AddState<GameplayState>("Gameplay");

	AppConfig config;
	config.appName = L"Test_1: Engine Showcase";
	config.winWidth = 1200;
	config.winHeight = 800;

	engine.Run(config);

	return 0;
}
