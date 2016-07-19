#include "stdafx.h"
#include "Snake.h"


Snake::Snake(int rows, int cols, int length, int moveDirection):
	m_rows(rows), m_cols(cols), m_length(length), m_moveDirection(moveDirection), rowDistribution(0, rows), colDistribution(0, cols)
{
	m_colSpacing = 2.0f / m_cols;
	m_rowSpacing = 2.0f / m_rows;
}


Snake::~Snake()
{
}

void Snake::InsertBlock(int start, int count)
{
	int row, col;

	row = rowDistribution(randomEngine);
	col = colDistribution(randomEngine);

	m_points.push_back(std::make_pair(col, row));
}

void Snake::UpdateScene()
{
	int col, row, i = 0;

	for (auto iter_point = m_points.begin(); iter_point != m_points.end(); iter_point++, i++)
	{
		col = iter_point->first;
		row = iter_point->second;

		mp_vertexPtArr[i * 8] = col * m_colSpacing - 1.0f;
		mp_vertexPtArr[i * 8 + 1] = row * m_rowSpacing - 1.0f;
		mp_vertexPtArr[i * 8 + 2] = (col + 1) * m_colSpacing - 1.0f;
		mp_vertexPtArr[i * 8 + 3] = row * m_rowSpacing - 1.0f;
		mp_vertexPtArr[i * 8 + 4] = col * m_colSpacing - 1.0f;
		mp_vertexPtArr[i * 8 + 5] = (row + 1) * m_rowSpacing - 1.0f;
		mp_vertexPtArr[i * 8 + 6] = (col + 1) * m_colSpacing - 1.0f;
		mp_vertexPtArr[i * 8 + 7] = (row + 1) * m_rowSpacing - 1.0f;

		mp_vertexIdxArr[i * 6] = i * 8;
		mp_vertexIdxArr[i * 6 + 1] = i * 8 + 1;
		mp_vertexIdxArr[i * 6 + 2] = i * 8 + 2;
		mp_vertexIdxArr[i * 6 + 3] = i * 8 + 2;
		mp_vertexIdxArr[i * 6 + 4] = i * 8 + 3;
		mp_vertexIdxArr[i * 6 + 5] = i * 8 + 1;
	}
}

void Snake::Reset()
{
	m_points.clear();

	InsertBlock(0, 1);
	UpdateScene();
}

void Snake::WillRender()
{
	mp_vertexPtArr = new float[m_cols * m_rows * 4 * 2];
	mp_vertexIdxArr = new int[m_cols * m_rows * 6];

	glGenVertexArrays(1, &m_glVertexArr);
	glGenBuffers(1, &m_glVertexBuf);
	glGenBuffers(1, &m_glVertexIdx);

	glBindVertexArray(m_glVertexArr);

	glBindBuffer(GL_ARRAY_BUFFER, m_glVertexBuf);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_glVertexBuf);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	int err = glGetError();
}

void Snake::DidRender()
{
	glDeleteVertexArrays(1, &m_glVertexArr);
	glDeleteBuffers(1, &m_glVertexIdx);
	glDeleteBuffers(1, &m_glVertexBuf);

	delete[] mp_vertexIdxArr;
	delete[] mp_vertexPtArr;
}

void Snake::Render()
{
	glBindVertexArray(m_glVertexArr);

	glBindBuffer(GL_ARRAY_BUFFER, m_glVertexBuf);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_length * 8,
		mp_vertexPtArr, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_glVertexIdx);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * m_length * 6,
		mp_vertexIdxArr, GL_DYNAMIC_DRAW);

	glDrawElements(GL_TRIANGLES, m_length * 6, GL_UNSIGNED_INT, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	int err = glGetError();
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

void Snake::onTimeout()
{
	move();
	UpdateScene();
}

void Snake::move()
{
	int row, col;

	col = m_points.begin()->first;
	row = m_points.begin()->second;

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

	m_points.push_front(std::make_pair(col, row));
	m_points.pop_back();
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