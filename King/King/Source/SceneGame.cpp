#include "SceneGame.h"

#include "GL\glew.h"
#include "shader.hpp"
#include "Utility.h"
#include "LoadTGA.h"

SceneGame::SceneGame(void)
{
	m_parameters.resize(U_TOTAL);
	lights.resize(NUM_LIGHTS);
}

SceneGame::~SceneGame(void)
{
}

void SceneGame::Init(string config)
{
	Scene2D::Init();

	gameBranch = TextTree::FileToRead(config);
	Config();


}

void SceneGame::Update(double dt)
{
	Scene2D::Update(dt);
}

void SceneGame::Render(void)
{
	Scene2D::Render();
	glDisable(GL_DEPTH_TEST);

	// Render


	glEnable(GL_DEPTH_TEST);
}

void SceneGame::Exit(void)
{
	Scene2D::Exit();

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
		if (branch->branchName == "Mesh")
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
	}
}

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
					for (unsigned j = lastContinue; j < attriValue.size() && j != ','; ++j)
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

						lastContinue = j;
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

void SceneGame::InitVariables(string config)
{

}

void SceneGame::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
{
	if(!mesh || mesh->textureID <= 0)
		return;

	glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, 1024, 0, 800, -10, 10);
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