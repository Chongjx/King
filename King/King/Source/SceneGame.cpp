#include "SceneGame.h"

#include "GL\glew.h"
#include "shader.hpp"
#include "Utility.h"
#include "LoadTGA.h"

// Set the m_parameters to match the number uniform type & lights to match the number lights
SceneGame::SceneGame(void)
{
	m_parameters.resize(U_TOTAL);
	lights.resize(NUM_LIGHTS);
	layout.resize(MAX_AREAS);
	gameInterfaces.resize(MAX_STATE);

	for (unsigned i = MENU_STATE; i < MAX_STATE; ++i)
	{
		gameInterfaces[i].interfaceType = i;
	}

	currentState = MENU_STATE;
	currentLocation = MAIN_AREA;

	fontSize = 100.f;
	menuFontSize = 200.f;
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

	switch(currentState)
	{
	case MENU_STATE:
		{
			break;
		}
	case INGAME_STATE:
		{
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
	case GAMEOVER_STATE:
		{
			break;
		}
	}

	// Update buttons
	for (unsigned i = 0; i < gameInterfaces[currentState].buttons.size(); ++i)
	{
		//gameInterfaces[currentState].buttons[i].Update(
	}
}

void SceneGame::UpdateAI(double dt)
{
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
			break;
		}
	case GAMEOVER_STATE:
		{
			break;
		}
	}

	RenderInterface();

	std::ostringstream ss;
	ss.precision(5);
	ss << "FPS: " << fps;
	RenderTextOnScreen(findMesh("GEO_TEXT"),  ss.str(), Color(1, 0, 0), fontSize * 0.75f, 0 , GAME_HEIGHT - fontSize);

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

	glDeleteVertexArrays(1, &m_vertexArrayID);
}

// Setting up of the game using data from the text files
void SceneGame::Config(void)
{
	if (DEBUG)
	{	
		gameBranch.printBranch();
	}

	for (vector<Branch>::iterator branch = gameBranch.childBranches.begin(); branch != gameBranch.childBranches.end(); ++branch)
	{
		if (branch->branchName == "Shader")
		{
			string vertexShader, fragmentShader;
			for (vector<Attribute>::iterator attri = branch->attributes.begin(); attri != branch->attributes.end(); ++attri)
			{
				Attribute tempAttri = *attri;
				if (tempAttri.name == "VertexShader")
				{
					vertexShader = tempAttri.value;
				}

				else if (tempAttri.name == "FragmentShader")
				{
					fragmentShader = tempAttri.value;
				}
			}

			m_programID = LoadShaders(vertexShader.c_str(), fragmentShader.c_str());

			InitShaders();
		}

		else if (branch->branchName == "Camera")
		{

		}

		else if (branch->branchName == "Lights")
		{

		}

		else if (branch->branchName == "Mesh")
		{
			for (vector<Attribute>::iterator attri = branch->attributes.begin(); attri != branch->attributes.end(); ++attri)
			{
				Attribute tempAttri = *attri;
				if (tempAttri.name == "Directory")
				{
					InitMesh(tempAttri.value);
				}
			}
		}

		else if (branch->branchName == "Interface")
		{
			for (vector<Attribute>::iterator attri = branch->attributes.begin(); attri != branch->attributes.end(); ++attri)
			{
				Attribute tempAttri = *attri;
				if (tempAttri.name == "Directory")
				{
					InitInterface(tempAttri.value);
				}
			}
		}

		else if (branch->branchName == "Layout")
		{
			for (vector<Attribute>::iterator attri = branch->attributes.begin(); attri != branch->attributes.end(); ++attri)
			{
				Attribute tempAttri = *attri;
				if (tempAttri.name == "Directory")
				{
					InitLevel(tempAttri.value);
				}
			}
		}
		else if (branch->branchName == "Sound")
		{
			for (vector<Attribute>::iterator attri = branch->attributes.begin(); attri != branch->attributes.end(); ++attri)
			{
				Attribute tempAttri = *attri;
				if (tempAttri.name == "Directory")
				{
					InitSound(tempAttri.value);
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
	lights[0].position.Set(0, 20, 0);
	lights[0].color.Set(1, 1, 1);
	lights[0].power = 1;
	lights[0].kC = 1.f;
	lights[0].kL = 0.01f;
	lights[0].kQ = 0.001f;
	lights[0].cosCutoff = cos(Math::DegreeToRadian(45));
	lights[0].cosInner = cos(Math::DegreeToRadian(30));
	lights[0].exponent = 3.f;
	lights[0].spotDirection.Set(0.f, 1.f, 0.f);

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

// Init all meshes needed in the scene from text file
void SceneGame::InitMesh(string config)
{
	Branch meshBranch = TextTree::FileToRead(config);

	if (DEBUG)
	{
		meshBranch.printBranch();
	}

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
			meshVar[i] = 0.0f;
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

			if (attriName == "Color")
			{
				string red;
				string green;
				string blue;
				int lastContinue = 0;

				for (int numColor = 0; numColor < 3; ++numColor)
				{
					for (unsigned j = lastContinue; j < attriValue.size() && attriValue[j] != ','; ++j)
					{
						if (numColor == 0)
						{
							red += attriValue[j];
						}

						else if (numColor == 1)
						{
							green += attriValue[j];
						}

						else
						{
							blue += attriValue[j];
						}

						lastContinue = j + 2;
					}
				}

				meshColor.Set(stof(red), stof(green), stof(blue));
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
			mesh = MeshBuilder::GenerateQuad(meshName, meshColor, meshVar[VAR_LENGTH]);
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
				bool play = false;
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
						if (attriValue == "true" || attriValue == "1")
						{
							repeat = true;
						}

						else
						{
							repeat = false;
						}
					}

					else if (attriName == "Play")
					{
						if (attriValue == "true" || attriValue == "1")
						{
							play = true;
						}

						else
						{
							play = false;
						}
					}

					else if (attriName == "AnimationTime")
					{
						animationTime = stof(attriValue);
					}
				}

				if (sa)
				{
					Animation* anime = new Animation();
					anime->Set(id, startFrame, endFrame, repeat, play, animationTime);
					sa->animations.push_back(anime);
				}
			}
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

void SceneGame::InitInterface(string config)
{
	Branch interfaceBranch = TextTree::FileToRead(config);

	if (DEBUG)
	{
		interfaceBranch.printBranch();
	}

	for (vector<Branch>::iterator branch = interfaceBranch.childBranches.begin(); branch != interfaceBranch.childBranches.end(); ++branch)
	{
		string name = "";
		string text = "";
		Buttons::BUTTON_TYPE type = Buttons::TEXT_BUTTON;
		Vector2 pos;
		float rotation = 0.f;
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
				string xCoord;
				string yCoord;
				int lastContinue = 0;
				for (unsigned j = 0; j < attriValue.size() &&  attriValue[j] != ','; ++j)
				{
					xCoord += attriValue[j];
					lastContinue = j + 2;
				}

				for (unsigned j = lastContinue; j < attriValue.size(); ++j)
				{
					yCoord += attriValue[j];
				}

				pos.x = stof(xCoord);
				pos.y = stof(yCoord);
			}

			else if (attriName == "Rotation")
			{
				rotation = stof(attriValue);
			}

			else if (attriName == "Scale")
			{
				string xCoord;
				string yCoord;
				int lastContinue = 0;
				for (unsigned j = 0; j < attriValue.size() && attriValue[j] != ','; ++j)
				{
					xCoord += attriValue[j];
					lastContinue = j + 2;
				}

				for (unsigned j = lastContinue; j < attriValue.size(); ++j)
				{
					yCoord += attriValue[j];
				}

				scale.x = stof(xCoord);
				scale.y = stof(yCoord);
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
		tempButton.Init(name, text, mesh, pos, scale, rotation, type);

		gameInterfaces[storagePos].buttons.push_back(tempButton);
	}
}

void SceneGame::InitLevel(string config)
{
	Branch levelBranch = TextTree::FileToRead(config);

	if (DEBUG)
	{
		levelBranch.printBranch();
	}

	for (vector<Branch>::iterator branch = levelBranch.childBranches.begin(); branch != levelBranch.childBranches.end(); ++branch)
	{
		string directory = "";
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
				if (attriValue == "true" || attriValue == "1")
				{
					enableX = true;
				}

				else
				{
					enableX = false;
				}
			}

			else if (attriName == "EnableYScroll")
			{
				if (attriValue == "true" || attriValue == "1")
				{
					enableY = true;
				}

				else
				{
					enableY = false;
				}
			}

			else if (attriName == "Directory")
			{
				directory = attriValue;
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

		TileMap tempMap;
		tempMap.Init(mapVar[VAR_ID], mapVar[VAR_SCREEN_WIDTH], mapVar[VAR_SCREEN_HEIGHT], mapVar[VAR_MAP_WIDTH], mapVar[VAR_MAP_HEIGHT], mapVar[VAR_MAP_OFFSETX], mapVar[VAR_MAP_OFFSETY], mapVar[VAR_MAP_FINE_OFFSETX], mapVar[VAR_MAP_FINE_OFFSETY], enableX, enableY, mapVar[TILE_SIZE]);
		tempMap.LoadMap(directory);

		layout[layout.size() - 1].ID = mapVar[VAR_ID];
		layout[layout.size() - 1].roomLayout.push_back(tempMap);
	}
}

// Init all game variables in the scene from text file
void SceneGame::InitVariables(string config)
{

}

// Init all game variables in the scene from text file
void SceneGame::InitSound(string config)
{
	Branch soundBranch = TextTree::FileToRead(config);
	irrklang::ISoundEngine* Soundengine = irrklang::createIrrKlangDevice();

	if (DEBUG)
	{
		soundBranch.printBranch();
	}
	for (vector<Branch>::iterator branch = soundBranch.childBranches.begin(); branch != soundBranch.childBranches.end(); ++branch)
	{
		string soundfile="";
		bool loop=false;
		float volume=0.0f;
		for (vector<Attribute>::iterator attri = branch->attributes.begin(); attri != branch->attributes.end(); ++attri)
		{
			Attribute tempAttri = *attri;
			string attriName = tempAttri.name;
			string attriValue = tempAttri.value;
			if(attriName=="SoundFile")
			{
				soundfile = attriValue;
			}
			else if(attriName=="Loop")
			{
				if (attriValue == "true" || attriValue == "1")
				{
					loop = true;
				}
			}
			else if(attriName=="DefaultVol")
			{
				volume = stof(attriValue);
			}
		}

		irrklang::ISoundSource* bookSound = Soundengine->addSoundSourceFromFile(soundfile.c_str()); 
		bookSound->setDefaultVolume(volume);
		Soundengine->play2D(bookSound,loop);
	}
}

void SceneGame::UpdateMenu(void)
{

}

void SceneGame::UpdateInGame(double dt)
{
}

void SceneGame::RenderInterface(void)
{
	for(unsigned i = 0; i < gameInterfaces[currentState].buttons.size(); ++i)
	{
		if (gameInterfaces[currentState].buttons[i].getType() == Buttons::TEXT_BUTTON)
		{
			RenderTextOnScreen(gameInterfaces[currentState].buttons[i].getMesh(), gameInterfaces[currentState].buttons[i].getText(), Color(1, 1, 1), fontSize, gameInterfaces[currentState].buttons[i].getPos().x, gameInterfaces[currentState].buttons[i].getPos().y);
		}

		else
		{
			Render2DMesh(gameInterfaces[currentState].buttons[i].getMesh(), false, gameInterfaces[currentState].buttons[i].getScale().x, gameInterfaces[currentState].buttons[i].getPos().x, gameInterfaces[currentState].buttons[i].getPos().y);
		}
	}
}

void SceneGame::RenderLevel(void)
{
	// check if player is within the game
	if ((unsigned)currentLocation < layout.size())
	{
		for (unsigned numMaps = 0; numMaps < layout[currentLocation].roomLayout.size(); ++ numMaps)
		{
			int m = 0;
			for(int i = 0; i < layout[currentLocation].roomLayout[numMaps].getNumTilesHeight(); i++)
			{
				for(int k = 0; k < layout[currentLocation].roomLayout[numMaps].getNumTilesWidth(); k++)
				{
					m = layout[currentLocation].roomLayout[numMaps].getTileOffsetX() + k;

					if ((layout[currentLocation].roomLayout[numMaps].getTileOffsetX() + k) >= layout[currentLocation].roomLayout[numMaps].getNumTilesMapWidth())
						break;

					TileSheet *tilesheet = dynamic_cast<TileSheet*>(findMesh("GEO_TILESHEET"));
					tilesheet->m_currentTile = layout[currentLocation].roomLayout[numMaps].screenMap[i][m];

					Render2DMesh(findMesh("GEO_TILESHEET"), false, (float)layout[currentLocation].roomLayout[numMaps].getTileSize() + 3, (k + 0.5f) * layout[currentLocation].roomLayout[numMaps].getTileSize() - layout[currentLocation].roomLayout[numMaps].getMapFineOffsetX(), layout[currentLocation].roomLayout[numMaps].getScreenHeight() - (float)(i + 0.5f) * layout[currentLocation].roomLayout[numMaps].getTileSize());
				}
			}
		}
	}
}

void SceneGame::RenderCharacters(void)
{
}

void SceneGame::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
{
	if(!mesh || mesh->textureID <= 0)
		return;

	glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, 1600, 0, 1200, -10, 10);
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity();
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	modelStack.Translate(x, y, 0);
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

void SceneGame::Render2DMesh(Mesh *mesh, const bool enableLight, const float size, const float x, const float y, const bool rotate)
{
	Mtx44 ortho;
	ortho.SetToOrtho(0, 1024, 0, 800, -100, 100);
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity();
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	modelStack.Translate(x, y, 0);
	modelStack.Scale(size, size, size);
	if (rotate)
		modelStack.Rotate(90, 0, 0, 1);

	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	if(mesh->textureID > 0)
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