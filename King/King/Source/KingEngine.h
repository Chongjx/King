#ifndef KING_ENGINE
#define KING_ENGINE

//Include GLEW
#include <GL/glew.h>

//Include the standard C++ headers
#include <stdio.h>
#include <stdlib.h>
#include <string>

#include "timer.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "SceneGame.h"

#define FIRST 0

using std::string;

class KEngine
{
public:
	static KEngine& getInstance()
	{
		static KEngine engine;
		return engine;
	}

	void Init(string config);
	void Run(void);
	void Exit(void);

	static Mouse* getMouse(void);
	static Keyboard* getKeyboard(void);
	
	bool getKeyboardUpdate(void);
	bool getMouseUpdate(void);

	static void setRun(bool brun);
	static bool getRun(void);

	static void setWindowWidth(int width);
	static void setWindowHeight(int height);

	static int getWindowWidth(void);
	static int getWindowHeight(void);
private:
	KEngine();
	~KEngine();

	void Config(void);

	GLFWwindow* m_window;
	StopWatch m_timer;
	double m_dElapsedTime;
	double m_dAccumulatedTime_ThreadOne;
	double m_dAccumulatedTime_ThreadTwo;

	Branch engineBranch;

	static Mouse* mouse;
	static Keyboard* keyboard;

	unsigned char FPS;
	unsigned int frameTime;
	static int windowWidth;
	static int windowHeight;
	string windowTitle;
	static bool run;
	bool fullScreen;

	// The handler for the scene
	SceneGame* scene;
	string sceneConfig;
};

#endif