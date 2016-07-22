#include "stdafx.h"
#include "Game.h"

Game *Game::mp_instance = NULL;

Game::Game() :
	mp_window(NULL)
{
	mp_instance = this;
}


Game::~Game()
{
	mp_instance = NULL;
}

Game* Game::GetInstance()
{
	if (NULL == mp_instance)
		new Game;
	return mp_instance;
}

int Game::Initialize()
{
	if (!glfwInit())
		return -1;

	mp_window = glfwCreateWindow(640, 480, "SNAKE GAME", NULL, NULL);
	if (!mp_window)
	{
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(mp_window);

	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	glfwSetKeyCallback(mp_window, KeyCallback);

	mp_shader = new Shader("./Shader/Module.vs", "./Shader/Module.frag");

	mp_food = new Food(16, 16);
	mp_snake = new Snake(16, 16, 0);
	mp_meshboard = new Meshboard(16, 16);

	mp_scoreboard = new Scoreboard(640, 480);


	return 0;
}

void Game::Run()
{
	uint64_t curTime, lastTime, timeLimit;

	WillRender();

	lastTime = 0;
	timeLimit = glfwGetTimerFrequency() / 4;
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(mp_window))
	{
		//Process events
		curTime = glfwGetTimerValue();
		if (curTime - lastTime >= timeLimit)
		{
			lastTime = curTime;
			//onTimeout();
		}
		glfwPollEvents();

		//Game logic
		/*
		if (true == mp_snake->CheckCollision())
		{
			glfwSetWindowShouldClose(mp_window, GLFW_TRUE);
		};

		if (true == mp_snake->CheckInbound(mp_food->Col(), mp_food->Row()))
		{
			mp_snake->InsertBlock();
			while (true == mp_snake->CheckInbound(mp_food->Col(), mp_food->Row()))
			{
				mp_food->Reset();
			}
		}
		*/

		//Render
		Render();
	}

	DidRender();
}

void Game::Terminate()
{
	glfwDestroyWindow(mp_window);
	mp_window = NULL;

	glfwTerminate();
}


void Game::WillRender()
{
	glfwGetFramebufferSize(mp_window, &m_width, &m_height);
	glViewport(0, 0, m_width, m_height);

	//mp_meshboard->WillRender();

	//mp_food->WillRender();

	/*
	mp_snake->WillRender();
	*/

	mp_scoreboard->WillRender();

	//mp_food->Reset();
	/*
	mp_snake->Reset();
	*/
}

void Game::Render()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	//mp_shader->Use();

	//mp_meshboard->Render();
	//mp_food->Render();
	//mp_snake->Render();

	mp_scoreboard->Render();

	glfwSwapBuffers(mp_window);
}

void Game::DidRender()
{
	mp_snake->DidRender();
	mp_food->DidRender();
	mp_meshboard->DidRender();
	mp_scoreboard->DidRender();
}


void Game::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (GLFW_PRESS == action)
	{
		mp_instance->onKeyDown(key);
	}
}

void Game::onKeyDown(int key)
{
	mp_snake->onKeyDown(key);
}

void Game::onTimeout()
{
	mp_snake->onTimeout();
}
