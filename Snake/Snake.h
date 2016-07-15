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
	std::list<std::pair<int, int> > m_points;

	std::default_random_engine randomEngine;
	std::uniform_int_distribution<int> rowDistribution, colDistribution;

		
	

public:
	Snake(int length, int rows, int cols, int moveDirection);
	~Snake();

	void Render();
	virtual void onKeyDown(int key);

	void move();
	bool CheckCollision();
	bool CheckInbound(int row, int col);
};

