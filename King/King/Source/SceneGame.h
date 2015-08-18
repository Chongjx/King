#ifndef SCENE_GAME
#define SCENE_GAME

#include "Scene2D.h"
#include "TextTree.h"
#include <irrKlang.h>

#define DEBUG true

using std::string;

#include "Room.h"
#include "Interface.h"

class SceneGame : public Scene2D
{
	static const int NUM_LIGHTS = 1;
	static const int TILESIZE = 32;
	static const int GAME_WIDTH = 1024;
	static const int GAME_HEIGHT = 800;
	enum UNIFORM_TYPE
	{
		U_MVP = 0,
		U_MODELVIEW,
		U_MODELVIEW_INVERSE_TRANSPOSE,
		U_MATERIAL_AMBIENT,
		U_MATERIAL_DIFFUSE,
		U_MATERIAL_SPECULAR,
		U_MATERIAL_SHININESS,
		U_LIGHTENABLED,
		U_NUMLIGHTS,
		U_LIGHT0_TYPE,
		U_LIGHT0_POSITION,
		U_LIGHT0_COLOR,
		U_LIGHT0_POWER,
		U_LIGHT0_KC,
		U_LIGHT0_KL,
		U_LIGHT0_KQ,
		U_LIGHT0_SPOTDIRECTION,
		U_LIGHT0_COSCUTOFF,
		U_LIGHT0_COSINNER,
		U_LIGHT0_EXPONENT,
		U_COLOR_TEXTURE_ENABLED,
		U_COLOR_TEXTURE,
		U_TEXT_ENABLED,
		U_TEXT_COLOR,
		U_TOTAL,
	};

	enum AREAS
	{
		MAIN_AREA,
		MAX_AREAS,
	};

	enum GAME_STATE
	{
		MENU_STATE,
		INGAME_STATE,
		INSTRUCTION_STATE,
		HIGHSCORE_STATE,
		OPTIONS_STATE,
		PAUSE_STATE,
		GAMEOVER_STATE,
		MAX_STATE,
	};
public:
	SceneGame(void);
	~SceneGame(void);

	void Init(string config);
	void Update(double dt);
	void UpdateAI(double dt);
	void Render(void);
	void Exit(void);

	void Config(void);
	void InitShaders(void);
	void InitMesh(string config);
	void InitInterface(string config);
	void InitLevel(string config);
	void InitVariables(string config);
	void InitSound(string config);

	void UpdateMenu(void);
	void UpdateInGame(double dt);
	
	void RenderInterface(void);
	void RenderLevel(void);
	void RenderCharacters(void);
	void RenderHUD(void);

	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void Render2DMesh(Mesh *mesh, const bool enableLight, const float size = 1.0f, const float x = 0.0f, const float y = 0.0f, const bool rotate = false);

	Mesh* findMesh(string meshName);

private:
	Branch gameBranch;
	vector<Room> layout;
	vector<Interface> gameInterfaces;

	GAME_STATE currentState;
	float menuFontSize;
	float fontSize;
	float gameSpeed;
	float gameVolume;
	int currentLocation;
};

#endif