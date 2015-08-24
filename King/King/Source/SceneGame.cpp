#include "SceneGame.h"
#include "GL\glew.h"
#include "shader.hpp"
#include "KingEngine.h"
#include "Utility.h"
#include "LoadTGA.h"

// Set the m_parameters to match the number uniform type & lights to match the number lights
SceneGame::SceneGame(void)
{
	m_parameters.resize(U_TOTAL);
	lights.resize(NUM_LIGHTS);
	layout.resize(MAX_AREAS);
	gameInterfaces.resize(MAX_STATE);
	interactionList.resize(MAX_INTERACTIONS);

	for (unsigned i = MENU_STATE; i < MAX_STATE; ++i)
	{
		gameInterfaces[i].interfaceType = i;
	}

	currentState = MENU_STATE;
	currentLocation = CELL_AREA;

	sceneWidth = 0.f;
	sceneHeight = 0.f;
	specialFontSize = 0.f;
	defaultFontSize = 0.f;
	paused = false;

	player = new Player();
	guardList.clear();
	prisonerList.clear();
}

SceneGame::~SceneGame(void)
{
}

// Init game settings
void SceneGame::Init(string config)
{
	// Call default scene init
	Scene2D::Init();

	gameBranch = TextTree::FileToRead(config);
	Config();
}

// Game update
void SceneGame::Update(double dt)
{
	// Call default scene update
	Scene2D::Update(dt);

	UpdateOpengl();
	UpdateInput();
	UpdateMouse();
	camera.Update(dt);
	UpdateState();
	UpdateEffect();

	// Update buttons
	for (unsigned i = 0; i < gameInterfaces[currentState].buttons.size(); ++i)
	{
		gameInterfaces[currentState].buttons[i].Update(getKey("Select"), mousePos.x, mousePos.y);
	}

	switch(currentState)
	{
	case MENU_STATE:
		{
			break;
		}
	case INGAME_STATE:
		{
			if (!paused)
			{
				UpdateInGame(dt);
			}
			break;
		}
	case INSTRUCTION_STATE:
		{
			break;
		}
	case HIGHSCORE_STATE:
		{
			break;
		}
	case OPTIONS_STATE:
		{
			break;
		}
	case PAUSE_STATE:
		{
			break;
		}
	case EXIT_STATE:
		{
			KEngine::setRun(false);
			break;
		}
	}
}

// Game render
void SceneGame::Render(void)
{
	// Call default scene render
	Scene2D::Render();
	glDisable(GL_DEPTH_TEST);

	switch(currentState)
	{
	case MENU_STATE:
		{
			break;
		}
	case INGAME_STATE:
		{
			RenderLevel();
			RenderTime();
			RenderCharacters();
			break;
		}
	case INSTRUCTION_STATE:
		{
			break;
		}
	case HIGHSCORE_STATE:
		{
			break;
		}
	case OPTIONS_STATE:
		{
			break;
		}
	case PAUSE_STATE:
		{
			RenderLevel();
			RenderTime();
			RenderCharacters();
			break;
		}
	case EXIT_STATE:
		{
			break;
		}
	}

	RenderInterface();

	/*std::ostringstream ss;
	ss.precision(5);
	ss << "FPS: " << fps;
	RenderTextOnScreen(findMesh("GEO_TEXT"), ss.str(), findColor("Red"), specialFontSize, 0, sceneHeight - specialFontSize);*/
	//std::cout << fps << std::endl;

	glEnable(GL_DEPTH_TEST);
}

// Game clean up
void SceneGame::Exit(void)
{
	// Call default scene exit
	Scene2D::Exit();

	// clear up all the mesh
	for (vector<Mesh*>::iterator mesh = meshList.begin(); mesh != meshList.end(); ++mesh)
	{
		Mesh* temp = *mesh;

		// handle spriteAnimation
		SpriteAnimation* checkSprite = dynamic_cast<SpriteAnimation*>(temp);

		if (checkSprite != NULL)
		{
			checkSprite->~SpriteAnimation();
			checkSprite = NULL;
		}

		else
		{
			delete temp;
			temp = NULL;
		}
	}

	// Clear player
	if(player)
	{
		delete player;
		player = NULL;
	}

	// Clear guards in guardList
	for (vector<Guards*>::iterator guard = guardList.begin(); guard != guardList.end(); ++guard)
	{
		Guards* tempGuard = *guard;
		if(tempGuard != NULL)
		{
			delete tempGuard;
			tempGuard = NULL;
		}
	}

	//Clear prisoners in prisonerList
	for (vector<Prisoners*>::iterator prisoner = prisonerList.begin(); prisoner != prisonerList.end(); ++prisoner)
	{
		Prisoners* tempPrisoner = *prisoner;
		if(tempPrisoner != NULL)
		{
			delete tempPrisoner;
			tempPrisoner = NULL;
		}
	}

	glDeleteVertexArrays(1, &m_vertexArrayID);
}

// Setting up of the game using data from the text files
void SceneGame::Config(void)
{
	for (vector<Branch>::iterator branch = gameBranch.childBranches.begin(); branch != gameBranch.childBranches.end(); ++branch)
	{
		if (branch->branchName == "Shader")
		{
			string vertexShader, fragmentShader;
			for (vector<Attribute>::iterator attri = branch->attributes.begin(); attri != branch->attributes.end(); ++attri)
			{
				Attribute tempAttri = *attri;
				string attriName = tempAttri.name;
				string attriValue = tempAttri.value;
				if (attriName == "VertexShader")
				{
					vertexShader = attriValue;
				}

				else if (attriName == "FragmentShader")
				{
					fragmentShader = attriValue;
				}
			}

			m_programID = LoadShaders(vertexShader.c_str(), fragmentShader.c_str());

			InitShaders();
		}

		else if (branch->branchName == "Settings")
		{
			for (vector<Attribute>::iterator attri = branch->attributes.begin(); attri != branch->attributes.end(); ++attri)
			{
				Attribute tempAttri = *attri;
				string attriName = tempAttri.name;
				string attriValue = tempAttri.value;
				if (attriName == "Directory")
				{
					InitSettings(attriValue);
				}
			}
		}

		else if (branch->branchName == "Variables")
		{
			for (vector<Attribute>::iterator attri = branch->attributes.begin(); attri != branch->attributes.end(); ++attri)
			{
				Attribute tempAttri = *attri;
				string attriName = tempAttri.name;
				string attriValue = tempAttri.value;
				if (attriName == "Directory")
				{
					InitVariables(attriValue);
				}
			}
		}

		else if (branch->branchName == "Mesh")
		{
			for (vector<Attribute>::iterator attri = branch->attributes.begin(); attri != branch->attributes.end(); ++attri)
			{
				Attribute tempAttri = *attri;
				string attriName = tempAttri.name;
				string attriValue = tempAttri.value;
				if (attriName == "Directory")
				{
					InitMesh(attriValue);
				}
			}
		}

		else if (branch->branchName == "Color")
		{
			for (vector<Attribute>::iterator attri = branch->attributes.begin(); attri != branch->attributes.end(); ++attri)
			{
				Attribute tempAttri = *attri;
				string attriName = tempAttri.name;
				string attriValue = tempAttri.value;
				if (attriName == "Directory")
				{
					InitColor(attriValue);
				}
			}
		}

		else if (branch->branchName == "Interface")
		{
			for (vector<Attribute>::iterator attri = branch->attributes.begin(); attri != branch->attributes.end(); ++attri)
			{
				Attribute tempAttri = *attri;
				string attriName = tempAttri.name;
				string attriValue = tempAttri.value;
				if (attriName == "Directory")
				{
					InitInterface(attriValue);
				}
			}
		}

		else if (branch->branchName == "Layout")
		{
			for (vector<Attribute>::iterator attri = branch->attributes.begin(); attri != branch->attributes.end(); ++attri)
			{
				Attribute tempAttri = *attri;
				string attriName = tempAttri.name;
				string attriValue = tempAttri.value;
				if (attriName == "Directory")
				{
					InitLevel(attriValue);
				}
			}
		}
		else if (branch->branchName == "Sound")
		{
			for (vector<Attribute>::iterator attri = branch->attributes.begin(); attri != branch->attributes.end(); ++attri)
			{
				Attribute tempAttri = *attri;
				string attriName = tempAttri.name;
				string attriValue = tempAttri.value;
				if (attriName == "Directory")
				{
					InitSound(attriValue);
				}
			}
		}
		else if (branch->branchName == "Player")
		{
			for (vector<Attribute>::iterator attri = branch->attributes.begin(); attri != branch->attributes.end(); ++attri)
			{
				Attribute tempAttri = *attri;
				string attriName = tempAttri.name;
				string attriValue = tempAttri.value;
				if (attriName == "Directory")
				{
					InitPlayer(attriValue);
				}
			}
		}
		else if (branch->branchName == "AI")
		{
			for (vector<Attribute>::iterator attri = branch->attributes.begin(); attri != branch->attributes.end(); ++attri)
			{
				Attribute tempAttri = *attri;
				string attriName = tempAttri.name;
				string attriValue = tempAttri.value;
				if (attriName == "Directory")
				{
					InitAI(attriValue);
				}
			}
		}
		else if (branch->branchName == "Objectives")
		{
			for (vector<Attribute>::iterator attri = branch->attributes.begin(); attri != branch->attributes.end(); ++attri)
			{
				Attribute tempAttri = *attri;
				string attriName = tempAttri.name;
				string attriValue = tempAttri.value;
				if (attriName == "Directory")
				{
					InitObjective(attriValue);
				}
			}
		}
	}
}

// Init scene shaders
void SceneGame::InitShaders(void)
{
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	// Get a handle for our uniform
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	//m_parameters[U_MODEL] = glGetUniformLocation(m_programID, "M");
	//m_parameters[U_VIEW] = glGetUniformLocation(m_programID, "V");
	m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
	m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
	m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
	m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
	m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
	m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");
	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");
	m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");
	m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0].type");
	m_parameters[U_LIGHT0_POSITION] = glGetUniformLocation(m_programID, "lights[0].position_cameraspace");
	m_parameters[U_LIGHT0_COLOR] = glGetUniformLocation(m_programID, "lights[0].color");
	m_parameters[U_LIGHT0_POWER] = glGetUniformLocation(m_programID, "lights[0].power");
	m_parameters[U_LIGHT0_KC] = glGetUniformLocation(m_programID, "lights[0].kC");
	m_parameters[U_LIGHT0_KL] = glGetUniformLocation(m_programID, "lights[0].kL");
	m_parameters[U_LIGHT0_KQ] = glGetUniformLocation(m_programID, "lights[0].kQ");
	m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[0].spotDirection");
	m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[0].cosCutoff");
	m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID, "lights[0].cosInner");
	m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID, "lights[0].exponent");
	// Get a handle for our "colorTexture" uniform
	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");
	// Get a handle for our "textColor" uniform
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");

	// Use our shader
	glUseProgram(m_programID);

	lights[0].type = Light::LIGHT_DIRECTIONAL;
	lights[0].position.Set(0, 0, 10);
	lights[0].color.Set(1, 1, 1);
	lights[0].power = 1;
	lights[0].kC = 1.f;
	lights[0].kL = 0.01f;
	lights[0].kQ = 0.001f;
	lights[0].cosCutoff = cos(Math::DegreeToRadian(45));
	lights[0].cosInner = cos(Math::DegreeToRadian(30));
	lights[0].exponent = 3.f;
	lights[0].spotDirection.Set(0.f, 0.f, 1.f);

	glUniform1i(m_parameters[U_NUMLIGHTS], 1);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);

	glUniform1i(m_parameters[U_LIGHT0_TYPE], lights[0].type);
	glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &lights[0].color.r);
	glUniform1f(m_parameters[U_LIGHT0_POWER], lights[0].power);
	glUniform1f(m_parameters[U_LIGHT0_KC], lights[0].kC);
	glUniform1f(m_parameters[U_LIGHT0_KL], lights[0].kL);
	glUniform1f(m_parameters[U_LIGHT0_KQ], lights[0].kQ);
	glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF], lights[0].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT0_COSINNER], lights[0].cosInner);
	glUniform1f(m_parameters[U_LIGHT0_EXPONENT], lights[0].exponent);
}

void SceneGame::InitSettings(string config)
{
	Branch settingBranch = TextTree::FileToRead(config);

	for (vector<Branch>::iterator branch = settingBranch.childBranches.begin(); branch != settingBranch.childBranches.end(); ++branch)
	{
		if (branch->branchName == "Resolution")
		{
			for (vector<Attribute>::iterator attri = branch->attributes.begin(); attri != branch->attributes.end(); ++attri)
			{
				Attribute tempAttri = *attri;
				string attriName = tempAttri.name;
				string attriValue = tempAttri.value;
				if (attriName == "SceneWidth")
				{
					sceneWidth = stof(attriValue);
				}

				else if (attriName == "SceneHeight")
				{
					sceneHeight = stof(attriValue);
				}
			}
		}

		else if (branch->branchName == "Camera")
		{
			Vector3 tempPos, tempTarget, tempUp;

			for (vector<Attribute>::iterator attri = branch->attributes.begin(); attri != branch->attributes.end(); ++attri)
			{
				Attribute tempAttri = *attri;
				string attriName = tempAttri.name;
				string attriValue = tempAttri.value;
				if (attriName == "CameraPos")
				{
					stringToVector(attriValue, tempPos);
				}

				else if (attriName == "CameraTarget")
				{
					stringToVector(attriValue, tempTarget);
				}

				else if (attriName == "CameraUp")
				{
					stringToVector(attriValue, tempUp);
				}
			}
			camera.Init(tempPos, tempTarget, tempUp);
		}

		else if (branch->branchName == "Font")
		{
			for (vector<Attribute>::iterator attri = branch->attributes.begin(); attri != branch->attributes.end(); ++attri)
			{
				Attribute tempAttri = *attri;
				string attriName = tempAttri.name;
				string attriValue = tempAttri.value;

				if (attriName == "Default")
				{
					defaultFontSize = stof(attriValue);
				}

				else if (attriName == "Special")
				{
					specialFontSize = stof(attriValue);
				}
			}
		}

		else if (branch->branchName == "GameSpeed")
		{
			for (vector<Attribute>::iterator attri = branch->attributes.begin(); attri != branch->attributes.end(); ++attri)
			{
				Attribute tempAttri = *attri;
				string attriName = tempAttri.name;
				string attriValue = tempAttri.value;
				if (attriName == "Speed")
				{
					gameSpeed = stof(attriValue);
				}
			}
		}

		else if (branch->branchName == "GameVolume")
		{
			for (vector<Attribute>::iterator attri = branch->attributes.begin(); attri != branch->attributes.end(); ++attri)
			{
				Attribute tempAttri = *attri;
				string attriName = tempAttri.name;
				string attriValue = tempAttri.value;
				if (attriName == "Volume")
				{
					gameVolume = stof(attriValue);
				}
			}
		}

		else if (branch->branchName == "Controls")
		{
			for (vector<Branch>::iterator childbranch = branch->childBranches.begin(); childbranch != branch->childBranches.end(); ++childbranch)
			{
				Branch tempChildBranch = *childbranch;
				KEYS tempKey;
				tempKey.name = "";
				tempKey.value;

				for (vector<Attribute>::iterator attri = childbranch->attributes.begin(); attri != childbranch->attributes.end(); ++attri)
				{
					Attribute tempAttri = *attri;
					string attriName = tempAttri.name;
					string attriValue = tempAttri.value;

					tempKey.name = tempChildBranch.branchName;

					if (attriName == "Key")
					{
						tempKey.value = stol(attriValue, NULL, 16);
					}
				}

				interactionKeys.push_back(tempKey);
			}

			for (vector<KEYS>::iterator key = interactionKeys.begin(); key != interactionKeys.end(); ++key)
			{
				KEYS tempKey = *key;
				tempKey.pressed = false;
			}
		}
	}
}

// Init all meshes needed in the scene from text file
void SceneGame::InitMesh(string config)
{
	Branch meshBranch = TextTree::FileToRead(config);

	for (vector<Branch>::iterator branch = meshBranch.childBranches.begin(); branch != meshBranch.childBranches.end(); ++branch)
	{
		string meshName = branch->branchName;
		Color meshColor;
		unsigned textureID = 0;

		enum MISC_VARIABLE
		{
			VAR_LENGTH,
			VAR_WIDTH,
			VAR_HEIGHT,
			VAR_RADIUS,
			VAR_INNER_RADIUS,
			VAR_OUTER_RADIUS,
			VAR_SLICES,
			VAR_STACKS,
			VAR_ANIM_TIME,
			MAX_VAR,
		};

		float meshVar[MAX_VAR];
		for (int i = 0; i < MAX_VAR; ++i)
		{
			meshVar[i] = 1.0f;
		}

		string meshVarNames[MAX_VAR] = 
		{
			"Length",
			"Width",
			"Height",
			"Radius",
			"InnerRadius",
			"OuerRadius",
			"Slices",
			"Stacks",
		};

		// default 2D mesh variables
		int meshPosX = 0;
		int meshPosY = 0;

		int meshTextRow = 0;
		int meshTextCol = 0;

		// Spirte Animation
		int meshSpriteRow = 0;
		int meshSpriteCol = 0;

		// Tile sheet
		int meshTileRow = 0;
		int meshTileCol = 0;

		string directory = "";
		string meshType = "";

		for (vector<Attribute>::iterator attri = branch->attributes.begin(); attri != branch->attributes.end(); ++attri)
		{
			Attribute tempAttri = *attri;
			string attriName = tempAttri.name;
			string attriValue = tempAttri.value;

			if (attriName == "ColorValue")
			{
				Vector3 tempColor;

				stringToVector(attriValue, tempColor);

				meshColor.Set(tempColor.x, tempColor.y, tempColor.z);
			}

			if (attriName == "ColorName")
			{
				meshColor = findColor(attriValue);
			}

			else if (attriName == "Type")
			{
				meshType = attriValue;
			}

			else if (attriName == "Directory")
			{
				directory = attriValue;
			}

			else if (attriName == "SpriteRow")
			{
				meshSpriteRow = stoi(attriValue);
			}

			else if (attriName == "SpriteCol")
			{
				meshSpriteCol = stoi(attriValue);
			}

			else if (attriName == "TextRow")
			{
				meshTextRow = stoi(attriValue);
			}

			else if (attriName == "TextCol")
			{
				meshTextCol = stoi(attriValue);
			}

			else if (attriName == "PosX")
			{
				meshPosX = stoi(attriValue);
			}

			else if (attriName == "PosY")
			{
				meshPosY = stoi(attriValue);
			}

			else if (attriName == "TileRow")
			{
				meshTileRow = stoi(attriValue);
			}

			else if (attriName == "TileCol")
			{
				meshTileCol = stoi(attriValue);
			}

			else if (attriName == "Texture")
			{
				textureID = LoadTGA(attriValue.c_str());
			}

			else
			{
				for (int k = 0; k < MAX_VAR; ++k)
				{
					if (attriName == meshVarNames[k])
					{
						meshVar[k] = stof(attriValue);
						break;
					}
				}
			}
		}

		// process data, generate mesh using meshbuilder
		Mesh* mesh = NULL;

		if (meshType == "Quad")
		{
			mesh = MeshBuilder::GenerateQuad(meshName, meshColor, meshVar[VAR_LENGTH], meshVar[VAR_WIDTH]);
		}

		else if (meshType == "Cube")
		{
			mesh = MeshBuilder::GenerateCube(meshName, meshColor, meshVar[VAR_LENGTH]);
		}

		else if (meshType == "Circle")
		{
			mesh = MeshBuilder::GenerateCircle(meshName, meshColor, (unsigned)meshVar[VAR_SLICES], meshVar[VAR_RADIUS]);
		}

		else if (meshType == "DebugQuad")
		{
			mesh = MeshBuilder::GenerateDebugQuad(meshName, meshColor, meshVar[VAR_LENGTH]);
		}

		else if (meshType == "DebugCircle")
		{
			mesh = MeshBuilder::GenerateDebugCircle(meshName, meshColor, (unsigned)meshVar[VAR_SLICES], meshVar[VAR_RADIUS]);
		}

		else if (meshType == "Sphere")
		{
			mesh = MeshBuilder::GenerateSphere(meshName, meshColor, (unsigned)meshVar[VAR_STACKS], (unsigned)meshVar[VAR_SLICES], meshVar[VAR_RADIUS]);
		}

		else if (meshType == "Obj")
		{
			mesh = MeshBuilder::GenerateOBJ(meshName, directory);
		}

		else if (meshType == "Text")
		{
			mesh = MeshBuilder::GenerateText(meshName, meshTextRow, meshTextCol);
		}

		else if (meshType == "2D")
		{
			mesh = MeshBuilder::Generate2DMesh(meshName, meshColor, (int)meshVar[VAR_WIDTH], (int)meshVar[VAR_HEIGHT]);
		}

		else if (meshType == "SpriteAnimation")
		{
			mesh = MeshBuilder::GenerateSpriteAnimation(meshName, meshSpriteRow, meshSpriteCol);

			SpriteAnimation *sa = dynamic_cast<SpriteAnimation*>(mesh);

			Branch tempBranch = *branch;
			// handle animations variables
			for (vector<Branch>::iterator childbranch = tempBranch.childBranches.begin(); childbranch != tempBranch.childBranches.end(); ++childbranch)
			{
				int id = 0;
				int startFrame = 0;
				int endFrame = 0;
				bool repeat = false;
				bool pause = false;
				float animationTime = 0.f;

				for (vector<Attribute>::iterator childAttri = childbranch->attributes.begin(); childAttri != childbranch->attributes.end(); ++childAttri)
				{
					Attribute tempAttri = *childAttri;
					string attriName = tempAttri.name;
					string attriValue = tempAttri.value;

					if (attriName == "ID")
					{
						id = stoi(attriValue);
					}

					else if (attriName == "StartFrame")
					{
						startFrame = stoi(attriValue);
					}

					else if (attriName == "EndFrame")
					{
						endFrame = stoi(attriValue);
					}

					else if (attriName == "Repeat")
					{
						stringToBool(attriValue, repeat);
					}

					else if (attriName == "Pause")
					{
						stringToBool(attriValue, pause);
					}

					else if (attriName == "AnimationTime")
					{
						animationTime = stof(attriValue);
					}
				}

				if (sa)
				{
					Animation* anime = new Animation();
					anime->Set(id, startFrame, endFrame, repeat, pause, animationTime);
					sa->animations.push_back(anime);
				}
			}

			//sort (sa->animations.begin(), sa->animations.end());
		}

		else if (meshType == "TileSheet")
		{
			mesh = MeshBuilder::GenerateTileSheet(meshName, meshTileRow, meshTileCol);
		}

		// push back mesh
		if (mesh != NULL)
		{
			meshList.push_back(mesh);

			if (textureID != NULL)
			{
				mesh->textureID = textureID;
			}
		}
	}
}

void SceneGame::InitColor(string config)
{
	Branch colorBranch = TextTree::FileToRead(config);

	for (vector<Branch>::iterator branch = colorBranch.childBranches.begin(); branch != colorBranch.childBranches.end(); ++branch)
	{
		Color tempColor;
		Vector3 colorValue;

		for (vector<Attribute>::iterator attri = branch->attributes.begin(); attri != branch->attributes.end(); ++attri)
		{
			Attribute tempAttri = *attri;
			string attriName = tempAttri.name;
			string attriValue = tempAttri.value;

			stringToVector(attriValue, colorValue);

			tempColor.Set(colorValue.x, colorValue.y, colorValue.z, branch->branchName);

			colorList.push_back(tempColor);
		}
	}
}

void SceneGame::InitInterface(string config)
{
	Branch interfaceBranch = TextTree::FileToRead(config);

	for (vector<Branch>::iterator branch = interfaceBranch.childBranches.begin(); branch != interfaceBranch.childBranches.end(); ++branch)
	{
		string name = "";
		string text = "";
		Buttons::BUTTON_TYPE type = Buttons::TEXT_BUTTON;
		Vector2 pos;
		float rotation = 0.f;
		Color tempColor;
		tempColor.Set(1, 1, 1);
		Vector2 scale;
		Mesh* mesh;

		int storagePos = -1;

		string gameStateName[MAX_STATE] = 
		{
			"Menu",
			"Ingame",
			"Instruction",
			"HighScore",
			"Options",
			"Pause",
			"GameOver",
		};

		for (vector<Attribute>::iterator attri = branch->attributes.begin(); attri != branch->attributes.end(); ++attri)
		{
			Attribute tempAttri = *attri;
			string attriName = tempAttri.name;
			string attriValue = tempAttri.value;

			name = branch->branchName;

			if (attriName == "Type")
			{
				if (attriValue == "Text")
				{
					type = Buttons::TEXT_BUTTON;
				}

				else
				{
					type = Buttons::IMAGE_BUTTON;
				}
			}

			else if (attriName == "Text")
			{
				text = attriValue;
			}

			else if (attriName == "Pos")
			{
				stringToVector(attriValue, pos);
			}

			else if (attriName == "Scale")
			{
				stringToVector(attriValue, scale);
			}

			else if (attriName == "Rotation")
			{
				rotation = stof(attriValue);
			}

			else if (attriName == "Color")
			{
				Vector3 tempCol;

				stringToVector(attriValue, tempCol);

				tempColor.Set(tempCol.x, tempCol.y, tempCol.z);
			}

			else if (attriName == "Mesh")
			{
				mesh = findMesh(attriValue);
			}

			else if (attriName == "GameState")
			{
				for (unsigned i = 0; i < MAX_STATE; ++i)
				{
					if (attriValue == gameStateName[i])
					{
						storagePos = i;
					}
				}
			}
		}

		Buttons tempButton;
		tempButton.Init(name, text, mesh, pos, scale, rotation, tempColor, type);
		gameInterfaces[storagePos].buttons.push_back(tempButton);
	}
}

void SceneGame::InitLevel(string config)
{
	Branch levelBranch = TextTree::FileToRead(config);

	for (vector<Branch>::iterator branch = levelBranch.childBranches.begin(); branch != levelBranch.childBranches.end(); ++branch)
	{
		TileMap::MAP_TYPE tempType;
		tempType = TileMap::MAX_TYPE;

		enum MAP_VAR
		{
			VAR_ID,
			VAR_BACKGROUND_ID,
			VAR_SCREEN_WIDTH,
			VAR_SCREEN_HEIGHT,
			VAR_MAP_WIDTH,
			VAR_MAP_HEIGHT,
			VAR_MAP_OFFSETX,
			VAR_MAP_OFFSETY,
			VAR_MAP_FINE_OFFSETX,
			VAR_MAP_FINE_OFFSETY,
			VAR_MAP_SCROLL_SPEED,
			TILE_SIZE,
			MAX_VAR,
		};

		int mapVar[MAX_VAR];

		for (int i = 0; i < MAX_VAR; ++i)
		{
			mapVar[i] = 0;
		}

		mapVar[TILE_SIZE] = TILESIZE;

		string mapVarNames[MAX_VAR] = 
		{
			"ID",
			"BackgroundID",
			"ScreenWidth",
			"ScreenHeight",
			"MapWidth",
			"MapHeight",
			"MapOffsetX",
			"MapOffsetY",
			"MapFineOffsetX",
			"MapFineOffsetY",
			"ScrollSpeed",
			"TileSize",
		};

		bool enableX = false;
		bool enableY = false;

		for (vector<Attribute>::iterator attri = branch->attributes.begin(); attri != branch->attributes.end(); ++attri)
		{
			Attribute tempAttri = *attri;
			string attriName = tempAttri.name;
			string attriValue = tempAttri.value;

			if (attriName == "EnableXScroll")
			{
				stringToBool(attriValue, enableX);
			}

			else if (attriName == "EnableYScroll")
			{
				stringToBool(attriValue, enableY);
			}

			else
			{
				for (int k = 0; k < MAX_VAR; ++k)
				{
					if (attriName == mapVarNames[k])
					{
						mapVar[k] = stoi(attriValue);
						break;
					}
				}
			}
		}

		if (branch->childBranches.size() != 0)
		{
			for (vector<Branch>::iterator childbranch = branch->childBranches.begin(); childbranch != branch->childBranches.end(); ++childbranch)
			{
				Branch tempChildBranch = *childbranch;
				string directory = "";
				TileMap::MAP_TYPE tempType;

				for (vector<Attribute>::iterator attri = childbranch->attributes.begin(); attri != childbranch->attributes.end(); ++attri)
				{
					Attribute tempAttri = *attri;
					string attriName = tempAttri.name;
					string attriValue = tempAttri.value;

					if (attriName == "Directory")
					{
						directory = attriValue;
					}

					else if (attriName == "Type")
					{
						if (attriValue == "Visual")
						{
							tempType = TileMap::TYPE_VISUAL;
						}

						else if (attriValue == "Back")
						{
							tempType = TileMap::TYPE_BACKGROUND;
						}

						else if (attriValue == "Collision")
						{
							tempType = TileMap::TYPE_COLLISION;

							if (childbranch->childBranches.size() != 0)
							{
								for (vector<Branch>::iterator childbranches = childbranch->childBranches.begin(); childbranches != childbranch->childBranches.end(); ++childbranches)
								{
									Branch tempChildBranch = *childbranches;

									if (tempChildBranch.branchName == "SpecialTiles")
									{
										SpecialTiles tiles;

										for (vector<Attribute>::iterator attri = childbranches->attributes.begin(); attri != childbranches->attributes.end(); ++attri)
										{
											Attribute tempAttri = *attri;
											string attriName = tempAttri.name;
											string attriValue = tempAttri.value;

											tiles.TileName = attriName;
											tiles.TileID = stoi(attriValue);

											layout[mapVar[VAR_ID]].specialTiles.push_back(tiles);
										}
									}
								}
							}
						}
					}
				}

				TileMap tempMap;
				tempMap.InitDynamic(mapVar[VAR_ID], mapVar[VAR_SCREEN_WIDTH], mapVar[VAR_SCREEN_HEIGHT], mapVar[VAR_MAP_WIDTH], mapVar[VAR_MAP_HEIGHT], mapVar[VAR_MAP_OFFSETX], mapVar[VAR_MAP_OFFSETY], mapVar[VAR_MAP_FINE_OFFSETX], mapVar[VAR_MAP_FINE_OFFSETY], enableX, enableY, (float)mapVar[VAR_MAP_SCROLL_SPEED], mapVar[TILE_SIZE]);
				tempMap.LoadMap(directory);
				tempMap.setMapType(tempType);

				layout[mapVar[VAR_ID]].ID = mapVar[VAR_ID];
				layout[mapVar[VAR_ID]].roomLayout.push_back(tempMap);
			}
		}
	}

	for (unsigned i = 0; i < layout.size(); ++i)
	{
		layout[i].rearrange();
	}
}

// Init all game variables in the scene from text file
void SceneGame::InitVariables(string config)
{
	Branch VariablesBranch = TextTree::FileToRead(config);

	for (vector<Branch>::iterator branch = VariablesBranch.childBranches.begin(); branch != VariablesBranch.childBranches.end(); ++branch)
	{
		if (branch->branchName == "StartTime")
		{
			for (vector<Attribute>::iterator attri = branch->attributes.begin(); attri != branch->attributes.end(); ++attri)
			{
				Attribute tempAttri = *attri;
				string attriName = tempAttri.name;
				string attriValue = tempAttri.value;
				int tempHr;
				int tempMin;
				int tempDay;

				for (vector<Attribute>::iterator attri = branch->attributes.begin(); attri != branch->attributes.end(); ++attri)
				{
					Attribute tempAttri = *attri;
					string attriName = tempAttri.name;
					string attriValue = tempAttri.value;
					if (attriName == "HOUR")
					{
						tempHr = stoi(attriValue);
					}
					else if(attriName == "MIN")
					{
						tempMin = stoi(attriValue);
					}
					else if(attriName == "DAY")
					{
						tempDay = stoi(attriValue);
					}
					else if(attriName == "DIFFICULTY")
					{
						day.setdifficulty(stof(attriValue));
					}
				}
				day.setCurrentTime(tempHr,tempMin,tempDay);
			}
		}
		else if (branch->branchName == "IconSprites")
		{
			for (vector<Branch>::iterator childbranch = branch->childBranches.begin(); childbranch != branch->childBranches.end(); ++childbranch)
			{
				Branch tempChildBranch = *childbranch;
				string name;
				Vector2 size;
				Vector2 pos;
				string mesh;

				for (vector<Attribute>::iterator attri = childbranch->attributes.begin(); attri != childbranch->attributes.end(); ++attri)
				{
					Attribute tempAttri = *attri;
					string attriName = tempAttri.name;
					string attriValue = tempAttri.value;

					if (attriName == "Name")
					{
						name = attriValue;
					}
					else if (attriName == "Size")
					{
						stringToVector(attriValue, size);
					}
					else if (attriName == "Pos")
					{
						stringToVector(attriValue, pos);
					}
					else if (attriName == "Mesh")
					{
						mesh = attriValue;
					}
				}
				day.Initicons(name,size,pos,mesh);
			}
		}
	}
}

void SceneGame::InitObjective(string config)
{
	Branch VariablesBranch = TextTree::FileToRead(config);
	for (vector<Branch>::iterator branch = VariablesBranch.childBranches.begin(); branch != VariablesBranch.childBranches.end(); ++branch)
	{
		if (branch->branchName == "Level")
		{
			for (vector<Branch>::iterator childbranch = branch->childBranches.begin(); childbranch != branch->childBranches.end(); ++childbranch)
			{
				childbranch->printBranch();
				Level templevel;
				//number branch
				for (vector<Branch>::iterator grandchildbranch = childbranch->childBranches.begin(); grandchildbranch != childbranch->childBranches.end(); ++grandchildbranch)
				{
					string Title;
					bool Get;
					int level;
					string keyItem; 
					for (vector<Attribute>::iterator attri = grandchildbranch->attributes.begin(); attri != grandchildbranch->attributes.end(); ++attri)
					{
						Attribute tempAttri = *attri;
						string attriName = tempAttri.name;
						string attriValue = tempAttri.value;
						if (attriName == "DISCRIPTION")
						{
							Title = attriValue;
						}
						else if (attriName == "GET")
						{
							stringToBool(attriValue,Get);
						}
						else if (attriName == "LEVEL")
						{
							level = stoi(attriValue);
						}
						else if (attriName == "KEYITEM")
						{
							keyItem = attriValue;
						}
					}
					Objective tempobjective;
					tempobjective.initObjctives(Title,Get,level,keyItem);
					templevel.objectives.push_back(tempobjective);
				}
				day.levels.push_back(templevel);
			}
		}
	}
}

// Init all game variables in the scene from text file
void SceneGame::InitSound(string config)
{
	Branch soundBranch = TextTree::FileToRead(config);
	{


		for (vector<Branch>::iterator branch = soundBranch.childBranches.begin(); branch != soundBranch.childBranches.end(); ++branch)
		{
			irrklang::ISoundEngine* Soundengine = irrklang::createIrrKlangDevice();
			string soundName = "";
			string soundFile = "";
			float volume = 0.f;
			bool loop = false;

			for (vector<Attribute>::iterator attri = branch->attributes.begin(); attri != branch->attributes.end(); ++attri)
			{
				Attribute tempAttri = *attri;
				string attriName = tempAttri.name;
				string attriValue = tempAttri.value;
				if(attriName=="SoundName")
				{
					soundName = attriValue;
				}
				if(attriName=="SoundFile")
				{
					soundFile = attriValue;
				}
				else if(attriName=="Loop")
				{
					if (attriValue == "true" || attriValue == "1")
					{
						loop = true;
					}
					else
					{
						loop = false;
					}
				}
				else if(attriName=="DefaultVol")
				{
					volume = stof(attriValue);
				}
			}
			Sound tempSound;
			tempSound.Init(soundName,soundFile,volume,loop);
			sound.sounds.push_back(tempSound);
		}
	}
}

void SceneGame::InitAI(string config)
{
	Branch AIBranch = TextTree::FileToRead(config);

	for (vector<Branch>::iterator branch = AIBranch.childBranches.begin(); branch != AIBranch.childBranches.end(); ++branch)
	{
		for (vector<Branch>::iterator childbranch = branch->childBranches.begin(); childbranch != branch->childBranches.end(); ++childbranch)
		{
			if (branch->branchName == "Guard")
			{
				Vector2 pos;
				Vector2 dir;
				string spriteName;
				int tiles = 0;
				int mapLocation = 0;

				for (vector<Attribute>::iterator childattri = childbranch->attributes.begin(); childattri != childbranch->attributes.end(); ++childattri)
				{
					Attribute tempAttri = *childattri;
					string attriName = tempAttri.name;
					string attriValue = tempAttri.value;

					if (attriName == "Pos")
					{
						stringToVector(attriValue, pos);
					}

					else if (attriName == "Dir")
					{
						stringToVector(attriValue, dir);
					}

					else if (attriName == "Tiles")
					{
						tiles = stoi(attriValue);
					}
					else if (attriName == "MapLocation")
					{
						mapLocation = stoi(attriValue);
					}

					else if (attriName == "Mesh")
					{
						spriteName = attriValue;
					}
				}

				Guards* guard = new Guards;
				guard->Init(pos * TILESIZE, dir, dynamic_cast<SpriteAnimation*>(findMesh(spriteName)), tiles, layout[mapLocation]);
				guard->changeAni(Guards_StateMachine::IDLE_STATE);
				guard->setRoom(layout[mapLocation]);
				guard->setSize(Vector2((float)TILESIZE, (float)TILESIZE));
				guardList.push_back(guard);
			}

			else if (branch->branchName == "Prisoner")
			{
				Vector2 pos;
				Vector2 dir;
				string spriteName;
				int tiles = 0;
				int mapLocation = 0;

				for (vector<Attribute>::iterator childattri = childbranch->attributes.begin(); childattri != childbranch->attributes.end(); ++childattri)
				{
					Attribute tempAttri = *childattri;
					string attriName = tempAttri.name;
					string attriValue = tempAttri.value;

					if (attriName == "Pos")
					{
						stringToVector(attriValue, pos);
					}

					else if (attriName == "Dir")
					{
						stringToVector(attriValue, dir);
					}

					else if (attriName == "Tiles")
					{
						tiles = stoi(attriValue);
					}
					else if (attriName == "MapLocation")
					{
						mapLocation = stoi(attriValue);
					}

					else if (attriName == "Mesh")
					{
						spriteName = attriValue;
					}
				}

				Prisoners* prisoner = new Prisoners;
				prisoner->Init(pos * TILESIZE, dir, dynamic_cast<SpriteAnimation*>(findMesh(spriteName)), tiles, layout[mapLocation]);
				prisoner->changeAni(Prisoners_StateMachine::IDLE_STATE);
				prisoner->setRoom(layout[mapLocation]);
				prisoner->setSize(Vector2((float)TILESIZE, (float)TILESIZE));
				prisonerList.push_back(prisoner);
			}
		}
	}
}

void SceneGame::InitPlayer(string config)
{
	Branch playerBranch = TextTree::FileToRead(config);

	for (vector<Branch>::iterator branch = playerBranch.childBranches.begin(); branch != playerBranch.childBranches.end(); ++branch)
	{
		Vector2 pos;
		Vector2 dir;
		string spriteName;
		int tiles = 0;
		int mapLocation = 0;

		for (vector<Attribute>::iterator attri = branch->attributes.begin(); attri != branch->attributes.end(); ++attri)
		{
			Attribute tempAttri = *attri;
			string attriName = tempAttri.name;
			string attriValue = tempAttri.value;

			if (attriName == "Pos")
			{
				stringToVector(attriValue, pos);
			}

			else if (attriName == "Dir")
			{
				stringToVector(attriValue, dir);
			}

			else if (attriName == "Tiles")
			{
				tiles = stoi(attriValue);
			}
			else if (attriName == "MapLocation")
			{
				mapLocation = stoi(attriValue);
			}

			else if (attriName == "Mesh")
			{
				spriteName = attriValue;
			}
		}

		player->Init(pos * TILESIZE, dir, dynamic_cast<SpriteAnimation*>(findMesh(spriteName)), tiles, layout[mapLocation]);
		player->setSize(Vector2((float)TILESIZE, (float)TILESIZE));
		player->setState(StateMachine::IDLE_STATE);
		player->setRoom(layout[mapLocation]);
	}
}

void SceneGame::InitInteractions(string config)
{

}

void SceneGame::UpdateOpengl(void)
{
	if(KEngine::getKeyboard()->getKey('1'))
		glEnable(GL_CULL_FACE);

	if(KEngine::getKeyboard()->getKey('2'))
		glDisable(GL_CULL_FACE);

	if(KEngine::getKeyboard()->getKey('3'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	if(KEngine::getKeyboard()->getKey('4'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void SceneGame::UpdateInput(void)
{
	for (unsigned i = 0; i < interactionKeys.size(); ++i)
	{
		interactionKeys[i].pressed = KEngine::getKeyboard()->getKey(interactionKeys[i].value);
	}
}

void SceneGame::UpdateMouse(void)
{
	double x, y;
	x = KEngine::getMouse()->getCurrentPosX();
	y = KEngine::getMouse()->getCurrentPosY();

	int w = KEngine::getWindowWidth();
	int h = KEngine::getWindowHeight();

	mousePos.Set((float)x * sceneWidth / w, (h - (float)y) * sceneHeight / h);
}

void SceneGame::UpdateState(void)
{
	bool updated = false;
	for (unsigned i = 0; i < gameInterfaces[currentState].buttons.size() && !updated; ++i)
	{
		if (gameInterfaces[currentState].buttons[i].getStatus() == Buttons::BUTTON_RELEASED)
		{
			switch(currentState)
			{
			case MENU_STATE:
				{
					if (gameInterfaces[currentState].buttons[i].getName() == "Play")
					{
						changeScene(INGAME_STATE);
					}

					else if (gameInterfaces[currentState].buttons[i].getName() == "Instruction")
					{
						changeScene(INSTRUCTION_STATE);
					}

					else if (gameInterfaces[currentState].buttons[i].getName() == "HighScore")
					{
						changeScene(HIGHSCORE_STATE);
					}

					else if (gameInterfaces[currentState].buttons[i].getName() == "Options")
					{
						changeScene(OPTIONS_STATE);
					}

					else if (gameInterfaces[currentState].buttons[i].getName() == "Exit")
					{
						changeScene(EXIT_STATE);
					}
					break;
				}
			case INGAME_STATE:
				{
					if (gameInterfaces[currentState].buttons[i].getName() == "Pause")
					{
						changeScene(PAUSE_STATE);
					}

					else if (gameInterfaces[currentState].buttons[i].getName() == "Instruction")
					{
						changeScene(INSTRUCTION_STATE);
					}
					break;
				}
			case INSTRUCTION_STATE:
				{
					if (gameInterfaces[currentState].buttons[i].getName() == "Return")
					{
						changeScene(MENU_STATE);
					}
					break;
				}
			case HIGHSCORE_STATE:
				{
					if (gameInterfaces[currentState].buttons[i].getName() == "Return")
					{
						changeScene(MENU_STATE);
					}
					break;
				}
			case OPTIONS_STATE:
				{
					if (gameInterfaces[currentState].buttons[i].getName() == "Return")
					{
						changeScene(MENU_STATE);
					}
					break;
				}
			case PAUSE_STATE:
				{
					if (gameInterfaces[currentState].buttons[i].getName() == "Resume")
					{
						changeScene(INGAME_STATE);
					}

					else if (gameInterfaces[currentState].buttons[i].getName() == "Exit")
					{
						changeScene(MENU_STATE);
					}
					break;
				}
			case EXIT_STATE:
				{
					break;
				}
			}

			updated = true;
		}
	}
}

void SceneGame::UpdateEffect(void)
{	
	static bool played = false;
	for (unsigned i = 0; i < gameInterfaces[currentState].buttons.size(); ++i)
	{
		if (gameInterfaces[currentState].buttons[i].getStatus() == Buttons::BUTTON_HOVER)
		{
			gameInterfaces[currentState].buttons[i].setColor(findColor("White"));
			gameInterfaces[currentState].buttons[i].setRotation(3.f);

			if (!played)
			{
				sound.Play("Sound_Bookflip2");
				played = true;
			}
		}
		else
		{
			gameInterfaces[currentState].buttons[i].setColor(findColor("LightGrey"));
			gameInterfaces[currentState].buttons[i].setRotation(1.f);
		}
	}

	unsigned numStatus = 0;
	for (unsigned i = 0; i < gameInterfaces[currentState].buttons.size(); ++i)
	{
		if (gameInterfaces[currentState].buttons[i].getStatus() == Buttons::BUTTON_IDLE)
		{
			++numStatus;
		}

		if (numStatus == gameInterfaces[currentState].buttons.size())
		{
			played = false;
		}
	}
}

void SceneGame::UpdateInGame(double dt)
{
	UpdatePlayer(dt);
	UpdateAI(dt);
	UpdateMap();
	UpdateInteractions();
	day.UpdateDay(dt,gameSpeed);
}

void SceneGame::UpdatePlayer(double dt)
{
	static bool movable = true;

	movable = true;
	if (player->getState() == StateMachine::IDLE_STATE || player->getState() == StateMachine::RUN_STATE)
	{
		if (getKey("Up"))
		{
			if (player->getDir().y != 1)
			{
				player->setDir(Vector2(0, 1));
			}

			else
			{
				player->setTargetPos(Vector2(player->getTargetPos().x, player->getTargetPos().y + TILESIZE));

				if (getKey("ToggleShift") && !player->GetRecovering())
				{
					player->changeAni(StateMachine::RUN_STATE);
				}
				else
				{
					player->changeAni(StateMachine::WALK_STATE);
				}

				//std::cout << player->getPos().x << " , " << player->getPos().y << "\n";
			}
		}

		if (getKey("Down"))
		{
			if (player->getDir().y != -1)
			{
				player->setDir(Vector2(0, -1));
			}

			else
			{
				player->setTargetPos(Vector2(player->getTargetPos().x, player->getTargetPos().y - TILESIZE));

				if (getKey("ToggleShift") && !player->GetRecovering())
				{
					player->changeAni(StateMachine::RUN_STATE);
				}
				else
				{
					player->changeAni(StateMachine::WALK_STATE);
				}
			}
		}

		if (getKey("Left"))
		{
			if (player->getDir().x != -1)
			{
				player->setDir(Vector2(-1, 0));
			}

			else
			{
				player->setTargetPos(Vector2(player->getTargetPos().x - TILESIZE, player->getTargetPos().y));

				if (getKey("ToggleShift") && !player->GetRecovering())
				{
					player->changeAni(StateMachine::RUN_STATE);
				}
				else
				{
					player->changeAni(StateMachine::WALK_STATE);
				}
			}
		}

		if (getKey("Right"))
		{
			if (player->getDir().x != 1)
			{
				player->setDir(Vector2(1, 0));
			}

			else
			{
				player->setTargetPos(Vector2(player->getTargetPos().x + TILESIZE, player->getTargetPos().y));

				if (getKey("ToggleShift") && !player->GetRecovering())
				{
					player->changeAni(StateMachine::RUN_STATE);
				}
				else
				{
					player->changeAni(StateMachine::WALK_STATE);
				}
			}
		}
	}

	for (unsigned special = 0; special < layout[currentLocation].specialTiles.size(); ++special)
	{
		if (layout[currentLocation].specialTiles[special].TileName == "Threadmill")
		{
			if(layout[currentLocation].roomLayout[TileMap::TYPE_VISUAL].screenMap[(sceneHeight-player->getPos().y - TILESIZE)/TILESIZE][(player->getPos().x)/TILESIZE] == layout[currentLocation].specialTiles[special].TileID)
			{
				currentInteraction = RUNNING_ON_THREADMILL;
				std::cout << player->getPos().x / TILESIZE << ", " << (sceneHeight - player->getPos().y  - TILESIZE) / TILESIZE << std::endl;
			}
			else
			{
				currentInteraction = NO_INTERACTION;
			}
		}
	}

	player->tileBasedMovement((int)sceneWidth, (int)sceneHeight, TILESIZE, dt);
	player->ConstrainPlayer(dt);
	player->Update(dt);
}

void SceneGame::UpdateAI(double dt)
{
	for (vector<Prisoners*>::iterator prisoner = prisonerList.begin(); prisoner != prisonerList.end(); ++prisoner)
	{
		Prisoners* tempPrisoner = *prisoner;

		if (tempPrisoner->getRoom().ID == player->getRoom().ID)
		{
			tempPrisoner->setRender(true);
		}

		else
		{
			tempPrisoner->setRender(false);
		}
		tempPrisoner->tileBasedMovement((int)sceneWidth, (int)sceneHeight, TILESIZE, dt);
		tempPrisoner->Update(dt);
	}

	for (vector<Guards*>::iterator guard = guardList.begin(); guard != guardList.end(); ++guard)
	{
		Guards* tempGuard = *guard;

		if (tempGuard->getRoom().ID == player->getRoom().ID)
		{
			tempGuard->setRender(true);
		}

		else
		{
			tempGuard->setRender(false);
		}
		tempGuard->tileBasedMovement((int)sceneWidth, (int)sceneHeight, TILESIZE, dt);
		tempGuard->Update(dt);
	}
}

void SceneGame::UpdateMap(void)
{
	if(KEngine::getKeyboard()->getKey('Z'))
	{
		this->currentLocation = CELL_AREA;
		player->setRoom(layout[CELL_AREA]);
		player->setPos(Vector2(15 * 32,15 * 32));
	}

	if(KEngine::getKeyboard()->getKey('X'))
	{
		this->currentLocation = EXERCISE_AREA;
		player->setRoom(layout[EXERCISE_AREA]);
		player->setPos(Vector2(15 * 32,15 * 32));
	}

	if(KEngine::getKeyboard()->getKey('C'))
	{
		this->currentLocation = CANTEEN_AREA;
		player->setRoom(layout[CANTEEN_AREA]);
		player->setPos(Vector2(0,0));
	}

	if(KEngine::getKeyboard()->getKey('V'))
	{
		this->currentLocation = SHOWER_AREA;
		player->setRoom(layout[SHOWER_AREA]);
		player->setPos(Vector2(0,0));
	}

	if(KEngine::getKeyboard()->getKey('B'))
	{
		this->currentLocation = MEETING_AREA;
		player->setRoom(layout[MEETING_AREA]);
		player->setPos(Vector2(0,0));
	}

	if(KEngine::getKeyboard()->getKey('N'))
	{
		this->currentLocation = COURTYARD_AREA;
		player->setRoom(layout[COURTYARD_AREA]);
		player->setPos(Vector2(0,0));
	}

	this->layout[currentLocation] = player->getRoom();
	for (unsigned i = 0; i < layout[currentLocation].roomLayout.size(); ++i)
	{
		this->layout[currentLocation].roomLayout[i].Update();
	}
}

void SceneGame::UpdateInteractions(void)
{
	switch (currentInteraction)
	{
	case NO_INTERACTION:
		std::cout << "No Interaction" << std::endl;
		break;
	case PICKUP_ITEM:;
		break;
	case DROP_ITEM:;
		break;
	case TALK_WITH_PRISONERS:;
		break;
	case TALK_WITH_GUARDS:;
		break;
	case OPEN_DOOR:;
		break;
	case CLOSE_DOOR:;
		break;
	case RUNNING_ON_THREADMILL:
		std::cout << "Running on threadmill" << std::endl;
		player->setDir(Vector2(-1,0));
		player->changeAni(StateMachine::RUN_STATE);
		break;
	case ATTACK:;
		break;
	default:;
		break;
	}
}

void SceneGame::changeScene(GAME_STATE nextState)
{
	this->currentState = nextState;	
	if(nextState==INGAME_STATE)
	{
		sound.Play("Sound_Background");	
		sound.Play("Sound_Bookflip2");
	}
	else
	{
		sound.Stop("Sound_Background");	
		sound.Play("Sound_Bookflip2");
	}
}

void SceneGame::RenderInterface(void)
{
	for(unsigned i = 0; i < gameInterfaces[currentState].buttons.size(); ++i)
	{
		if (gameInterfaces[currentState].buttons[i].getType() == Buttons::TEXT_BUTTON)
		{
			RenderTextOnScreen(gameInterfaces[currentState].buttons[i].getMesh(), gameInterfaces[currentState].buttons[i].getText(), gameInterfaces[currentState].buttons[i].getColor(), defaultFontSize, gameInterfaces[currentState].buttons[i].getPos().x, gameInterfaces[currentState].buttons[i].getPos().y, gameInterfaces[currentState].buttons[i].getRotation());

		}

		else
		{
			Render2DMesh(gameInterfaces[currentState].buttons[i].getMesh(), false, gameInterfaces[currentState].buttons[i].getScale(), Vector2(gameInterfaces[currentState].buttons[i].getPos().x + gameInterfaces[currentState].buttons[i].getScale().x * 0.5f, gameInterfaces[currentState].buttons[i].getPos().y + gameInterfaces[currentState].buttons[i].getScale().y * 0.5f), gameInterfaces[currentState].buttons[i].getRotation());
			//Render2DMesh(gameInterfaces[currentState].buttons[i].getMesh(), false, gameInterfaces[currentState].buttons[i].getScale(), gameInterfaces[currentState].buttons[i].getPos(), gameInterfaces[currentState].buttons[i].getRotation());
		}

		if (DEBUG)
		{
			Render2DMesh(findMesh("GEO_DEBUGQUAD"), false, gameInterfaces[currentState].buttons[i].getScale(), Vector2(gameInterfaces[currentState].buttons[i].getPos().x + gameInterfaces[currentState].buttons[i].getScale().x * 0.5f, gameInterfaces[currentState].buttons[i].getPos().y + gameInterfaces[currentState].buttons[i].getScale().y * 0.5f), gameInterfaces[currentState].buttons[i].getRotation());
		}
	}

	glDisable(GL_DEPTH_TEST);
}

void SceneGame::RenderLevel(void)
{
	// check if player is within the game
	if ((unsigned)currentLocation < layout.size())
	{
		for (unsigned numMaps = 0; numMaps < layout[currentLocation].roomLayout.size(); ++numMaps)
		{
			int m = 0;
			int n = 0;
			for(int i = 0; i < layout[currentLocation].roomLayout[numMaps].getNumTilesHeight() + 1; i++)
			{
				n = -(layout[currentLocation].roomLayout[numMaps].getTileOffsetY()) + i;

				for(int k = 0; k < layout[currentLocation].roomLayout[numMaps].getNumTilesWidth() + 1; k++)
				{
					m = layout[currentLocation].roomLayout[numMaps].getTileOffsetX() + k;
					
					if (m >= layout[currentLocation].roomLayout[numMaps].getNumTilesMapWidth() || m < 0)
						break;
					if (n >= layout[currentLocation].roomLayout[numMaps].getNumTilesMapHeight() || n < 0)
						break;

					if (layout[currentLocation].roomLayout[numMaps].getMapType() != TileMap::TYPE_COLLISION)
					{
						TileSheet *tilesheet = dynamic_cast<TileSheet*>(findMesh("GEO_TILESHEET"));
						tilesheet->m_currentTile = layout[currentLocation].roomLayout[numMaps].screenMap[n][m];

						if (tilesheet->m_currentTile != -1)
						{
							Render2DMesh(findMesh("GEO_TILESHEET"), false, (float)layout[currentLocation].roomLayout[numMaps].getTileSize() + 3, (k + 0.5f) * layout[currentLocation].roomLayout[numMaps].getTileSize() - layout[currentLocation].roomLayout[numMaps].getMapFineOffsetX(), layout[currentLocation].roomLayout[numMaps].getScreenHeight() - (float)(i + 0.5f) * layout[currentLocation].roomLayout[numMaps].getTileSize() - layout[currentLocation].roomLayout[numMaps].getMapFineOffsetY());
						}
					}

					else
					{
						if (DEBUG)
						{
							int collideTile = layout[currentLocation].roomLayout[numMaps].screenMap[n][m];

							if (collideTile != -1 && layout[currentLocation].roomLayout[numMaps].getMapType() == TileMap::TYPE_COLLISION)
							{
								Render2DMesh(findMesh("GEO_DEBUGQUAD"), false, (float)layout[currentLocation].roomLayout[numMaps].getTileSize() + 3, (k + 0.5f) * layout[currentLocation].roomLayout[numMaps].getTileSize() - layout[currentLocation].roomLayout[numMaps].getMapFineOffsetX(), layout[currentLocation].roomLayout[numMaps].getScreenHeight() - (float)(i + 0.5f) * layout[currentLocation].roomLayout[numMaps].getTileSize() - layout[currentLocation].roomLayout[numMaps].getMapFineOffsetY());
							}
						}
					}
				}
			}
		}
	}
}

void SceneGame::RenderCharacters(void)
{
	// Render player
	//Render2DMesh(player->getSprite(), false, TILESIZE, player->getPos().x + layout[currentLocation].roomLayout[0].getMapOffsetX(), player->getPos().y - layout[currentLocation].roomLayout[0].getMapOffsetY());
	Render2DMesh(player->getSprite(), false, (float)TILESIZE * 1.5f, player->getPos().x + TILESIZE * 0.5f - layout[currentLocation].roomLayout[TileMap::TYPE_VISUAL].getMapOffsetX(), player->getPos().y + TILESIZE * 0.5f - layout[currentLocation].roomLayout[TileMap::TYPE_VISUAL].getMapOffsetY());
	//std::cout <<  layout[currentLocation].roomLayout[TileMap::TYPE_VISUAL].getMapOffsetX() << std::endl;

	if (DEBUG)
	{
		Render2DMesh(findMesh("GEO_DEBUGQUAD"), false, (float)TILESIZE, player->getPos().x + TILESIZE * 0.5f - layout[currentLocation].roomLayout[TileMap::TYPE_VISUAL].getMapOffsetX(), player->getPos().y + TILESIZE * 0.5f - layout[currentLocation].roomLayout[TileMap::TYPE_VISUAL].getMapOffsetY());
	}

	for (vector<Guards*>::iterator guard = guardList.begin(); guard != guardList.end(); ++guard)
	{
		Guards* tempGuard = *guard;

		if (tempGuard->getRender())
		{
			Render2DMesh(tempGuard->getSprite(), false, (float)TILESIZE * 1.5f, tempGuard->getPos().x + TILESIZE * 0.5f - layout[currentLocation].roomLayout[TileMap::TYPE_VISUAL].getMapOffsetX(), tempGuard->getPos().y + TILESIZE * 0.5f - layout[currentLocation].roomLayout[TileMap::TYPE_VISUAL].getMapOffsetY());

			if (DEBUG)
			{
				Render2DMesh(findMesh("GEO_DEBUGQUAD"), false, (float)TILESIZE, tempGuard->getPos().x + TILESIZE * 0.5f - layout[currentLocation].roomLayout[TileMap::TYPE_VISUAL].getMapOffsetX(),  tempGuard->getPos().y + TILESIZE * 0.5f - layout[currentLocation].roomLayout[TileMap::TYPE_VISUAL].getMapOffsetY());
			}
		}
	}

	for (vector<Prisoners*>::iterator prisoner = prisonerList.begin(); prisoner != prisonerList.end(); ++prisoner)
	{
		Prisoners* tempPrisoner = *prisoner;

		if (tempPrisoner->getRender())
		{
			Render2DMesh(tempPrisoner->getSprite(), false, (float)TILESIZE * 1.5f, tempPrisoner->getPos().x + TILESIZE * 0.5f - layout[currentLocation].roomLayout[TileMap::TYPE_VISUAL].getMapOffsetX(), tempPrisoner->getPos().y + TILESIZE * 0.5f - layout[currentLocation].roomLayout[TileMap::TYPE_VISUAL].getMapOffsetY());

			if (DEBUG)
			{
				Render2DMesh(findMesh("GEO_DEBUGQUAD"), false, (float)TILESIZE, tempPrisoner->getPos().x + TILESIZE * 0.5f - layout[currentLocation].roomLayout[TileMap::TYPE_VISUAL].getMapOffsetX(),  tempPrisoner->getPos().y + TILESIZE * 0.5f - layout[currentLocation].roomLayout[TileMap::TYPE_VISUAL].getMapOffsetY());
			}
		}
	}
}

void SceneGame::RenderTime(void)
{
	if(day.getCurrentTime().hour >= 18 || day.getCurrentTime().hour >= 0 && day.getCurrentTime().hour <6)
	{
		std::ostringstream ss;
		ss.precision(2);
		ss << day.getCurrentTime().hour << ":" << day.getCurrentTime().min ;
		RenderTextOnScreen(findMesh("GEO_TEXT"), ss.str(), findColor("LightGrey"), specialFontSize, 0,sceneHeight - specialFontSize );

		std::ostringstream ss2;
		ss2.precision(1);
		ss2<< day.getCurrentTime().day;
		RenderTextOnScreen(findMesh("GEO_TEXT"), ss2.str(), findColor("Skyblue"), specialFontSize,day.moon.pos.x+ specialFontSize, day.moon.pos.y);

		Render2DMesh(findMesh(day.moon.mesh),false, day.moon.size, day.moon.pos);
		if (DEBUG)
		{
			Render2DMesh(findMesh("GEO_DEBUGQUAD"),false, day.moon.size, day.moon.pos);
		}
		//16, 736 original position
	}

	if(day.getCurrentTime().hour >= 6 && day.getCurrentTime().hour <18)
	{
		std::ostringstream ss;
		ss.precision(2);
		ss << day.getCurrentTime().hour << ":" << day.getCurrentTime().min ;
		RenderTextOnScreen(findMesh("GEO_TEXT"), ss.str(), findColor("Skyblue"), specialFontSize, 0, sceneHeight - specialFontSize);

		std::ostringstream ss2;
		ss2.precision(1);
		ss2<< day.getCurrentTime().day;
		RenderTextOnScreen(findMesh("GEO_TEXT"), ss2.str(), findColor("Skyblue"), specialFontSize,day.sun.pos.x + specialFontSize, day.sun.pos.y );

		Render2DMesh(findMesh(day.sun.mesh),false, day.sun.size, day.sun.pos);

		if (DEBUG)
		{
			Render2DMesh(findMesh("GEO_DEBUGQUAD"),false, day.sun.size, day.sun.pos);
		}


	}

	glDisable(GL_DEPTH_TEST);
}

void SceneGame::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y, float rotation)
{
	if(!mesh || mesh->textureID <= 0)
		return;

	glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, double(sceneWidth), 0, double(sceneHeight), -10, 10);
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity();
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	modelStack.Translate(x, y, 0);
	if (rotation != 0.f)
	{
		modelStack.Rotate(rotation, 0, 0, 1);
	}
	modelStack.Scale(size, size, size);

	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for(unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 1.0f + 0.5f, 0.5f, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	modelStack.PopMatrix();
	viewStack.PopMatrix();
	projectionStack.PopMatrix();
	glEnable(GL_DEPTH_TEST);
}

void SceneGame::Render3DMesh(Mesh* mesh, bool enableLight)
{
	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

	modelView = viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);

	if (enableLight && bLightEnabled)
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);
		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView_inverse_transpose.a[0]);

		//load material
		glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
		glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
		glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
		glUniform1f(m_parameters[U_MATERIAL_SHININESS], mesh->material.kShininess);
	}

	else
	{	
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	}

	if(mesh->textureID != NULL)
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureID);
		glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	}
	else
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	}
	mesh->Render();
}

void SceneGame::Render2DMesh(Mesh *mesh, const bool enableLight, const float size, const float x, const float y, const float rotation)
{
	Mtx44 ortho;
	ortho.SetToOrtho(0, double(sceneWidth), 0, double(sceneHeight), -100, 100);
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity();
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	modelStack.Translate(x, y, 0);
	modelStack.Scale(size, size, size);
	if (rotation != 0.f)
		modelStack.Rotate(rotation, 0, 0, 1);

	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

	if(mesh->textureID != NULL)
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureID);
		glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	}
	else
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	}
	mesh->Render();

	if(mesh->textureID > 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	modelStack.PopMatrix();
	viewStack.PopMatrix();
	projectionStack.PopMatrix();
}

void SceneGame::Render2DMesh(Mesh *mesh, const bool enableLight, const Vector2 scale , const Vector2 pos, const float rotation)
{
	Mtx44 ortho;
	ortho.SetToOrtho(0, double(sceneWidth), 0, double(sceneHeight), -100, 100);
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity();
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	modelStack.Translate(pos.x, pos.y, 0);
	modelStack.Scale(scale.x, scale.y, 1);
	if (rotation != 0.f)
		modelStack.Rotate(rotation, 0, 0, 1);

	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

	if(mesh->textureID != NULL)
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureID);
		glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	}
	else
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	}
	mesh->Render();

	if(mesh->textureID > 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	modelStack.PopMatrix();
	viewStack.PopMatrix();
	projectionStack.PopMatrix();
}

Mesh* SceneGame::findMesh(string meshName)
{
	for (vector<Mesh*>::iterator it = meshList.begin(); it != meshList.end(); ++it)
	{
		Mesh* tempMesh = *it;

		if (tempMesh->name == meshName)
		{
			return tempMesh;
		}
	}

	std::cout << "Unable to find mesh! Check your naming" << std::endl;

	return NULL;
}

Color SceneGame::findColor(string colorName)
{
	for (vector<Color>::iterator it = colorList.begin(); it != colorList.end(); ++it)
	{
		Color tempColor = *it;

		if (tempColor.name == colorName)
		{
			return tempColor;
		}
	}

	std::cout << "Unable to find color! Check your naming" << std::endl;

	return Color(1, 1, 1);
}

bool SceneGame::getKey(string keyName)
{
	for (unsigned i = 0; i < interactionKeys.size(); ++i)
	{
		if (keyName == interactionKeys[i].name)
		{
			return interactionKeys[i].pressed;
		}
	}

	return false;
}

void SceneGame::stringToVector(string text, Vector2 &vec)
{
	int lastContinue = 0;
	string xValue;
	string yValue;

	for (int position = 0; position < 2; ++position)
	{
		for (unsigned j = lastContinue; j < text.size() && text[j] != ','; ++j)
		{
			if (position == 0)
			{
				xValue += text[j];
			}

			else if (position == 1)
			{
				yValue += text[j];
			}

			lastContinue = j + 2;
		}
	}

	vec.Set(stof(xValue), stof(yValue));
}

void SceneGame::stringToVector(string text, Vector3 &vec)
{
	int lastContinue = 0;
	string xValue;
	string yValue;
	string zValue;

	for (int position = 0; position < 3; ++position)
	{
		for (unsigned j = lastContinue; j < text.size() && text[j] != ','; ++j)
		{
			if (position == 0)
			{
				xValue += text[j];
			}

			else if (position == 1)
			{
				yValue += text[j];
			}

			else
			{
				zValue += text[j];
			}

			lastContinue = j + 2;
		}
	}

	vec.Set(stof(xValue), stof(yValue), stof(zValue));
}

void SceneGame::stringToBool(string text, bool &boo)
{
	if (text == "True" || text == "1" || text == "true")
	{
		boo = true;
	}

	else
	{
		boo = false;
	}
}