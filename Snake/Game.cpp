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
	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	mp_window = glfwCreateWindow(640, 480, "SNAKE GAME", NULL, NULL);
	if (!mp_window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(mp_window);

	glfwSetKeyCallback(mp_window, KeyCallback);

	/*Init glew after setting context*/
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}
	glViewport(0, 0, 640, 480);

	mp_food = new Food(4, 4);
	mp_snake = new Snake(1, 4, 4, 1);
	mp_meshboard = new Meshboard(640, 480, 4, 4);

	mp_shader = new Shader(".\\vertexShader.txt", ".\\fragmentShader.txt");

	return 0;
}

void Game::Start()
{
	mp_shader->Use();

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(mp_window))
	{
		glfwPollEvents();

		/* Render here */
		Render();

		/* Poll for and process events */
		glfwSwapBuffers(mp_window);
	}
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
	mp_snake->Render();
	mp_food->Render();
}

void Game::onKeyDown(int key)
{
	mp_snake->onKeyDown(key);
}

void Game::onTimeout()
{
}
