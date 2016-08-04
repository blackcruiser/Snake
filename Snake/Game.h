#pragma once

#include <random>

enum GAME_MESSAGE
{
	MSG_GAME_1,
	MSG_GAME_2,
	MSG_SNAKE_1,
	MSG_SNAKE_2,
	MSG_FOOD_1,
};

#include <list>

#include "BaseModule.h"
#include "Food.h"
#include "Snake.h"
#include "Meshboard.h"
#include "Scoreboard.h"

class Game : BaseModule
{
private:
	static Game *m_pInstance;
	Snake *m_pSnake;
	Food *m_pFood;
	Meshboard *m_pMeshboard;
	Scoreboard *m_pScoreboard;

	Shader *m_pSceneShader, *m_pTextShader;
	GLFWwindow *m_pWindow;
	int m_width, m_height;

private:
	Game();

public:
	static Game *GetInstance();
	~Game();

	int Initialize();
	void Run();
	void Terminate();


	void WillRender();
	virtual void Render();
	void DidRender();


	static void KeyCallback(GLFWwindow* window, int key, 
		int scancode, int action, int mods);
	virtual void onKeyDown(int key);
	virtual void onTimeout();
};