#pragma once

#include <list>
#include <utility>
#include <random>

#include "BaseModule.h"

enum SNAKE_MOVE_DIRECTION
{
	SNAKE_MOVE_DIRECTIION_UP,
	SNAKE_MOVE_DIRECTIION_DOWN,
	SNAKE_MOVE_DIRECTIION_LEFT,
	SNAKE_MOVE_DIRECTIION_RIGHT,
};


class Snake : public BaseModule
{
private:
	int m_length, m_moveDirection, m_rows, m_cols;
	float m_colSpacing, m_rowSpacing;
	std::list<std::pair<int, int> > m_points;

	std::default_random_engine randomEngine;
	std::uniform_int_distribution<int> rowDistribution, colDistribution;

	GLuint m_glVertexBuf, m_glVertexArr, m_glVertexIdx;
	float *mp_vertexPtArr;
	int *mp_vertexIdxArr;

public:
	Snake(int rows, int cols, int length, int moveDirection);
	~Snake();

	void InsertBlock(int start, int count);

	void UpdateScene();

	void Reset();

	void WillRender();
	void DidRender();
	void Render();

	virtual void onKeyDown(int key);
	virtual void onTimeout();

	void move();
	bool CheckCollision();
	bool CheckInbound(int row, int col);
};

