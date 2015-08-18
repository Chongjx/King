#include "KingEngine.h"

int main(void)
{
	// get instance of the game engine and run
	KEngine &engine = KEngine::getInstance();
	engine.Init("Reader\\AppConfig.txt");
	engine.Run();
	engine.Exit();
}