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
	for (int i = 0; i < MAX_AREAS; ++i)
	{
		layout[i] = new Room;
	}
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
	FOG = true;

	energyScale = 85;

	currentInteraction = NO_INTERACTION;

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
	energyTranslate = sceneWidth * 0.105f;
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
	UpdateEnergy(dt);
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
			RenderItem();
			RenderCharacters();
			if(FOG)
			{
				RenderFOV();
			}
			RenderTime();
			RenderInterface(renderInventory);
			RenderPlayerInventory(renderInventory);
			RenderObjectives();
			RenderDialogs();
			RenderItemOnMouse(getKey("Select"));
			RenderCursor();
			RenderEnergy();

			break;
		}
	case INSTRUCTION_STATE:
		{
			RenderInstruct();
			break;
		}
	case HIGHSCORE_STATE:
		{
			//save(ScoreDirectory);
			RenderScore();
			break;
		}
	case OPTIONS_STATE:
		{
		
			break;
		}
	case PAUSE_STATE:
		{
			RenderLevel();
			RenderItem();
			RenderCharacters();
			if(FOG)
			{
				RenderFOV();
			}
			RenderTime();
			RenderInterface(renderInventory);
			RenderPlayerInventory(renderInventory);
			RenderObjectives();
			RenderDialogs();
			RenderItemOnMouse(getKey("Select"));
			RenderCursor();
			RenderEnergy();
			Render2DMesh(findMesh("GEO_Semi_Quad"), false,Vector2(sceneWidth,sceneHeight),Vector2(sceneWidth*0.5f,sceneHeight*0.5f),0);
			break;
		}
	case EXIT_STATE:
		{
			break;
		}
	}

	if (currentState != SceneGame::INGAME_STATE || currentState != SceneGame::PAUSE_STATE)
	{
		RenderInterface(false);
		RenderCursor();
	}

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

		else if (branch->branchName == "Item")
		{
			for (vector<Attribute>::iterator attri = branch->attributes.begin(); attri != branch->attributes.end(); ++attri)
			{
				Attribute tempAttri = *attri;
				string attriName = tempAttri.name;
				string attriValue = tempAttri.value;
				if (attriName == "Directory")
				{
					InitItem(attriValue);
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
		else if (branch->branchName == "Instructions")
		{
			for (vector<Attribute>::iterator attri = branch->attributes.begin(); attri != branch->attributes.end(); ++attri)
			{
				Attribute tempAttri = *attri;
				string attriName = tempAttri.name;
				string attriValue = tempAttri.value;
				if (attriName == "Directory")
				{
					InitInstruct(attriValue);
				}
			}
		}
		else if (branch->branchName == "Dialog")
		{
			for (vector<Attribute>::iterator attri = branch->attributes.begin(); attri != branch->attributes.end(); ++attri)
			{
				Attribute tempAttri = *attri;
				string attriName = tempAttri.name;
				string attriValue = tempAttri.value;
				if (attriName == "Directory")
				{
					InitInteractions(attriValue);
				}
			}
		}
		else if (branch->branchName == "Score")
		{
			for (vector<Attribute>::iterator attri = branch->attributes.begin(); attri != branch->attributes.end(); ++attri)
			{
				Attribute tempAttri = *attri;
				string attriName = tempAttri.name;
				string attriValue = tempAttri.value;
				if (attriName == "Directory")
				{
					InitScore(attriValue);
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
	// various properties of the game (width, height, controls, etc)
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
	// various colors needed for text

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

void SceneGame::InitInstruct(string config)
{
	Branch InstructBranch = TextTree::FileToRead(config);
	// Instructions for the game
	for (vector<Branch>::iterator branch = InstructBranch.childBranches.begin(); branch != InstructBranch.childBranches.end(); ++branch)
	{
		Instructions tempInstruct;
		string temptext;
		for (vector<Attribute>::iterator attri = branch->attributes.begin(); attri != branch->attributes.end(); ++attri)
		{
			Attribute tempAttri = *attri;
			string attriName = tempAttri.name;
			string attriValue = tempAttri.value;

			temptext = attriValue;

			tempInstruct.InitInstructions( branch->branchName,temptext);
			instructions.push_back(tempInstruct);
		}
	}
}

void SceneGame::InitInterface(string config)
{
	Branch interfaceBranch = TextTree::FileToRead(config);
	// Init interfaces of the game, buttons etc..

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
	// Processing all levels layout

	for (vector<Branch>::iterator branch = levelBranch.childBranches.begin(); branch != levelBranch.childBranches.end(); ++branch)
	{
		TileMap::MAP_TYPE tempType;
		tempType = TileMap::MAX_TYPE;
		string roomName = branch->branchName;

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

		layout[mapVar[VAR_ID]]->name = roomName;
		layout[mapVar[VAR_ID]]->ID = mapVar[VAR_ID];

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

									// handling special tiles for collision & interaction purposes
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

											layout[mapVar[VAR_ID]]->specialTiles.push_back(tiles);
										}
									}

									// handling doors in all the rooms for transition between rooms purposes
									else if (tempChildBranch.branchName == "Door")
									{
										Door tempDoor;
										tempDoor.status = false;

										for (vector<Attribute>::iterator attri = childbranches->attributes.begin(); attri != childbranches->attributes.end(); ++attri)
										{
											Attribute tempAttri = *attri;
											string attriName = tempAttri.name;
											string attriValue = tempAttri.value;

											if (tempAttri.name == "Transition")
											{
												tempDoor.transitionRoom = stoi(attriValue);
											}

											else if (tempAttri.name == "ID")
											{
												tempDoor.ID = stoi(attriValue);
											}

											else if (tempAttri.name == "Position")
											{
												stringToVector(attriValue, tempDoor.pos);
											}
										}
										layout[mapVar[VAR_ID]]->doors.push_back(tempDoor);
									}
								}
							}
						}

						// For AI waypoints purposes
						else if (attriValue == "WayPoint")
						{
							tempType = TileMap::TYPE_WAYPOINT;
						}
					}
				}

				TileMap tempMap;
				tempMap.InitDynamic(mapVar[VAR_ID], mapVar[VAR_SCREEN_WIDTH], mapVar[VAR_SCREEN_HEIGHT], mapVar[VAR_MAP_WIDTH], mapVar[VAR_MAP_HEIGHT], mapVar[VAR_MAP_OFFSETX], mapVar[VAR_MAP_OFFSETY], mapVar[VAR_MAP_FINE_OFFSETX], mapVar[VAR_MAP_FINE_OFFSETY], enableX, enableY, (float)mapVar[VAR_MAP_SCROLL_SPEED], mapVar[TILE_SIZE]);
				tempMap.LoadMap(directory);
				tempMap.setMapType(tempType);

				layout[mapVar[VAR_ID]]->roomLayout.push_back(tempMap);
			}
		}
	}

	for (unsigned i = 0; i < layout.size(); ++i)
	{
		layout[i]->rearrange();
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
					else if (attriName == "LEVEL")
					{
						day.setCurrentLevel(stoi(attriValue));
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
void SceneGame::InitScore(string config)
{
	Branch ScoreBranch = TextTree::FileToRead(config);
	// Instructions for the game
	ScoreDirectory =config;
	for (vector<Branch>::iterator branch = ScoreBranch.childBranches.begin(); branch != ScoreBranch.childBranches.end(); ++branch)
	{
		Score TempScore;
		for (vector<Attribute>::iterator attri = branch->attributes.begin(); attri != branch->attributes.end(); ++attri)
		{
			Attribute tempAttri = *attri;
			string attriName = tempAttri.name;
			string attriValue = tempAttri.value;

			TempScore.setScore(stoi(attriValue));
			score.push_back(TempScore);
		}
	}
}

void SceneGame::InitObjective(string config)
{
	Branch VariablesBranch = TextTree::FileToRead(config);
	// Objectives of the game

	for (vector<Branch>::iterator branch = VariablesBranch.childBranches.begin(); branch != VariablesBranch.childBranches.end(); ++branch)
	{
		if (branch->branchName == "Level")
		{
			for (vector<Branch>::iterator childbranch = branch->childBranches.begin(); childbranch != branch->childBranches.end(); ++childbranch)
			{
				for (vector<Attribute>::iterator attri = childbranch->attributes.begin(); attri != childbranch->attributes.end(); ++attri)
				{
					Attribute tempAttri = *attri;
					string attriName = tempAttri.name;
					string attriValue = tempAttri.value;
					if (attriName == "LEVEL")
					{
						day.levels.resize(stoi(attriValue));
					}
				}
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
					day.levels[level].objectives.push_back(tempobjective);
				}
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
	// Init AIs pos, mesh, maplocation & waypoint etc...

	for (vector<Branch>::iterator branch = AIBranch.childBranches.begin(); branch != AIBranch.childBranches.end(); ++branch)
	{
		for (vector<Branch>::iterator childbranch = branch->childBranches.begin(); childbranch != branch->childBranches.end(); ++childbranch)
		{
			if (branch->branchName == "Guard")
			{
				Vector2 pos;
				Vector2 dir;
				string spriteName;
				string waypoint;
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

					else if (attriName == "Waypoint")
					{
						waypoint = attriValue;
					}
				}

				Guards* guard = new Guards;
				guard->Init(pos * TILESIZE, dir, dynamic_cast<SpriteAnimation*>(findMesh(spriteName)), tiles, layout[mapLocation], waypoint);
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
				string waypoint;
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

					else if (attriName == "Waypoint")
					{
						waypoint = attriValue;
					}
				}

				Prisoners* prisoner = new Prisoners;
				prisoner->Init(pos * TILESIZE, dir, dynamic_cast<SpriteAnimation*>(findMesh(spriteName)), tiles, layout[mapLocation], waypoint);
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
	// Init player's pos, mesh, maplocation & waypoint etc...

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

		BaseFOV = tiles;
		player->Init(pos * TILESIZE, dir, dynamic_cast<SpriteAnimation*>(findMesh(spriteName)), tiles, layout[mapLocation]);
		player->setSize(Vector2((float)TILESIZE, (float)TILESIZE));
		player->setState(StateMachine::IDLE_STATE);
		player->setRoom(layout[mapLocation]);
	}
}

void SceneGame::InitItem(string config)
{
	Branch playerBranch = TextTree::FileToRead(config);
	// Init items pos & location in the game, etc...

	for (vector<Branch>::iterator branch = playerBranch.childBranches.begin(); branch != playerBranch.childBranches.end(); ++branch)
	{
		int ID = 0, locationID = 0;
		string name, description, meshName;
		Vector2 pos;
		CItem::ITEM_TYPE type;
		CItem::ITEM_STATUS status;

		for (vector<Attribute>::iterator attri = branch->attributes.begin(); attri != branch->attributes.end(); ++attri)
		{
			Attribute tempAttri = *attri;
			string attriName = tempAttri.name;
			string attriValue = tempAttri.value;

			name = branch->branchName;

			if (attriName == "ItemStatus")
			{
				if (attriValue == "Ground")
					status = CItem::ITEM_ONGROUND;
				else if (attriValue == "Inventory")
					status = CItem::ITEM_ININVENTORY;
				else if (attriValue == "Inactive")
					status = CItem::ITEM_INACTIVE;
			}

			else if (attriName == "ItemType")
			{
				if (attriValue == "Weapon")
					type = CItem::ITEM_WEAPON;
				else if (attriValue == "Clothing")
					type = CItem::ITEM_CLOTHNG;
				else if (attriValue == "Misc")
					type = CItem::ITEM_MISC;
			}

			else if (attriName == "Pos")
			{
				stringToVector(attriValue, pos);
				pos *= TILESIZE;
			}
			else if (attriName == "ID")
			{
				ID = stoi(attriValue);
			}
			else if (attriName == "Name")
			{
				name = attriValue;
			}
			else if (attriName == "Description")
			{
				description = attriValue;
			}
			else if (attriName == "Location")
			{
				locationID = stoi(attriValue);
			}
			else if (attriName == "Mesh")
			{
				meshName = attriValue;
			}
		}

		CItem* item = new CItem();

		item->Init(status, type, pos, ID, name, description, findMesh(meshName), locationID);
		itemList.push_back(item);
	}
	updateMousePos = true;
	indexItem1 = 0;
	indexItem2 = 0;
	renderInventory = true;
}

void SceneGame::InitInteractions(string config)
{
	Branch InteractionsBranch = TextTree::FileToRead(config);
	// Init all possible dialogs

	for (vector<Branch>::iterator branch = InteractionsBranch.childBranches.begin(); branch != InteractionsBranch.childBranches.end(); ++branch)
	{
		for (vector<Branch>::iterator childbranch = branch->childBranches.begin(); childbranch != branch->childBranches.end(); ++childbranch)
		{
			if (branch->branchName == "WeaDialog")
			{
				Dialogs tempDialogs;
				int tempID;
				string tempText;
				for (vector<Attribute>::iterator attri = childbranch->attributes.begin(); attri != childbranch->attributes.end(); ++attri)
				{
					Attribute tempAttri = *attri;
					string attriName = tempAttri.name;
					string attriValue = tempAttri.value;

					if (attriName == "ID")
					{
						tempID = stoi(attriValue);
					}
					else if(attriName == "Text")
					{
						tempText = attriValue;
					}


				}
				tempDialogs.InitDialogs(tempID,tempText);
				dialogs.push_back(tempDialogs);
			}
			else if (branch->branchName == "SelfDialog")
			{
				Dialogs tempDialogs;
				int tempID;
				string tempText;
				for (vector<Attribute>::iterator attri = childbranch->attributes.begin(); attri != childbranch->attributes.end(); ++attri)
				{
					Attribute tempAttri = *attri;
					string attriName = tempAttri.name;
					string attriValue = tempAttri.value;

					if (attriName == "ID")
					{
						tempID = stoi(attriValue);
					}
					else if(attriName == "Text")
					{
						tempText = attriValue;
					}
				}
				tempDialogs.InitDialogs(tempID,tempText);
				dialogs.push_back(tempDialogs);
			}
			else if (branch->branchName == "Setting")
			{
				Dialogs tempDialogs;
				string tempMesh;
				for (vector<Attribute>::iterator attri = childbranch->attributes.begin(); attri != childbranch->attributes.end(); ++attri)
				{
					Attribute tempAttri = *attri;
					string attriName = tempAttri.name;
					string attriValue = tempAttri.value;
					if (attriName == "Mesh")
					{
						tempMesh = attriValue;
					}
				}
				dialog.InitSetting(tempMesh);
			}
		}
	}
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

	if(KEngine::getKeyboard()->getKey('9'))
		FOG= true;

	if(KEngine::getKeyboard()->getKey('0'))
		FOG=false;
}

void SceneGame::UpdateInput(void)
{
	// updating keyboard intput via a handler from the engine
	for (unsigned i = 0; i < interactionKeys.size(); ++i)
	{
		interactionKeys[i].pressed = KEngine::getKeyboard()->getKey(interactionKeys[i].value);
	}
}

void SceneGame::UpdateMouse(void)
{
	// updating mouse pos in the scene via a handler from the engine
	double x, y;
	x = KEngine::getMouse()->getCurrentPosX();
	y = KEngine::getMouse()->getCurrentPosY();

	int w = KEngine::getWindowWidth();
	int h = KEngine::getWindowHeight();

	mousePos.Set((float)x * sceneWidth / w, (h - (float)y) * sceneHeight / h);
}

void SceneGame::UpdateState(void)
{
	// changing the scene game state
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
	// Provide player with feedback
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

void SceneGame::UpdatePlayerInventory(bool mousePressed, bool keyboardPressed, double mouseX, double mouseY, double dt)
{
	if (findItem("AccessCard"))
	{
		for (int numRooms = 0; numRooms < MAX_AREAS; ++numRooms)
		{
			for (unsigned numMaps = 0; numMaps < layout[numRooms]->roomLayout.size(); ++numMaps)
			{
				int m = 0;
				int n = 0;

				if (numMaps == TileMap::TYPE_VISUAL || numMaps == TileMap::TYPE_COLLISION)
				{
					for(int i = 0; i < layout[numRooms]->roomLayout[numMaps].getNumTilesHeight() + 1; i++)
					{
						n = -(layout[numRooms]->roomLayout[numMaps].getTileOffsetY()) + i;

						for(int k = 0; k < layout[numRooms]->roomLayout[numMaps].getNumTilesWidth() + 1; k++)
						{
							m = layout[numRooms]->roomLayout[numMaps].getTileOffsetX() + k;

							if (m >= layout[numRooms]->roomLayout[numMaps].getNumTilesMapWidth() || m < 0)
								break;
							if (n >= layout[numRooms]->roomLayout[numMaps].getNumTilesMapHeight() || n < 0)
								break;

							TileSheet *tilesheet = dynamic_cast<TileSheet*>(findMesh("GEO_TILESHEET"));
							tilesheet->m_currentTile = layout[numRooms]->roomLayout[numMaps].screenMap[n][m];

							for (unsigned special = 0; special < layout[numRooms]->specialTiles.size(); ++special)
							{
								if (layout[numRooms]->specialTiles[special].TileName == "PrisonDoorLeftClosed" || layout[numRooms]->specialTiles[special].TileName == "PrisonDoorRightClosed")
								{
									if(tilesheet->m_currentTile == layout[numRooms]->specialTiles[special].TileID)
									{
										layout[numRooms]->roomLayout[numMaps].screenMap[n][m] -= 2;
									}
								}
							}
						}
					}
				}
			}
		}
	}

	else
	{
		for (int numRooms = 0; numRooms < MAX_AREAS; ++numRooms)
		{
			for (unsigned numMaps = 0; numMaps < layout[numRooms]->roomLayout.size(); ++numMaps)
			{
				int m = 0;
				int n = 0;

				if (numMaps == TileMap::TYPE_VISUAL || numMaps == TileMap::TYPE_COLLISION)
				{
					for(int i = 0; i < layout[numRooms]->roomLayout[numMaps].getNumTilesHeight() + 1; i++)
					{
						n = -(layout[numRooms]->roomLayout[numMaps].getTileOffsetY()) + i;

						for(int k = 0; k < layout[numRooms]->roomLayout[numMaps].getNumTilesWidth() + 1; k++)
						{
							m = layout[numRooms]->roomLayout[numMaps].getTileOffsetX() + k;

							if (m >= layout[numRooms]->roomLayout[numMaps].getNumTilesMapWidth() || m < 0)
								break;
							if (n >= layout[numRooms]->roomLayout[numMaps].getNumTilesMapHeight() || n < 0)
								break;

							TileSheet *tilesheet = dynamic_cast<TileSheet*>(findMesh("GEO_TILESHEET"));
							tilesheet->m_currentTile = layout[numRooms]->roomLayout[numMaps].screenMap[n][m];

							for (unsigned special = 0; special < layout[numRooms]->specialTiles.size(); ++special)
							{
								if (layout[numRooms]->specialTiles[special].TileName == "PrisonDoorLeftOpened" || layout[numRooms]->specialTiles[special].TileName == "PrisonDoorRightOpened")
								{
									if(tilesheet->m_currentTile == layout[numRooms]->specialTiles[special].TileID)
									{
										layout[numRooms]->roomLayout[numMaps].screenMap[n][m] += 2;
									}
								}
							}
						}
					}
				}
			}
		}
	}

	// picking, dropping & switching of items
	for(vector<CItem*>::iterator it = itemList.begin(); it != itemList.end(); ++it)
	{
		CItem *item = (CItem *)*it;
		//Picking up of item
		if ((player->getPos() - item->getItemPos()).Length() <= TILESIZE && layout[currentLocation]->ID == item->getLocationID())
		{
			if (mouseX <= item->getItemPos().x + TILESIZE - layout[currentLocation]->roomLayout[TileMap::TYPE_VISUAL].getMapOffsetX()
				&& mouseX >= item->getItemPos().x - layout[currentLocation]->roomLayout[TileMap::TYPE_VISUAL].getMapOffsetX()
				&& mouseY <= item->getItemPos().y + TILESIZE - layout[currentLocation]->roomLayout[TileMap::TYPE_VISUAL].getMapOffsetY()
				&& mouseY >= item->getItemPos().y - layout[currentLocation]->roomLayout[TileMap::TYPE_VISUAL].getMapOffsetY())
			{
				if (mousePressed && item->getItemStatus() == CItem::ITEM_ONGROUND)
				{
					if(item->getItemID() == 1)
					{
						UpdateDialog(DUMBBELL);
					}
					else if(item->getItemID() == 2)
					{
						UpdateDialog(WATERGUN);
					}
					else if(item->getItemID() == 3)
					{
						UpdateDialog(GUARD_UNIFORM);
					}
					else if (item->getItemID() == 4)
					{
						UpdateDialog(FORK);
					}
					else if(item->getItemID() == 5)
					{
						UpdateDialog(MATCHES);
					}
					else if(item->getItemID() == 6)
					{
						UpdateDialog(ACCESS_CARD);
					}
					else if(item->getItemID() == 7)
					{
						UpdateDialog(TORCHLIGHT);
					}

					CInventory tempInventory = player->getInventory();
					tempInventory.addItem(item);
					player->setInventory(tempInventory);
					sound.Play("Sound_PickUp");
					break;
				}
			}

			else if (keyboardPressed)
			{
				if (item->getItemStatus() == CItem::ITEM_ONGROUND)
				{
					
					if(item->getItemID() == 1)
					{
						UpdateDialog(DUMBBELL);
					}
					else if(item->getItemID() == 2)
					{
						UpdateDialog(WATERGUN);
					}
					else if(item->getItemID() == 3)
					{
						UpdateDialog(GUARD_UNIFORM);
					}
					else if (item->getItemID() == 4)
					{
						UpdateDialog(FORK);
					}
					else if(item->getItemID() == 5)
					{
						UpdateDialog(MATCHES);
					}
					else if(item->getItemID() == 6)
					{
						UpdateDialog(ACCESS_CARD);
					}
					else if(item->getItemID() == 7)
					{
						UpdateDialog(TORCHLIGHT);
					}

					CInventory tempInventory = player->getInventory();
					tempInventory.addItem(item);
					player->setInventory(tempInventory);
					sound.Play("Sound_PickUp");
					break;
				}
			}
		}
	}

	bool dropItem = true;
	//get mouse position when player clicks
	if (mousePressed && updateMousePos == true)
	{
		//check through all the buttons in the game state
		for (unsigned i = 0; i < gameInterfaces[currentState].buttons.size(); ++i)
		{
			//check only against image buttons
			if (gameInterfaces[currentState].buttons[i].getType() == Buttons::IMAGE_BUTTON)
			{
				//Find location of mouse press
				//If mouseX & mouseY is within a box, store pos
				if (mouseX < gameInterfaces[currentState].buttons[i].getPos().x + gameInterfaces[currentState].buttons[i].getScale().x
					&& mouseX > gameInterfaces[currentState].buttons[i].getPos().x
					&& mouseY < gameInterfaces[currentState].buttons[i].getPos().y + gameInterfaces[currentState].buttons[i].getScale().y
					&& mouseY > gameInterfaces[currentState].buttons[i].getPos().y)
				{
					//store pos
					if (i < player->getInventory().getVecOfItems().size() + 1)
					{
						indexItem1 = stoi(gameInterfaces[currentState].buttons[i].getName());
						tempMouseX = mouseX;
						tempMouseY = mouseY;
						//breaking line
						updateMousePos = false;
						break;
					}
				}
			}
		}
	}
	//get mouse position when player releases the click
	else if (!mousePressed && updateMousePos == false)
	{
		for (unsigned i = 0; i < gameInterfaces[currentState].buttons.size(); ++i)
		{
			//Find location of mouse press
			//If mouseX and mouse Y within box range, swap item
			if (gameInterfaces[currentState].buttons[i].getType() == Buttons::IMAGE_BUTTON)
			{
				if (mouseX < gameInterfaces[currentState].buttons[i].getPos().x + gameInterfaces[currentState].buttons[i].getScale().x
					&& mouseX > gameInterfaces[currentState].buttons[i].getPos().x
					&& mouseY < gameInterfaces[currentState].buttons[i].getPos().y + gameInterfaces[currentState].buttons[i].getScale().y
					&& mouseY > gameInterfaces[currentState].buttons[i].getPos().y)
				{
					{
						//Swap item
						if (i < player->getInventory().getVecOfItems().size() + 1)
						{
							indexItem2 = stoi(gameInterfaces[currentState].buttons[i].getName());
							player->getInventory().swapItem(indexItem1,indexItem2);
							updateMousePos = true;
							dropItem = false;
								sound.Play("Sound_PickUp");
							break;

						}
					}
				}
			}
		}

		if (dropItem == true)
		{
			for (unsigned i = 0; i < gameInterfaces[currentState].buttons.size(); ++i)
			{
				//Find location of mouse press
				//If mouseX and mouse Y NOT within box range, drop item
				if (gameInterfaces[currentState].buttons[i].getType() == Buttons::IMAGE_BUTTON)
				{
					if ((mouseX > gameInterfaces[currentState].buttons[i].getPos().x + gameInterfaces[currentState].buttons[i].getScale().x
						|| mouseX < gameInterfaces[currentState].buttons[i].getPos().x
						|| mouseY > gameInterfaces[currentState].buttons[i].getPos().y + gameInterfaces[currentState].buttons[i].getScale().y
						|| mouseY < gameInterfaces[currentState].buttons[i].getPos().y)
						&& stof(gameInterfaces[currentState].buttons[i].getName()) < player->getInventory().getVecOfItems().size())
					{
						{
							if (tempMouseX < gameInterfaces[currentState].buttons[i].getPos().x + gameInterfaces[currentState].buttons[i].getScale().x
								&& tempMouseX > gameInterfaces[currentState].buttons[i].getPos().x
								&& tempMouseY < gameInterfaces[currentState].buttons[i].getPos().y + gameInterfaces[currentState].buttons[i].getScale().y
								&& tempMouseY > gameInterfaces[currentState].buttons[i].getPos().y)
							{
								//remove item from the box obtained at tempX n tempY
								player->getInventory().getVecOfItems().at(stoi(gameInterfaces[currentState].buttons[i].getName()))->setItemStatus(CItem::ITEM_ONGROUND);
								if (player->getInventory().getVecOfItems().at(stoi(gameInterfaces[currentState].buttons[i].getName()))->getLocationID() == layout[currentLocation]->roomLayout[TileMap::TYPE_VISUAL].getID())
								{
									player->getInventory().getVecOfItems().at(stoi(gameInterfaces[currentState].buttons[i].getName()))->setItemPos(Vector2((float)(player->getPos().x), (float)(player->getPos().y)));
								}
								else
								{
									player->getInventory().getVecOfItems().at(stoi(gameInterfaces[currentState].buttons[i].getName()))->setLocationID(layout[currentLocation]->roomLayout[TileMap::TYPE_VISUAL].getID());
									player->getInventory().getVecOfItems().at(stoi(gameInterfaces[currentState].buttons[i].getName()))->setItemPos(Vector2((float)(player->getPos().x), (float)(player->getPos().y)));
								}
								player->getInventory().removeItem(stoi(gameInterfaces[currentState].buttons[i].getName()));
								updateMousePos = true;
									sound.Play("Sound_PickUp");
								break;
							}
						}
					}
				}
			}
		}
	}
	if (getKey("ToggleInv"))
	{
		renderInventory = !renderInventory;
	}
	float slidingSpeed = 200.f;
	for (unsigned i = 0; i < gameInterfaces[currentState].buttons.size(); ++i)
	{
		if (gameInterfaces[currentState].buttons[i].getType() == Buttons::IMAGE_BUTTON)
		{
			if (renderInventory == true)
			{
				if (translateInventoryY < gameInterfaces[currentState].buttons[i].getPos().y)
				{
					translateInventoryY += (float)(slidingSpeed * dt);
				}
				else
				{
					translateInventoryY = (float)gameInterfaces[currentState].buttons[i].getPos().y;
				}
				break;
			}
			else 
			{
				if (translateInventoryY > -gameInterfaces[currentState].buttons[i].getScale().y)
				{
					translateInventoryY -= (float)(slidingSpeed * dt);
				}
				else 
				{
					translateInventoryY = (float)-gameInterfaces[currentState].buttons[i].getScale().y;
				}
				break;
			}
		}
	}
}

void SceneGame::UpdateInGame(double dt)
{
	UpdatePlayer(dt);
	UpdateAI(dt);
	UpdatePlayerInventory(getKey("Select"), getKey("Enter"), mousePos.x, mousePos.y, dt);
	UpdateInteractions(dt);
	day.UpdateDay(dt,gameSpeed);
	UpdateFOV();
	UpdateObjective();
}

void SceneGame::UpdateFOV(void)
{
	if(day.getCurrentTime().hour >= 18 || day.getCurrentTime().hour >= 0 && day.getCurrentTime().hour <6)	//Night
	{		
		if (player->GetFOV() >= BaseFOV)
		{
			if(day.getCurrentTime().hour !=0)
			{
				player->SetFOV(player->GetFOV() - day.getCurrentTime().hour/day.getCurrentTime().hour);	
			}
			else
			{
				player->SetFOV(player->GetFOV() - 1);	
			}
		}
	}
	else if (day.getCurrentTime().hour >= 6 && day.getCurrentTime().hour <18) //Day
	{
		if (player->GetFOV() <= 72)
		{
			if(day.getCurrentTime().hour !=0)
			{
				player->SetFOV(player->GetFOV() + day.getCurrentTime().hour/day.getCurrentTime().hour);	
			}
			else
			{
				player->SetFOV(player->GetFOV() - 1);	
			}
		}
	}
}

void SceneGame::UpdatePlayer(double dt)
{
	if (player->getState() == StateMachine::IDLE_STATE)
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

	// convert player from world pos to screen pos
	static Vector2 playerPosToScreen;
	playerPosToScreen.Set((player->getPos().x)/TILESIZE, (sceneHeight - player->getPos().y - TILESIZE)/TILESIZE);

	// Update for interaction
	for (unsigned special = 0; special < layout[currentLocation]->specialTiles.size(); ++special)
	{
		// threadmill
		if (layout[currentLocation]->specialTiles[special].TileName == "Threadmill")
		{
			if(layout[currentLocation]->roomLayout[TileMap::TYPE_VISUAL].screenMap[(int)playerPosToScreen.y][(int)playerPosToScreen.x] == layout[currentLocation]->specialTiles[special].TileID)
			{
				currentInteraction = RUNNING_ON_THREADMILL;
			}
		}
		// bed - fast forward time
		else if (layout[currentLocation]->specialTiles[special].TileName == "Bed")
		{
			if(layout[currentLocation]->roomLayout[TileMap::TYPE_COLLISION].screenMap[(int)playerPosToScreen.y][(int)playerPosToScreen.x] == layout[currentLocation]->specialTiles[special].TileID)
			{
				if(getKey("Enter"))
				{
					currentInteraction = SLEEP;
				}
			}
		}

		// open a closed cell
		else if (layout[currentLocation]->specialTiles[special].TileName == "CellDoorClosed")
		{
			// the door is below the player
			if(player->getDir().y == -1)
			{
				if(layout[currentLocation]->roomLayout[TileMap::TYPE_COLLISION].screenMap[(int)playerPosToScreen.y + 1][(int)playerPosToScreen.x] == layout[currentLocation]->specialTiles[special].TileID)
				{
					if((getKey("OpenDoor") || getKey("Select")) && findItem("Fork"))
					{
						sound.Play("Sound_DoorOpen");

						for (unsigned openDoor = 0; openDoor < layout[currentLocation]->specialTiles.size(); ++openDoor)
						{
							if (layout[currentLocation]->specialTiles[openDoor].TileName == "CellDoorOpened")
							{
								layout[currentLocation]->roomLayout[TileMap::TYPE_VISUAL].screenMap[(int)playerPosToScreen.y + 1][(int)playerPosToScreen.x] = layout[currentLocation]->specialTiles[openDoor].TileID;
								layout[currentLocation]->roomLayout[TileMap::TYPE_COLLISION].screenMap[(int)playerPosToScreen.y + 1][(int)playerPosToScreen.x] = layout[currentLocation]->specialTiles[openDoor].TileID;
								break;
							}
						}
					}
				}
			}

			// the door is above the player
			else if(player->getDir().y == 1)
			{
				if(layout[currentLocation]->roomLayout[TileMap::TYPE_COLLISION].screenMap[(int)playerPosToScreen.y - 1][(int)playerPosToScreen.x] == layout[currentLocation]->specialTiles[special].TileID)
				{
					if((getKey("OpenDoor") || getKey("Select")) && findItem("Fork"))
					{
							sound.Play("Sound_DoorOpen");
						for (unsigned openDoor = 0; openDoor < layout[currentLocation]->specialTiles.size(); ++openDoor)
						{
							if (layout[currentLocation]->specialTiles[openDoor].TileName == "CellDoorOpened")
							{
								layout[currentLocation]->roomLayout[TileMap::TYPE_VISUAL].screenMap[(int)playerPosToScreen.y - 1][(int)playerPosToScreen.x] = layout[currentLocation]->specialTiles[openDoor].TileID;
								layout[currentLocation]->roomLayout[TileMap::TYPE_COLLISION].screenMap[(int)playerPosToScreen.y - 1][(int)playerPosToScreen.x] = layout[currentLocation]->specialTiles[openDoor].TileID;
								break;
							}
						}
					}
				}
			}
		}

		// close a opened cell
		else if (layout[currentLocation]->specialTiles[special].TileName == "CellDoorOpened")
		{
			// door is below the player
			if(player->getDir().y == -1)
			{
				if( layout[currentLocation]->roomLayout[TileMap::TYPE_COLLISION].screenMap[(int)playerPosToScreen.y + 1][(int)playerPosToScreen.x] == layout[currentLocation]->specialTiles[special].TileID)
				{
					if(getKey("CloseDoor") || getKey("RSelect") && findItem("Fork"))
					{
						sound.Play("Sound_DoorClose");
						for (unsigned openDoor = 0; openDoor < layout[currentLocation]->specialTiles.size(); ++openDoor)
						{
							if (layout[currentLocation]->specialTiles[openDoor].TileName == "CellDoorClosed")
							{
								layout[currentLocation]->roomLayout[TileMap::TYPE_VISUAL].screenMap[(int)playerPosToScreen.y + 1][(int)playerPosToScreen.x] = layout[currentLocation]->specialTiles[openDoor].TileID;
								layout[currentLocation]->roomLayout[TileMap::TYPE_COLLISION].screenMap[(int)playerPosToScreen.y + 1][(int)playerPosToScreen.x] = layout[currentLocation]->specialTiles[openDoor].TileID;
								break;
							}
						}
					}
				}
			}
			// door is above the player
			else if(player->getDir().y == 1)
			{
				if( layout[currentLocation]->roomLayout[TileMap::TYPE_COLLISION].screenMap[(int)playerPosToScreen.y - 1][(int)playerPosToScreen.x] == layout[currentLocation]->specialTiles[special].TileID)
				{
					if(getKey("CloseDoor") || getKey("RSelect") && findItem("Fork"))
					{
							sound.Play("Sound_DoorClose");
						for (unsigned openDoor = 0; openDoor < layout[currentLocation]->specialTiles.size(); ++openDoor)
						{
							if (layout[currentLocation]->specialTiles[openDoor].TileName == "CellDoorClosed")
							{
								layout[currentLocation]->roomLayout[TileMap::TYPE_VISUAL].screenMap[(int)playerPosToScreen.y - 1][(int)playerPosToScreen.x] = layout[currentLocation]->specialTiles[openDoor].TileID;
								layout[currentLocation]->roomLayout[TileMap::TYPE_COLLISION].screenMap[(int)playerPosToScreen.y - 1][(int)playerPosToScreen.x] = layout[currentLocation]->specialTiles[openDoor].TileID;
								break;
							}
						}
					}
				}
			}
		}

		else if (layout[currentLocation]->specialTiles[special].TileName == "PrisonDoorLeftOpened" || layout[currentLocation]->specialTiles[special].TileName == "PrisonDoorRightOpened")
		{
			if(layout[currentLocation]->roomLayout[TileMap::TYPE_VISUAL].screenMap[(int)playerPosToScreen.y][(int)playerPosToScreen.x] == layout[currentLocation]->specialTiles[special].TileID)
			{
				for (unsigned numDoors = 0; numDoors < layout[currentLocation]->doors.size(); ++numDoors)
				{
					// switch room
					if (playerPosToScreen == layout[currentLocation]->doors[numDoors].pos)
					{
						// find the door on that map
						int nextRoom = layout[currentLocation]->doors[numDoors].transitionRoom;
						Vector2 nextRoomDoorPos;

						for (unsigned nextRoomDoors = 0; nextRoomDoors < layout[nextRoom]->doors.size(); ++nextRoomDoors)
						{
							if (layout[nextRoom]->doors[nextRoomDoors].transitionRoom == currentLocation && layout[currentLocation]->doors[numDoors].ID == layout[nextRoom]->doors[nextRoomDoors].ID)
							{
								nextRoomDoorPos = layout[nextRoom]->doors[nextRoomDoors].pos;
								break;
							}
						}

						player->setRoom(layout[nextRoom]);
						// offset the current pos by 1 tile
						if (player->getDir().x == 1)
						{
							player->setPos(Vector2((nextRoomDoorPos.x + 1)* layout[nextRoom]->roomLayout[TileMap::TYPE_VISUAL].getTileSize(), layout[nextRoom]->roomLayout[TileMap::TYPE_VISUAL].getScreenHeight() - (nextRoomDoorPos.y + 1) * layout[nextRoom]->roomLayout[TileMap::TYPE_VISUAL].getTileSize()));
						}

						else if (player->getDir().x == -1)
						{
							player->setPos(Vector2((nextRoomDoorPos.x - 1)* layout[nextRoom]->roomLayout[TileMap::TYPE_VISUAL].getTileSize(), layout[nextRoom]->roomLayout[TileMap::TYPE_VISUAL].getScreenHeight() - (nextRoomDoorPos.y + 1) * layout[nextRoom]->roomLayout[TileMap::TYPE_VISUAL].getTileSize()));
						}

						else if (player->getDir().y == 1)
						{
							player->setPos(Vector2(nextRoomDoorPos.x * layout[nextRoom]->roomLayout[TileMap::TYPE_VISUAL].getTileSize(), layout[nextRoom]->roomLayout[TileMap::TYPE_VISUAL].getScreenHeight() - (nextRoomDoorPos.y) * layout[nextRoom]->roomLayout[TileMap::TYPE_VISUAL].getTileSize()));
						}

						else if (player->getDir().y == -1)
						{
							player->setPos(Vector2(nextRoomDoorPos.x * layout[nextRoom]->roomLayout[TileMap::TYPE_VISUAL].getTileSize(), layout[nextRoom]->roomLayout[TileMap::TYPE_VISUAL].getScreenHeight() - (nextRoomDoorPos.y + 2) * layout[nextRoom]->roomLayout[TileMap::TYPE_VISUAL].getTileSize()));
						}

						player->setTargetPos(player->getPos());

						for (vector<Guards*>::iterator guard = guardList.begin(); guard != guardList.end(); ++guard)
						{
							Guards* tempGuard = *guard;
							if (tempGuard->getRoom() == layout[currentLocation])
							{
								tempGuard->ResetPos();
							}
						}

						currentLocation = nextRoom;
						break;
					}
				}

				break;
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

		if (tempPrisoner->getRoom()->ID == player->getRoom()->ID)
		{
			tempPrisoner->setRender(true);
		}

		else
		{
			tempPrisoner->setRender(false);
		}
		//tempPrisoner->PathFinding((int)sceneWidth, (int)sceneHeight, TILESIZE, dt);
		tempPrisoner->tileBasedMovement((int)sceneWidth, (int)sceneHeight, TILESIZE, dt);
		tempPrisoner->Update(dt);
	}

	for (vector<Guards*>::iterator guard = guardList.begin(); guard != guardList.end(); ++guard)
	{
		Guards* tempGuard = *guard;

		if (tempGuard->getRoom()->ID == player->getRoom()->ID)
		{
			tempGuard->setRender(true);
			tempGuard->SetUpdate(true);
		}

		else
		{
			tempGuard->setRender(false);
			tempGuard->SetUpdate(false);
		}
	}

	for (vector<Guards*>::iterator guard = guardList.begin(); guard != guardList.end(); ++guard)
	{
		static bool played = false;
		Guards* tempGuard = *guard;

		bool firstChase = tempGuard->getChase();

		if (tempGuard->GetUpdate())
		{
			tempGuard->CheckChase(player->getTargetPos(), TILESIZE, dt);	
			tempGuard->Update((int)sceneWidth, (int)sceneHeight, TILESIZE, dt);

			if (tempGuard->getChase() != firstChase && tempGuard->getChase())
			{
				sound.Play("Sound_Alert");
			}
	
			// If the player is caught by the guard
			if ((tempGuard->getPos() - player->getPos()).Length() < TILESIZE * 0.2f)
			{
				sound.Play("Sound_Caught");
				player->ResetPos();
				player->setRoom(layout[CELL_AREA]);
				currentLocation = CELL_AREA;
			}
		}
	}
}

void SceneGame::UpdateMap(void)
{
	for (unsigned i = 0; i < layout[currentLocation]->roomLayout.size(); ++i)
	{
		this->layout[currentLocation]->roomLayout[i].Update();
	}
}

void SceneGame::UpdateInteractions(double dt)
{
	switch (currentInteraction)
	{
	case NO_INTERACTION:
		gameSpeed = 10;
		UpdateDialog(NEED_TO_ESCAPE);
		break;
	case SLEEP:
		gameSpeed = 75;
		UpdateDialog(IM_TIRED);
		break;
	case RUNNING_ON_THREADMILL:
		UpdateThreadmill();
		break;
	default:;
		break;
	}

	//std::cout << currentInteraction << std::endl;
}

void SceneGame::UpdateThreadmill(void)
{
	player->setDir(Vector2(-1, 0));
	player->setAni(Character::RUN_LEFT);
	if(getKey("Right"))
	{
		player->setDir(Vector2(1,0));
	}
	else if(getKey("Up"))
	{
		player->setDir(Vector2(0,1));
	}
	else if(getKey("Down"))
	{
		player->setDir(Vector2(0,-1));
	}
}

void SceneGame::UpdateDialog(Dialog_ID diaName)
{
	//startTimer += (float) dt * dialog.GetTextSpeed();
	//clearTimer += (float) dt * dialog.GetTextSpeed();

	//std::cout << diaName << std::endl;
	//std::cout << currentDialogue << std::endl;
	
	unsigned currentSize = dialogString.length();
	if(currentSize < findDialog(diaName).GetText().size())
	{
		dialogString = findDialog(diaName).GetText();
		sound.Play("Sound_Beep");
	}

	/*if(clearTimer >= dialog.GetTextSpeed())
	{
		for (unsigned i = 0; i < dialogString.size(); i++)
		{
			dialogString[i]=NULL;
		}
		dialogString.resize(0);
	}*/
}

void SceneGame::UpdateObjective(void)
{
	for (vector<Level>::iterator level = day.levels.begin(); level != day.levels.end(); ++level)
	{
		for (vector<Objective>::iterator objective = level->objectives.begin(); objective !=  level->objectives.end(); objective++)
		{
			for (vector<CItem*>::iterator itemIt = itemList.begin(); itemIt != itemList.end(); ++itemIt)
			{
				CItem* item = *(itemIt);

				if (item->getItemName() == objective->getkeyItem())
				{
					objective->setState(objective->OBJECTIVE_COMPLETED);

					if (item->getItemStatus() == CItem::ITEM_ININVENTORY)
					{
						objective->setState(Objective::OBJECTIVE_COMPLETED);
					}
					else if (item->getItemStatus() == CItem::ITEM_ONGROUND)
					{
						objective->setState(Objective::OBJECTIVE_INPCOMPLETE);
					}
				}
			}
		}
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

void SceneGame::RenderInterface(bool toggle)
{
	for(unsigned i = 0; i < gameInterfaces[currentState].buttons.size(); ++i)
	{
		if (gameInterfaces[currentState].buttons[i].getType() == Buttons::TEXT_BUTTON)
		{
			RenderTextOnScreen(gameInterfaces[currentState].buttons[i].getMesh(), gameInterfaces[currentState].buttons[i].getText(), gameInterfaces[currentState].buttons[i].getColor(), defaultFontSize, gameInterfaces[currentState].buttons[i].getPos().x, gameInterfaces[currentState].buttons[i].getPos().y, gameInterfaces[currentState].buttons[i].getRotation());

		}

		else
		{
			//if (toggle == true)
			{
				Render2DMesh(gameInterfaces[currentState].buttons[i].getMesh(), false, gameInterfaces[currentState].buttons[i].getScale(), Vector2(gameInterfaces[currentState].buttons[i].getPos().x + gameInterfaces[currentState].buttons[i].getScale().x * 0.5f, gameInterfaces[currentState].buttons[i].getPos().y + gameInterfaces[currentState].buttons[i].getScale().y * 0.5f + translateInventoryY), gameInterfaces[currentState].buttons[i].getRotation());
			}
			//Render2DMesh(gameInterfaces[currentState].buttons[i].getMesh(), false, gameInterfaces[currentState].buttons[i].getScale(), gameInterfaces[currentState].buttons[i].getPos(), gameInterfaces[currentState].buttons[i].getRotation());
		}

		if (DEBUG)
		{
			Render2DMesh(findMesh("GEO_DEBUGQUAD"), false, gameInterfaces[currentState].buttons[i].getScale(), Vector2(gameInterfaces[currentState].buttons[i].getPos().x + gameInterfaces[currentState].buttons[i].getScale().x * 0.5f, gameInterfaces[currentState].buttons[i].getPos().y + gameInterfaces[currentState].buttons[i].getScale().y * 0.5f), gameInterfaces[currentState].buttons[i].getRotation());
		}
	}

	glDisable(GL_DEPTH_TEST);
}


void SceneGame::RenderScore(void)
{
	float y_Space = specialFontSize * 2;
	int place = 1;
	for (vector<Score>::iterator itr = score.begin(); itr != score.end(); ++itr)
	{
		y_Space += specialFontSize;
		std::ostringstream ss2;
		ss2.precision(1);
		ss2 <<place << ". "<< itr->getScore()<<" Days" <<endl;
		RenderTextOnScreen(findMesh("GEO_TEXT"), ss2.str(), findColor("White"), specialFontSize, sceneWidth*0.25f ,sceneHeight - specialFontSize - y_Space);
		place++;
	}
	y_Space = specialFontSize * 2;
	glDisable(GL_DEPTH_TEST);
}
void SceneGame::RenderObjectives(void)
{
	float y_Space = specialFontSize * 2;

	std::ostringstream ss;
	ss <<"Objectives"<<endl;

	RenderTextOnScreen(findMesh("GEO_TEXT_BACKGROUND"), ss.str(), findColor("Blue"), specialFontSize * 0.5f, 0, sceneHeight - specialFontSize - y_Space);

	for (vector<Level>::iterator level = day.levels.begin(); level != day.levels.end(); ++level)
	{
		for (vector<Objective>::iterator objective = level->objectives.begin(); objective !=  level->objectives.end(); objective++)
		{
			y_Space += specialFontSize;
			if(objective->getlevel() == day.getCurrentLevel())
			{
				if(objective->getObjectiveState() == objective->OBJECTIVE_INPCOMPLETE)
				{
					std::ostringstream ss;
					ss << objective->getTitle()<<endl;
					RenderTextOnScreen(findMesh("GEO_TEXT_BACKGROUND"), ss.str(), findColor("Blue"), specialFontSize * 0.5f, 0 ,sceneHeight - specialFontSize - y_Space);
				}
				else if(objective->getObjectiveState() == objective->OBJECTIVE_COMPLETED)
				{
					std::ostringstream ss;
					ss << objective->getTitle()<<endl;
					RenderTextOnScreen(findMesh("GEO_TEXT_BACKGROUND"), ss.str(), findColor("Green"), specialFontSize * 0.5f, 0 ,sceneHeight - specialFontSize - y_Space);
				}
			}
		}
		y_Space = specialFontSize * 2;
	}

	glDisable(GL_DEPTH_TEST);
}


void SceneGame::RenderCursor(void)
{
	Render2DMesh(findMesh("GEO_CURSOR"), false, (float) TILESIZE,mousePos.x,mousePos.y );

	glDisable(GL_DEPTH_TEST);
}

void SceneGame::RenderFOV(void)
{
	if ((unsigned)currentLocation < layout.size())
	{
		int m = 0;
		int n = 0;

		for(int i = 0; i < layout[currentLocation]->roomLayout[TileMap::TYPE_COLLISION].getNumTilesHeight() + 1; i++)
		{
			n = -(layout[currentLocation]->roomLayout[TileMap::TYPE_COLLISION].getTileOffsetY()) + i;

			for(int k = 0; k < layout[currentLocation]->roomLayout[TileMap::TYPE_COLLISION].getNumTilesWidth() + 1; k++)
			{						
				m = layout[currentLocation]->roomLayout[TileMap::TYPE_COLLISION].getTileOffsetX() + k;

				if (m >= layout[currentLocation]->roomLayout[TileMap::TYPE_COLLISION].getNumTilesMapWidth() || m < 0)
					break;
				if (n >= layout[currentLocation]->roomLayout[TileMap::TYPE_COLLISION].getNumTilesMapHeight() || n < 0)
					break;

				TileSheet *tilesheet = dynamic_cast<TileSheet*>(findMesh("GEO_TILESHEET"));
				tilesheet->m_currentTile = layout[currentLocation]->roomLayout[TileMap::TYPE_COLLISION].screenMap[n][m];

				//for (unsigned special = 0; special < layout[currentLocation]->specialTiles.size(); ++special)
				//{
				//	if(layout[currentLocation]->specialTiles[special].TileName == "Wall")
				//	{
				if(player->CalculateDistance(Vector2 (m * (float)TILESIZE, layout[currentLocation]->roomLayout[TileMap::TYPE_COLLISION].getScreenHeight() - (n + 1) * (float)TILESIZE), (float)TILESIZE) <= player->GetFOV()*0.5f)// && (tilesheet->m_currentTile != layout[currentLocation]->specialTiles[special].TileID))
				{
					Render2DMesh(findMesh("GEO_FOV_CLEAR"), false, (float)layout[currentLocation]->roomLayout[TileMap::TYPE_COLLISION].getTileSize() , (k + 0.5f) * layout[currentLocation]->roomLayout[TileMap::TYPE_COLLISION].getTileSize() - layout[currentLocation]->roomLayout[TileMap::TYPE_COLLISION].getMapFineOffsetX(), layout[currentLocation]->roomLayout[TileMap::TYPE_COLLISION].getScreenHeight() - (float)(i + 0.5f) * layout[currentLocation]->roomLayout[TileMap::TYPE_COLLISION].getTileSize() - layout[currentLocation]->roomLayout[TileMap::TYPE_COLLISION].getMapFineOffsetY());
				}

				else if(player->CalculateDistance(Vector2 (m * (float)TILESIZE, layout[currentLocation]->roomLayout[TileMap::TYPE_VISUAL].getScreenHeight() - (n + 1) * (float)TILESIZE), (float)TILESIZE) <= player->GetFOV())// && (tilesheet->m_currentTile != layout[currentLocation]->specialTiles[special].TileID))
				{
					Render2DMesh(findMesh("GEO_FOV_SEMI"), false, (float)layout[currentLocation]->roomLayout[TileMap::TYPE_COLLISION].getTileSize() , (k + 0.5f) * layout[currentLocation]->roomLayout[TileMap::TYPE_COLLISION].getTileSize() - layout[currentLocation]->roomLayout[TileMap::TYPE_COLLISION].getMapFineOffsetX(), layout[currentLocation]->roomLayout[TileMap::TYPE_COLLISION].getScreenHeight() - (float)(i + 0.5f) * layout[currentLocation]->roomLayout[TileMap::TYPE_COLLISION].getTileSize() - layout[currentLocation]->roomLayout[TileMap::TYPE_COLLISION].getMapFineOffsetY());
				}

				else
				{
					Render2DMesh(findMesh("GEO_FOV_SOLID"), false, (float)layout[currentLocation]->roomLayout[TileMap::TYPE_COLLISION].getTileSize() , (k + 0.5f) * layout[currentLocation]->roomLayout[TileMap::TYPE_COLLISION].getTileSize() - layout[currentLocation]->roomLayout[TileMap::TYPE_COLLISION].getMapFineOffsetX(), layout[currentLocation]->roomLayout[TileMap::TYPE_COLLISION].getScreenHeight() - (float)(i + 0.5f) * layout[currentLocation]->roomLayout[TileMap::TYPE_COLLISION].getTileSize() - layout[currentLocation]->roomLayout[TileMap::TYPE_COLLISION].getMapFineOffsetY());
				}	
						//	}
						//}				
			}
		}
	}
}

void SceneGame::RenderLevel(void)
{
	// check if player is within the game
	if ((unsigned)currentLocation < layout.size())
	{
		for (unsigned numMaps = 0; numMaps < layout[currentLocation]->roomLayout.size(); ++numMaps)
		{
			int m = 0;
			int n = 0;

			if (numMaps == TileMap::TYPE_BACKGROUND || numMaps == TileMap::TYPE_VISUAL)
			{
				for(int i = 0; i < layout[currentLocation]->roomLayout[numMaps].getNumTilesHeight() + 1; i++)
				{
					n = -(layout[currentLocation]->roomLayout[numMaps].getTileOffsetY()) + i;

					for(int k = 0; k < layout[currentLocation]->roomLayout[numMaps].getNumTilesWidth() + 1; k++)
					{
						m = layout[currentLocation]->roomLayout[numMaps].getTileOffsetX() + k;

						if (m >= layout[currentLocation]->roomLayout[numMaps].getNumTilesMapWidth() || m < 0)
							break;
						if (n >= layout[currentLocation]->roomLayout[numMaps].getNumTilesMapHeight() || n < 0)
							break;

						if (layout[currentLocation]->roomLayout[numMaps].getMapType() != TileMap::TYPE_COLLISION)
						{
							TileSheet *tilesheet = dynamic_cast<TileSheet*>(findMesh("GEO_TILESHEET"));
							tilesheet->m_currentTile = layout[currentLocation]->roomLayout[numMaps].screenMap[n][m];

							if (tilesheet->m_currentTile != -1)
							{
								// offset the world
								Render2DMesh(findMesh("GEO_TILESHEET"), false, (float)layout[currentLocation]->roomLayout[numMaps].getTileSize() + 3, (k + 0.5f) * layout[currentLocation]->roomLayout[numMaps].getTileSize() - layout[currentLocation]->roomLayout[numMaps].getMapFineOffsetX(), layout[currentLocation]->roomLayout[numMaps].getScreenHeight() - (float)(i + 0.5f) * layout[currentLocation]->roomLayout[numMaps].getTileSize() - layout[currentLocation]->roomLayout[numMaps].getMapFineOffsetY());
							}
						}

						else
						{
							if (DEBUG)
							{
								int collideTile = layout[currentLocation]->roomLayout[numMaps].screenMap[n][m];

								if (collideTile != -1 && layout[currentLocation]->roomLayout[numMaps].getMapType() == TileMap::TYPE_COLLISION)
								{
									Render2DMesh(findMesh("GEO_DEBUGQUAD"), false, (float)layout[currentLocation]->roomLayout[numMaps].getTileSize() + 3, (k + 0.5f) * layout[currentLocation]->roomLayout[numMaps].getTileSize() - layout[currentLocation]->roomLayout[numMaps].getMapFineOffsetX(), layout[currentLocation]->roomLayout[numMaps].getScreenHeight() - (float)(i + 0.5f) * layout[currentLocation]->roomLayout[numMaps].getTileSize() - layout[currentLocation]->roomLayout[numMaps].getMapFineOffsetY());
								}
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
	Render2DMesh(player->getSprite(), false, (float)TILESIZE * 1.5f, player->getPos().x + TILESIZE * 0.5f - layout[currentLocation]->roomLayout[TileMap::TYPE_VISUAL].getMapOffsetX(), player->getPos().y + TILESIZE * 0.5f - layout[currentLocation]->roomLayout[TileMap::TYPE_VISUAL].getMapOffsetY());

	/*std::cout << layout[currentLocation]->roomLayout[TileMap::TYPE_VISUAL].getMapOffsetX() << ", " << layout[currentLocation]->roomLayout[TileMap::TYPE_VISUAL].getMapOffsetY() << std::endl;
	std::cout << layout[currentLocation]->roomLayout[TileMap::TYPE_VISUAL].getMapFineOffsetX() << ", " << layout[currentLocation]->roomLayout[TileMap::TYPE_VISUAL].getMapFineOffsetY() << std::endl;*/

	if (DEBUG)
	{
		Render2DMesh(findMesh("GEO_DEBUGQUAD"), false, (float)TILESIZE, player->getPos().x + TILESIZE * 0.5f - layout[currentLocation]->roomLayout[TileMap::TYPE_VISUAL].getMapOffsetX(), player->getPos().y + TILESIZE * 0.5f - layout[currentLocation]->roomLayout[TileMap::TYPE_VISUAL].getMapOffsetY());
	}

	for (vector<Guards*>::iterator guard = guardList.begin(); guard != guardList.end(); ++guard)
	{
		Guards* tempGuard = *guard;

		if (tempGuard->getRender())
		{
			Render2DMesh(tempGuard->getSprite(), false, (float)TILESIZE * 1.5f, tempGuard->getPos().x + TILESIZE * 0.5f - layout[currentLocation]->roomLayout[TileMap::TYPE_VISUAL].getMapOffsetX(), tempGuard->getPos().y + TILESIZE * 0.5f - layout[currentLocation]->roomLayout[TileMap::TYPE_VISUAL].getMapOffsetY());

			if (DEBUG)
			{
				Render2DMesh(findMesh("GEO_DEBUGQUAD"), false, (float)TILESIZE, tempGuard->getPos().x + TILESIZE * 0.5f - layout[currentLocation]->roomLayout[TileMap::TYPE_VISUAL].getMapOffsetX(),  tempGuard->getPos().y + TILESIZE * 0.5f - layout[currentLocation]->roomLayout[TileMap::TYPE_VISUAL].getMapOffsetY());
			}
		}
	}

	for (vector<Prisoners*>::iterator prisoner = prisonerList.begin(); prisoner != prisonerList.end(); ++prisoner)
	{
		Prisoners* tempPrisoner = *prisoner;

		if (tempPrisoner->getRender())
		{
			Render2DMesh(tempPrisoner->getSprite(), false, (float)TILESIZE * 1.5f, tempPrisoner->getPos().x + TILESIZE * 0.5f - layout[currentLocation]->roomLayout[TileMap::TYPE_VISUAL].getMapOffsetX(), tempPrisoner->getPos().y + TILESIZE * 0.5f - layout[currentLocation]->roomLayout[TileMap::TYPE_VISUAL].getMapOffsetY());

			if (DEBUG)
			{
				Render2DMesh(findMesh("GEO_DEBUGQUAD"), false, (float)TILESIZE, tempPrisoner->getPos().x + TILESIZE * 0.5f - layout[currentLocation]->roomLayout[TileMap::TYPE_VISUAL].getMapOffsetX(),  tempPrisoner->getPos().y + TILESIZE * 0.5f - layout[currentLocation]->roomLayout[TileMap::TYPE_VISUAL].getMapOffsetY());
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
		RenderTextOnScreen(findMesh("GEO_TEXT_BACKGROUND"), ss.str(), findColor("Darkblue"), specialFontSize, 0,sceneHeight - specialFontSize );
		std::ostringstream ss2;
		ss2.precision(1);
		ss2<< "Day: " << day.getCurrentTime().day;
		RenderTextOnScreen(findMesh("GEO_TEXT_BACKGROUND"), ss2.str(), findColor("Red"), specialFontSize,0, sceneHeight - specialFontSize*2 );
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
		RenderTextOnScreen(findMesh("GEO_TEXT_BACKGROUND"), ss.str(), findColor("Skyblue"), specialFontSize, 0, sceneHeight - specialFontSize);

		std::ostringstream ss2;
		ss2.precision(1);
		ss2<< "Day:" << day.getCurrentTime().day;
		RenderTextOnScreen(findMesh("GEO_TEXT_BACKGROUND"), ss2.str(), findColor("Red"), specialFontSize,0, sceneHeight - specialFontSize*2 );

		Render2DMesh(findMesh(day.sun.mesh),false, day.sun.size, day.sun.pos);

		if (DEBUG)
		{
			Render2DMesh(findMesh("GEO_DEBUGQUAD"),false, day.sun.size, day.sun.pos);
		}
	}
	glDisable(GL_DEPTH_TEST);
}

void SceneGame::RenderItem(void)
{
	for (vector<CItem*>::iterator it = itemList.begin(); it != itemList.end(); ++it)
	{
		CItem *renderItem = (CItem *)*it;
		if (renderItem->getLocationID() == layout[currentLocation]->roomLayout[TileMap::TYPE_VISUAL].getID() && renderItem->getItemStatus() == CItem::ITEM_ONGROUND)
		{
			Render2DMesh(renderItem->getMesh(), false, static_cast<float>(TILESIZE), static_cast<float>(renderItem->getItemPos().x + TILESIZE * 0.5 - layout[currentLocation]->roomLayout[TileMap::TYPE_VISUAL].getMapOffsetX()), static_cast<float>(renderItem->getItemPos().y + TILESIZE * 0.5 - layout[currentLocation]->roomLayout[TileMap::TYPE_VISUAL].getMapOffsetY()));
		}
	}
}

void SceneGame::RenderPlayerInventory(bool toggle)
{
	float firstButton, buttonScaleX, buttonScaleY;
	//store location to render item
	for(unsigned i = 0; i < gameInterfaces[currentState].buttons.size(); ++i)
	{
		if (gameInterfaces[currentState].buttons[i].getType() == Buttons::IMAGE_BUTTON && gameInterfaces[currentState].buttons[i].getName() == "0")
		{
			firstButton = gameInterfaces[currentState].buttons[i].getPos().x;
			buttonScaleX = gameInterfaces[currentState].buttons[i].getScale().x;
			buttonScaleY = gameInterfaces[currentState].buttons[i].getScale().y;
			break;
		}
	}

	for (unsigned int i = 0; i < player->getInventory().getVecOfItems().size(); i++)
	{
		if (player->getInventory().getVecOfItems().at(i)->getItemStatus() == CItem::ITEM_ININVENTORY)
			Render2DMesh(player->getInventory().getVecOfItems().at(i)->getMesh(), false, (float)(TILESIZE), (float)((firstButton + buttonScaleX * 0.5) + i * buttonScaleX), (float)(buttonScaleY * 0.5 + translateInventoryY));
	}
}

void SceneGame::RenderItemOnMouse(bool pressed)
{
	if (pressed && updateMousePos == false)
	{
		for (unsigned i = 0; i < gameInterfaces[currentState].buttons.size(); ++i)
		{
			if (gameInterfaces[currentState].buttons[i].getType() == Buttons::IMAGE_BUTTON)
			{
				if (tempMouseX < gameInterfaces[currentState].buttons[i].getPos().x + gameInterfaces[currentState].buttons[i].getScale().x
					&& tempMouseX > gameInterfaces[currentState].buttons[i].getPos().x
					&& tempMouseY < gameInterfaces[currentState].buttons[i].getPos().y + gameInterfaces[currentState].buttons[i].getScale().y
					&& tempMouseY > gameInterfaces[currentState].buttons[i].getPos().y)
				{
					Render2DMesh(player->getInventory().getVecOfItems().at(stoi(gameInterfaces[currentState].buttons[i].getName()))->getMesh(), false, static_cast<float>(TILESIZE), mousePos.x, mousePos.y);
				}
			}
		}
	}
}

void SceneGame::RenderEnergy(void)
{
	Render2DMesh(findMesh("GEO_ENERGY"), false, Vector2(110, 80), Vector2(sceneWidth*0.11f, sceneHeight*0.045f));
	Render2DMesh(findMesh("GEO_ENERGYBAR"), false, Vector2((float)energyScale, 65), Vector2(energyTranslate, sceneHeight*0.045f));
	Render2DMesh(findMesh("GEO_PRISONER"), false, Vector2(50, 75), Vector2(sceneWidth*0.03f, sceneHeight*0.045f));
}
void SceneGame::UpdateEnergy(double dt)
{
	if(player->getState() == StateMachine::RUN_STATE)
	{
		energyScale -= (float)dt * 20 * 0.85f;
		energyTranslate -= (float)dt * 20 / 2.5f;
	}

	else if(player->getState() == StateMachine::WALK_STATE)
	{
		energyScale += (float)dt * 8 * 0.85f;
		energyTranslate += (float)dt * 8 / 2.5f;
	}
	else if(player->getState() == StateMachine::IDLE_STATE)
	{
		energyScale += (float)dt * 10 * 0.85f;
		energyTranslate += (float)dt * 10 / 2.5f;
	}

	if (energyScale > 85)
	{
		energyScale = 85;
		energyTranslate = sceneWidth * 0.105f;
	}

	else if(energyScale < 0)
	{
		energyScale = 0;
	}
}

void SceneGame::RenderInstruct(void)
{
	float y_Space = specialFontSize;
	for (vector<Instructions>::iterator Instruct = instructions.begin(); Instruct != instructions.end(); ++Instruct)
	{
		y_Space +=specialFontSize;
		std::ostringstream ss;
		ss.precision(2);
		ss << Instruct->GetHeader() <<":"<< Instruct->GetText() ;
		RenderTextOnScreen(findMesh("GEO_TEXT"), ss.str(), findColor("White"), specialFontSize, 0,sceneHeight -y_Space );
		//16, 736 original position
	}
	y_Space = specialFontSize;

	glDisable(GL_DEPTH_TEST);
}

void SceneGame::RenderDialogs(void)
{
	Render2DMesh(findMesh("GEO_BUBBLE"), false, Vector2(375, 64), Vector2(sceneWidth*0.8f, sceneHeight*0.85f));
	RenderTextOnScreen(findMesh("GEO_TEXT"), dialogString, findColor("Darkblue"), specialFontSize*0.4f, sceneWidth * 0.65f,sceneHeight*0.85f);
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


Dialogs SceneGame::findDialog(Dialog_ID diaID)
{
	Dialogs tempDialogs;
	for (vector<Dialogs>::iterator it = dialogs.begin(); it != dialogs.end(); ++it)
	{
		tempDialogs = *it;
		if (tempDialogs.GetID() == diaID)
		{

			return tempDialogs;
		}
	}

	std::cout << "Unable to find Dialog! Check your naming" << std::endl;

	return tempDialogs;
}

bool SceneGame::findItem(string name)
{
	for (unsigned i = 0; i < player->getInventory().getVecOfItems().size(); ++i)
	{
		if (player->getInventory().getVecOfItems()[i]->getItemName() == name)
		{
			return true;
		}
	}
	return false;
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

	std::cout << "No such key! Check your naming!" << std::endl;
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
void SceneGame::save (string file)
{
	Branch BScore = TextTree::FileToRead(file);
	Score playerScore;

	playerScore.setScore(day.getCurrentTime().day);

	score.push_back(playerScore);

	for (vector<Score>::iterator it = score.begin(); it != score.end(); ++it)
	{
		for (vector<Score>::iterator it2 = score.begin(); it2 != score.end(); ++it2)
		{
			Score temp;
			if (it->getScore() < it2->getScore())   
			{ 
				temp = *it2;             
				*it2 = *it;
				*it = temp;
			}
		}
	}

	score.pop_back();

		int  currentScore = 0;
	for (vector<Branch>::iterator branch = BScore.childBranches.begin(); branch != BScore.childBranches.end(); ++branch)
	{
		for (vector<Attribute>::iterator attri = branch->attributes.begin(); attri != branch->attributes.end(); ++attri)
		{
			Attribute tempAttri = *attri;
			if (attri->name=="Score")
			attri->value= to_string((long double)score[currentScore].getScore());
		}
		currentScore++;
	}
	BScore.printBranch();
	TextTree::FileToWrite(file, BScore);
}