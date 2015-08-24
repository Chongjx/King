#ifndef SCENE_GAME
#define SCENE_GAME

#include "Scene2D.h"
#include "TextTree.h"

#define DEBUG true

#include "Player.h"
#include "Prisoners.h"
#include "Guards.h"
#include "Interface.h"
#include "Sound.h"
#include "Level.h"
#include "Day.h"
#include "Interactions.h"

using std::string;

class SceneGame : public Scene2D
{
	static const int NUM_LIGHTS = 1;
	static const int TILESIZE = 32;
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
		CELL_AREA,
		EXERCISE_AREA,
		CANTEEN_AREA,
		SHOWER_AREA,
		MEETING_AREA,
		GAURDROOM_AREA,
		FAKEROOM_1_AREA,
		FAKEROOM_2_AREA,
		FAKEROOM_3_AREA,
		FAKEROOM_4_AREA,
		CAMERAROOM_AREA,
		COURTYARD_AREA,
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
		EXIT_STATE,
		MAX_STATE,
	};

	enum INTERACTIONS
	{
		PICKUP_ITEM,
		DROP_ITEM,
		TALK_WITH_PRISONERS,
		TALK_WITH_GUARDS,
		OPEN_DOOR,
		CLOSE_DOOR,
		RUNNING_ON_THREADMILL,
		ATTACK,
		MAX_INTERACTIONS
	};

public:
	SceneGame(void);
	~SceneGame(void);

	void Init(string config);
	void Update(double dt);
	void Render(void);
	void Exit(void);

	void Config(void);
	void InitShaders(void);
	void InitSettings(string config);
	void InitMesh(string config);
	void InitColor(string config);
	void InitInterface(string config);
	void InitLevel(string config);
	void InitVariables(string config);
	void InitSound(string config);
	void InitPlayer(string config);
	void InitInteractions(string config);
	void InitAI(string config);
void InitObjective(string config);

	void UpdateOpengl(void);
	void UpdateInput(void);
	void UpdateMouse(void);
	void UpdateState(void);
	void UpdateEffect(void);
	void UpdateInGame(double dt);
	void UpdatePlayer(double dt);
	void UpdateAI(double dt);
	void UpdateMap(void);
	void UpdateInteractions(void);

	void changeScene(GAME_STATE nextState);

	void RenderInterface(void);
	void RenderLevel(void);
	void RenderCharacters(void);
	void RenderHUD(void);
	void RenderGUI(void);
	void RenderTime(void);

	void Render3DMesh(Mesh *mesh, bool enableLight);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y, float rotation = 0.f);
	void Render2DMesh(Mesh *mesh, const bool enableLight, const float size = 1.0f, const float x = 0.0f, const float y = 0.0f, const float rotation = 0.f);
	void Render2DMesh(Mesh *mesh, const bool enableLight, const Vector2 size, const Vector2 pos, const float rotation = 0.f);

	Mesh* findMesh(string meshName);
	Color findColor(string colorName);
	bool getKey(string keyName);
private:
	float sceneWidth;
	float sceneHeight;

	Branch gameBranch;
	vector<KEYS> interactionKeys;
	vector<Room> layout;
	vector<Interface> gameInterfaces;
	vector<Prisoners*> prisonerList;
	vector<Guards*> guardList;
	Vector2 mousePos;
	vector<Interactions> interactionList;

	GAME_STATE currentState;
	INTERACTIONS currentInteraction;
	float defaultFontSize;
	float specialFontSize;
	float gameVolume;
	float gameSpeed;
	int currentLocation;
	bool paused;

	void stringToVector(string text, Vector2 &vec);
	void stringToVector(string text, Vector3 &vec);
	void stringToBool(string text, bool &boo);

	Sound sound;
	Player* player;
	Day day;
};

#endif