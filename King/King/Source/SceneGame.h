#ifndef SCENE_GAME
#define SCENE_GAME

#include "Scene2D.h"
#include "TextTree.h"

#define DEBUG true

using std::string;

#include "Room.h"
#include "Interface.h"
#include "Sound.h"
#include "Player.h"

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
		EXIT_STATE,
		MAX_STATE,
	};

	struct Time
	{
		int hour;
		float min;
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
	void InitSettings(string config);
	void InitMesh(string config);
	void InitColor(string config);
	void InitInterface(string config);
	void InitLevel(string config);
	void InitVariables(string config);
	void InitSound(string config);
	void InitPlayer(string config);

	void UpdateOpengl(void);
	void UpdateInput(void);
	void UpdateMouse(void);
	void UpdateState(void);
	void UpdateEffect(void);
	void UpdateInGame(double dt);

	void changeScene(GAME_STATE nextState);

	void RenderInterface(void);
	void RenderLevel(void);
	void RenderCharacters(void);
	void RenderHUD(void);
	void RenderGUI(void);

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
	Vector2 mousePos;

	GAME_STATE currentState;
	float defaultFontSize;
	float specialFontSize;
	float gameSpeed;
	float gameVolume;
	int currentLocation;
	bool paused;
	float difficulty;
	Time currentTime;

	void stringToVector(string text, Vector2 &vec);
	void stringToVector(string text, Vector3 &vec);
	void stringToBool(string text, bool &boo);

	Sound sound;
	Player* player;


};

#endif