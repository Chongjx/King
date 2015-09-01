#include "KingEngine.h"
//#include <vld.h>
//#define _CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h>

int main(void)
{
	// get instance of the game engine and run
	KEngine &engine = KEngine::getInstance();
	engine.Init("Reader\\AppConfig.txt");
	engine.Run();
	engine.Exit();

	//_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
	//_CrtDumpMemoryLeaks();
}