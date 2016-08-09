#include "stdafx.h"
#include "Game.h"
#include "Shader.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

Game *Game::m_pInstance = NULL;

Game::Game(int width, int height, int cols, int rows) :
	m_pWindow(NULL), m_pFood(NULL), m_pSnake(NULL),
	m_pMeshboard(NULL), m_pScoreboard(NULL),
	m_width(width), m_height(height), m_cols(cols), m_rows(rows),
	m_lastTime(0), m_curTime(0)
{
}

Game::~Game()
{
	m_pInstance = NULL;
}

Game* Game::GetInstance(int width, int height, int cols, int rows)
{
	if (NULL == m_pInstance)
		m_pInstance = new Game(width, height, cols, rows);
	else if (width != m_pInstance->m_width || height != m_pInstance->m_height
		|| cols != m_pInstance->m_cols || rows != m_pInstance->m_rows)
		m_pInstance  = new Game(width, height, cols, rows);

	return m_pInstance;
}

int Game::Initialize()
{
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

	Rectf partRegion, fullRegion = { 0, 0,
		static_cast<float>(m_width), static_cast<float>(m_height) };

	m_timeLimit = glfwGetTimerFrequency() / 4;

	m_pSceneShader = new Shader("./Shader/Module.vs", "./Shader/Module.frag");
	m_pTextShader = new Shader("./Shader/Text.vs", "./Shader/Text.frag");
	m_pSnakeShader = new Shader("./Shader/Snake.vs", "./Shader/Snake.frag");

	partRegion = fullRegion;
	partRegion.height *= 0.80f;
	m_pFood = new Food(partRegion, m_rows, m_cols, m_pSceneShader);
	m_pSnake = new Snake(partRegion, m_rows, m_cols, 0, m_pSnakeShader);
	m_pMeshboard = new Meshboard(partRegion, m_rows, m_cols, m_pSceneShader);

	partRegion = fullRegion;
	partRegion.y = fullRegion.height * 0.80f;
	partRegion.height = fullRegion.height * 0.20f;
	m_pScoreboard = new Scoreboard(partRegion, m_pSceneShader, m_pTextShader);

	m_pFood->Reset();
	m_pSnake->Reset();
	m_pScoreboard->Reset();

	return 0;
}

void Game::Run()
{
	WillRender();

	while (!glfwWindowShouldClose(m_pWindow))
	{
		ProcessEvent();
		ProcessLogic();
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
	delete m_pSnakeShader;
}

void Game::ProcessEvent()
{
	m_curTime = glfwGetTimerValue();
	if (m_curTime - m_lastTime >= m_timeLimit)
	{
		m_lastTime = m_curTime;
		onTimeout();
	}
	glfwPollEvents();
}

void Game::ProcessLogic()
{
	if (true == m_pSnake->CheckCollision())
	{
		glfwSetWindowShouldClose(m_pWindow, GLFW_TRUE);
	};

	if (true == m_pSnake->CheckInbound(m_pFood->Col(), m_pFood->Row()))
	{
		m_pSnake->InsertBlock();

		while (true == m_pSnake->CheckInbound(m_pFood->Col(), m_pFood->Row()))
		{
			m_pScoreboard->AddPoint();
			m_pFood->Reset();
		}
	}
}


void Game::WillRender()
{
	GLuint glProjLoc;
	glm::mat4 glmProjMat;

	glViewport(0, 0, m_width, m_height);

	glmProjMat = glm::ortho(0.0f, static_cast<GLfloat>(m_width),
		0.0f, static_cast<GLfloat>(m_height));

	glProjLoc = glGetUniformLocation(m_pSceneShader->program, "projMat");
	m_pSceneShader->Use();
	glUniformMatrix4fv(glProjLoc, 1, GL_FALSE, glm::value_ptr(glmProjMat));

	glProjLoc = glGetUniformLocation(m_pTextShader->program, "projMat");
	m_pTextShader->Use();
	glUniformMatrix4fv(glProjLoc, 1, GL_FALSE, glm::value_ptr(glmProjMat));

	glProjLoc = glGetUniformLocation(m_pSnakeShader->program, "projMat");
	m_pSnakeShader->Use();
	glUniformMatrix4fv(glProjLoc, 1, GL_FALSE, glm::value_ptr(glmProjMat));

	m_pMeshboard->WillRender();
	m_pFood->WillRender();
	m_pSnake->WillRender();
	m_pScoreboard->WillRender();
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
