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
	int err;

	if (!glfwInit())
		return -1;

	mp_window = glfwCreateWindow(640, 480, "SNAKE GAME", NULL, NULL);
	if (!mp_window)
	{
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(mp_window);

	glfwSetKeyCallback(mp_window, KeyCallback);

	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	glfwGetFramebufferSize(mp_window, &m_width, &m_height);
	glViewport(0, 0, m_width, m_height);

	err = glGetError();

	mp_food = new Food(8, 8);
	mp_snake = new Snake(8, 8, 1, 1);
	mp_meshboard = new Meshboard(8, 8);

	mp_shader = new Shader(".\\vertexShader.txt", ".\\fragmentShader.txt");

	return 0;
}

void Game::Run()
{
	uint64_t curTime, lastTime;

	mp_meshboard->WillRender();

	mp_food->WillRender();
	mp_food->Reset();

	mp_snake->WillRender();
	mp_snake->Reset();

	lastTime = 0;
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(mp_window))
	{
		curTime = glfwGetTimerValue();
		if (curTime - lastTime >= glfwGetTimerFrequency())
		{
			onTimeout();
		}
		lastTime = curTime;

		glfwPollEvents();

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		mp_shader->Use();
		/* Render here */
		Render();

		/* Poll for and process events */
		glfwSwapBuffers(mp_window);
	}

	mp_snake->DidRender();
	mp_food->DidRender();
	mp_meshboard->DidRender();
	
}

void Game::Terminate()
{
	glfwDestroyWindow(mp_window);
	mp_window = NULL;

	glfwTerminate();
}


void Game::sendMessage(int message)
{
	m_messageQueue.push_back(message);
}

void Game::dispatchMessage()
{
	for (auto it_message = m_messageQueue.begin(); it_message != m_messageQueue.end(); it_message++)
	{
		int message = *it_message;

		if (MSG_GAME_1 <= message && message <= MSG_GAME_2)
			ProcessMessage(message);
		else if (MSG_SNAKE_1 <= message && message <= MSG_SNAKE_2)
			mp_snake->ProcessMessage(message);
	}
}

void Game::ProcessMessage(int message)
{
}


void Game::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (GLFW_PRESS == action)
	{
		mp_instance->onKeyDown(key);
	}
}

void Game::TimerCallback()
{

}

void Game::Render()
{
	mp_meshboard->Render();
	mp_food->Render();
	mp_snake->Render();
}

void Game::onKeyDown(int key)
{
	mp_snake->onKeyDown(key);
}

void Game::onTimeout()
{
	mp_snake->onTimeout();
}
