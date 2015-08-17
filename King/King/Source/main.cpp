#include "KingEngine.h"

int main(void)
{
	KEngine &engine = KEngine::getInstance();
	engine.Init("Reader\\AppConfig.txt");
	engine.Run();
	engine.Exit();
}