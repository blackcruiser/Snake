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
//#include <queue>

#include "BaseModule.h"
#include "Food.h"
#include "Snake.h"
#include "Meshboard.h"

class Game : BaseModule
{
private:
	static Game *mp_instance;

	GLFWwindow *mp_window;
	//std::list<BaseModule *> m_moduleList;
	Snake *mp_snake;
	Food *mp_food;
	Meshboard *mp_meshboard;

	std::list<int> m_messageQueue;

private:
	Game();

public:
	static Game *GetInstance();
	~Game();

	int Initialize();
	void Start();
	void Terminate();

	static void KeyCallback(GLFWwindow* window, int key, 
		int scancode, int action, int mods);
	static void TimerCallback();


	void sendMessage(int message);
	void dispatchMessage();
	virtual void ProcessMessage(int message);
	virtual void Render();
	virtual void onKeyDown(int key);
	virtual void onTimeout();
};