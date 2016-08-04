#pragma once

#include <list>
#include <utility>
#include <random>

#include "BaseModule.h"
#include "Shader.h"

enum SNAKE_MOVE_DIRECTION
{
	SNAKE_MOVE_DIRECTIION_UP,
	SNAKE_MOVE_DIRECTIION_RIGHT,
	SNAKE_MOVE_DIRECTIION_DOWN,
	SNAKE_MOVE_DIRECTIION_LEFT,
};


class Snake : public BaseModule
{
private:
	Rectf m_renderRegion;
	int m_hdMovDirection, m_rows, m_cols;
	float m_colSpacing, m_rowSpacing;
	std::list<std::pair<int, int> > m_points;
	std::list<int> m_movDirection;

	std::default_random_engine randomEngine;
	std::uniform_int_distribution<int> rowDistribution, colDistribution;

	Shader *m_pShader;
	GLuint m_glVertexBuf, m_glVertexArr, m_glVertexIdx;
	float *mp_vertexPtArr;
	int *mp_vertexIdxArr;

public:
	Snake(Rectf &renderRegion, int rows, int cols, int moveDirection, Shader *pShader);
	~Snake();

	void Reset();
	void InsertBlock();
	void move();

	bool CheckCollision();
	bool CheckInbound(int col, int row);

	void UpdateScene();

	void WillRender();
	void DidRender();
	void Render();

	virtual void onKeyDown(int key);
	virtual void onTimeout();
};

