#include "KingEngine.h"

// opengl stuff
//Define an error callback
static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
	_fgetchar();
}

//Define the key input callback
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

void resize_callback(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, w, h);
}

Mouse* KEngine::getMouse(void)
{
	return this->mouse;
}

bool KEngine::getRun(void)
{
	return this->run;
}

bool KEngine::isKeyPressed(unsigned short key)
{
    return ((GetAsyncKeyState(key) & 0x8001) != 0);
}

bool KEngine::getKeyboardUpdate(void)
{
	return true;
}

bool KEngine::getMouseUpdate(void)
{
	this->mouse->Update();

	return true;
}

// KEngine class
KEngine::KEngine()
{
	this->mouse = new Mouse();
}

KEngine::~KEngine()
{
}

// Setting up of the engine using data from the text files
void KEngine::Config(void)
{
	if (DEBUG)
	{
		engineBranch.printBranch();
	}

	for (vector<Branch>::iterator branch = engineBranch.childBranches.begin(); branch != engineBranch.childBranches.end(); ++branch)
	{
		if (branch->branchName == "SetUp")
		{
			for (vector<Attribute>::iterator attri = branch->attributes.begin(); attri != branch->attributes.end(); ++attri)
			{
				Attribute tempAttri = *attri;
				string attriName = tempAttri.name;
				string attriValue = tempAttri.value;
				if (attriName == "WindowTitle")
				{
					this->windowTitle = attriValue;
				}

				else if (attriName == "WindowWidth")
				{
					this->m_window_width = stoi(attriValue);
				}

				else if (attriName == "WindowHeight")
				{
					this->m_window_height = stoi(attriValue);
				}

				else if (attriName == "FullScreen")
				{
					if (attriValue == "true" || attriValue == "1")
					{
						this->fullScreen = true;
					}

					else
					{
						this->fullScreen = false;
					}
				}

				else if (attriName == "FrameRate")
				{
					this->FPS = stoi(attriValue);
					this->frameTime = 1000 / FPS;
				}

				else if (attriName == "Run")
				{
					if (attriValue == "true" || attriValue == "1")
					{
						this->run = true;
					}

					else
					{
						this->run = false;
					}
				}
			}
		}

		else if (branch->branchName == "MouseConfig")
		{
			this->mouse->Init(branch->attributes[FIRST].value);
			this->mouse->m_window = m_window;
		}

		else if (branch->branchName == "SceneConfig")
		{
			this->sceneConfig = branch->attributes[FIRST].value;
		}
	}
}

// Init the engine settings
void KEngine::Init(string config)
{
	engineBranch = TextTree::FileToRead(config);

	Config();
	//Set the error callback
	glfwSetErrorCallback(error_callback);

	//Initialize GLFW
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}

	//Set the GLFW window creation hints - these are optional
	glfwWindowHint(GLFW_SAMPLES, 4); //Request 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //Request a specific OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //Request a specific OpenGL version
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL 

	//Create a window and create its OpenGL context
	const char* title = windowTitle.c_str();
	
	if (fullScreen)
	{
		m_window = glfwCreateWindow(m_window_width, m_window_height, title, glfwGetPrimaryMonitor(), NULL);
	}
	else
	{
		m_window = glfwCreateWindow(m_window_width, m_window_height, title, NULL, NULL);
	}
	this->mouse->m_window = m_window;

	//If the window couldn't be created
	if (!m_window)
	{
		fprintf( stderr, "Failed to open GLFW window.\n" );
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	//This function makes the context of the specified window current on the calling thread. 
	glfwMakeContextCurrent(m_window);

	//Sets the key callback
	//glfwSetKeyCallback(m_window, key_callback);
	glfwSetWindowSizeCallback(m_window, resize_callback);

	glewExperimental = true; // Needed for core profile
	//Initialize GLEW
	GLenum err = glewInit();

	//If GLEW hasn't initialized
	if (err != GLEW_OK) 
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		//return -1;
	}

	// Hide the cursor
	//glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	// Set these 2 variables to zero
	m_dElapsedTime = 0.0;
	m_dAccumulatedTime_ThreadOne = 0.0;
	m_dAccumulatedTime_ThreadTwo = 0.0;
}

// main game loop
void KEngine::Run(void)
{
	scene = new SceneGame();
	//SceneGame* gameScene = dynamic_cast<SceneGame*> (scene);

	scene->Init(sceneConfig);

	m_timer.startTimer();	// Start timer to calculate how long it takes to render this frame

	while (!glfwWindowShouldClose(m_window) && !isKeyPressed(VK_ESCAPE) && run)
	{
		// Get the elapsed time
		m_dElapsedTime = m_timer.getElapsedTime();
		m_dAccumulatedTime_ThreadOne += m_dElapsedTime;
		m_dAccumulatedTime_ThreadTwo += m_dElapsedTime;
		if (m_dAccumulatedTime_ThreadOne > 0.03)
		{
			getMouseUpdate();
			getKeyboardUpdate();
			scene->Update(m_dElapsedTime);
			m_dAccumulatedTime_ThreadOne = 0.0;
		}
		if (m_dAccumulatedTime_ThreadTwo > 1.0)
		{
			//UpdateAI();
			m_dAccumulatedTime_ThreadTwo = 0.0;
		}
		// Render the scene
		scene->Render();
		//Swap buffers
		glfwSwapBuffers(m_window);
		//Get and organize events, like keyboard and mouse input, window resizing, etc...
		glfwPollEvents();
        m_timer.waitUntil(frameTime);       // Frame rate limiter. Limits each frame to a specified time in ms.   
	}

	scene->Exit();
	delete scene;
}

// clean up
void KEngine::Exit(void)
{
	if (this->mouse != NULL)
	{
		delete this->mouse;
		this->mouse = NULL;
	}
	//Close OpenGL window and terminate GLFW
	glfwDestroyWindow(m_window);
	//Finalize and clean up GLFW
	glfwTerminate();
}