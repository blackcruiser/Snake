#include "stdafx.h"
#include "Game.h"
#include "Shader.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

Game *Game::m_pInstance = NULL;

Game::Game() :
	m_pWindow(NULL), m_pFood(NULL), m_pSnake(NULL), 
	m_pMeshboard(NULL), m_pScoreboard(NULL)
{}

Game::~Game()
{
	m_pInstance = NULL;
}

Game* Game::GetInstance()
{
	if (NULL == m_pInstance)
		m_pInstance = new Game;
	return m_pInstance;
}

int Game::Initialize()
{
	m_width = 640.00f;
	m_height = 600.00f;

	if (!glfwInit())
		return -1;

	m_pWindow = glfwCreateWindow(m_width, m_height, "SNAKE GAME", NULL, NULL);
	if (!m_pWindow)
	{
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(m_pWindow);

	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	glfwSetKeyCallback(m_pWindow, KeyCallback);

	Rectf partRegion, fullRegion= { 0, 0, 
		static_cast<float>(m_width), static_cast<float>(m_height)  };

	m_pSceneShader = new Shader("./Shader/Module.vs", "./Shader/Module.frag");
	m_pTextShader = new Shader("./Shader/Text.vs", "./Shader/Text.frag");

	partRegion = fullRegion;
	partRegion.height *= 0.80f;
	m_pFood = new Food(partRegion, 16, 16, m_pSceneShader);
	m_pSnake = new Snake(partRegion, 16, 16, 0, m_pSceneShader);
	m_pMeshboard = new Meshboard(partRegion, 16, 16, m_pSceneShader);

	partRegion = fullRegion;
	partRegion.y = fullRegion.height * 0.80f;
	partRegion.height *= fullRegion.height * 0.20f;
	m_pScoreboard = new Scoreboard(partRegion, 640, 480, m_pSceneShader, m_pTextShader);

	return 0;
}

void Game::Run()
{
	uint64_t curTime, lastTime, timeLimit;

	WillRender();

	lastTime = 0;
	timeLimit = glfwGetTimerFrequency() / 4;
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(m_pWindow))
	{
		//Process events
		curTime = glfwGetTimerValue();
		if (curTime - lastTime >= timeLimit)
		{
			lastTime = curTime;
			onTimeout();
		}
		glfwPollEvents();

		//Game logic
		if (true == m_pSnake->CheckCollision())
		{
			glfwSetWindowShouldClose(m_pWindow, GLFW_TRUE);
		};

		if (true == m_pSnake->CheckInbound(m_pFood->Col(), m_pFood->Row()))
		{
			m_pSnake->InsertBlock();
			while (true == m_pSnake->CheckInbound(m_pFood->Col(), m_pFood->Row()))
			{
				m_pFood->Reset();
			}
		}

		//Render
		Render();
	}

	DidRender();
}

void Game::Terminate()
{
	glfwDestroyWindow(m_pWindow);
	m_pWindow = NULL;

	glfwTerminate();

	delete m_pFood;
	delete m_pMeshboard;
	delete m_pSnake;
	delete m_pScoreboard;

	delete m_pSceneShader;
	delete m_pTextShader;
}


void Game::WillRender()
{
	GLuint glProjLoc;
	glm::mat4 glmProjMat;

	glViewport(0, 0, m_width, m_height);

	glmProjMat = glm::ortho(0.0f, static_cast<GLfloat>(m_width),
		0.0f, static_cast<GLfloat>(m_height));

	m_pSceneShader->Use();
	glProjLoc = glGetUniformLocation(m_pTextShader->program, "projection");
	glUniformMatrix4fv(glProjLoc, 1, GL_FALSE, glm::value_ptr(glmProjMat));
	GL_PRINT_ERROR;

	m_pTextShader->Use();
	glProjLoc = glGetUniformLocation(m_pTextShader->program, "projection");
	glUniformMatrix4fv(glProjLoc, 1, GL_FALSE, glm::value_ptr(glmProjMat));

	m_pMeshboard->WillRender();
	m_pFood->WillRender();
	m_pSnake->WillRender();
	m_pScoreboard->WillRender();

	m_pFood->Reset();
	m_pSnake->Reset();
}

void Game::Render()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	m_pMeshboard->Render();
	m_pFood->Render();
	m_pSnake->Render();
	m_pScoreboard->Render();

	glfwSwapBuffers(m_pWindow);
}

void Game::DidRender()
{
	m_pSnake->DidRender();
	m_pFood->DidRender();
	m_pMeshboard->DidRender();
	m_pScoreboard->DidRender();
}


void Game::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (GLFW_PRESS == action)
	{
		m_pInstance->onKeyDown(key);
	}
}

void Game::onKeyDown(int key)
{
	m_pSnake->onKeyDown(key);
}

void Game::onTimeout()
{
	m_pSnake->onTimeout();
}
