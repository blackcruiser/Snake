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

#include "Shader.h"
#include "BaseModule.h"
#include "Food.h"
#include "Snake.h"
#include "Meshboard.h"

class Game : BaseModule
{
private:
	static Game *mp_instance;

	Shader *mp_shader;
	GLFWwindow *mp_window;

	Snake *mp_snake;
	Food *mp_food;
	Meshboard *mp_meshboard;

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