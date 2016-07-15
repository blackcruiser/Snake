#include "stdafx.h"
#include "Snake.h"


Snake::Snake(int length, int rows, int cols, int moveDirection):
	m_rows(rows), m_cols(cols), m_length(length), m_moveDirection(moveDirection), rowDistribution(0, rows), colDistribution(0, cols)
{
	m_points.clear();
	m_points.push_back(std::make_pair(rowDistribution(randomEngine), colDistribution(randomEngine)));
}


Snake::~Snake()
{
}

void Snake::Render()
{

}

void Snake::onKeyDown(int key)
{
	switch (key)
	{
	case GLFW_KEY_UP:
		m_moveDirection = SNAKE_MOVE_DIRECTIION_UP;
		break;
	case GLFW_KEY_DOWN:
		m_moveDirection = SNAKE_MOVE_DIRECTIION_DOWN;
		break;
	case GLFW_KEY_LEFT:
		m_moveDirection = SNAKE_MOVE_DIRECTIION_LEFT;
		break;
	case GLFW_KEY_RIGHT:
		m_moveDirection = SNAKE_MOVE_DIRECTIION_RIGHT;
		break;
	}
}

void Snake::move()
{
	int row, col;

	m_points.pop_back();

	row = m_points.begin()->first;
	col = m_points.begin()->second;

	switch (m_moveDirection)
	{
	case SNAKE_MOVE_DIRECTIION_UP:
		row = row - 1;
		break;
	case SNAKE_MOVE_DIRECTIION_DOWN:
		row = row + 1;
		break;
	case SNAKE_MOVE_DIRECTIION_LEFT:
		col = col - 1;
		break;
	case SNAKE_MOVE_DIRECTIION_RIGHT:
		col = col + 1;
		break;
	}

	m_points.push_back(std::make_pair(row, col));
}

bool Snake::CheckCollision()
{
	int row, col;

	row = m_points.begin()->first;
	col = m_points.begin()->second;

	if (row < 0 && row >= m_rows || col < 0 && col >= m_cols)
		return true;

	for (auto it_point = ++m_points.begin(); it_point != m_points.end(); it_point++)
	{
		if (row == it_point->first && col == it_point->second)
			return true;
	}

	return false;
}

bool Snake::CheckInbound(int row, int col)
{
	for (auto it_point = m_points.begin(); it_point != m_points.end(); it_point++)
	{
		if (row == it_point->first && col == it_point->second)
			return true;
	}
	return false;
}