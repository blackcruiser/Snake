#include "stdafx.h"
#include "Snake.h"


Snake::Snake(int rows, int cols, int movDirection) :
	m_rows(rows), m_cols(cols), m_hdMovDirection(movDirection),
	rowDistribution(0, rows), colDistribution(0, cols)
{
	m_colSpacing = 2.0f / m_cols;
	m_rowSpacing = 2.0f / m_rows;
}


Snake::~Snake()
{
}


void Snake::Reset()
{
	m_points.clear();
	InsertBlock();
}

void Snake::InsertBlock()
{
	bool isSuccess = false;
	int row, col, blkIdx, movDirection;

	if (0 == m_points.size())
	{
		while (true)
		{
			row = rowDistribution(randomEngine);
			col = colDistribution(randomEngine);

			if (false == CheckInbound(col, row))
			{
				movDirection = m_hdMovDirection;
				isSuccess = true;
				break;
			}
		}
	}
	else if (0 < m_points.size())
	{
		col = m_points.back().first;
		row = m_points.back().second;
		movDirection = m_movDirection.back();

		//figure out where to add new block
		switch (movDirection)
		{
		case SNAKE_MOVE_DIRECTIION_UP:
			blkIdx = 2;
			break;
		case SNAKE_MOVE_DIRECTIION_RIGHT:
			blkIdx = 3;
			break;
		case SNAKE_MOVE_DIRECTIION_DOWN:
			blkIdx = 0;
			break;
		case SNAKE_MOVE_DIRECTIION_LEFT:
			blkIdx = 1;
			break;
		}

		for (int i = 0; i < 4; i++)
		{
			//try to add new block
			switch (blkIdx)
			{
			case 0:
				row++;
				movDirection = SNAKE_MOVE_DIRECTIION_DOWN;
				break;
			case 1:
				col++;
				movDirection = SNAKE_MOVE_DIRECTIION_LEFT;
				break;
			case 2:
				row--;
				movDirection = SNAKE_MOVE_DIRECTIION_UP;
				break;
			case 3:
				col--;
				movDirection = SNAKE_MOVE_DIRECTIION_RIGHT;
				break;
			}

			if (false == CheckInbound(col, row))
			{
				isSuccess = true;
				break;
			}

			blkIdx = (blkIdx + 1) % 4;
		}
	}
	
	if (true == isSuccess)
	{
		m_points.push_back(std::make_pair(col, row));
		m_movDirection.push_back(movDirection);
		UpdateScene();
	}
	else
	{
		//TO DO
		//unable to insert new block
	}
}



void Snake::move()
{
	int row, col;

	col = m_points.begin()->first;
	row = m_points.begin()->second;

	switch (m_hdMovDirection)
	{
	case SNAKE_MOVE_DIRECTIION_UP:
		row = row + 1;
		break;
	case SNAKE_MOVE_DIRECTIION_DOWN:
		row = row - 1;
		break;
	case SNAKE_MOVE_DIRECTIION_LEFT:
		col = col - 1;
		break;
	case SNAKE_MOVE_DIRECTIION_RIGHT:
		col = col + 1;
		break;
	}

	m_points.push_front(std::make_pair(col, row));
	m_movDirection.push_front(m_hdMovDirection);
	m_points.pop_back();
	m_movDirection.pop_back();
}


bool Snake::CheckCollision()
{
	int row, col;

	col = m_points.begin()->first;
	row = m_points.begin()->second;

	if (row < 0 || row >= m_rows || col < 0 || col >= m_cols)
		return true;

	for (auto it_point = ++m_points.begin(); it_point != m_points.end(); it_point++)
	{
		if (col == it_point->first && row == it_point->second)
			return true;
	}

	return false;
}

bool Snake::CheckInbound(int col, int row)
{
	if (row < 0 && row >= m_rows || col < 0 && col >= m_cols)
		return true;

	for (auto it_point = m_points.begin(); it_point != m_points.end(); it_point++)
	{
		if (col == it_point->first && row == it_point->second)
			return true;
	}
	return false;
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

		mp_vertexIdxArr[i * 6] = i * 4;
		mp_vertexIdxArr[i * 6 + 1] = i * 4 + 1;
		mp_vertexIdxArr[i * 6 + 2] = i * 4 + 2;
		mp_vertexIdxArr[i * 6 + 3] = i * 4 + 2;
		mp_vertexIdxArr[i * 6 + 4] = i * 4 + 3;
		mp_vertexIdxArr[i * 6 + 5] = i * 4 + 1;
	}
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
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_rows * m_cols * 8,
		0, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_glVertexIdx);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * m_rows * m_cols * 6,
		0, GL_DYNAMIC_DRAW);


	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
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
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindVertexArray(m_glVertexArr);

	glBindBuffer(GL_ARRAY_BUFFER, m_glVertexBuf);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * m_points.size() * 8, mp_vertexPtArr);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_glVertexIdx);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(int) * m_points.size() * 6, mp_vertexIdxArr);

	glDrawElements(GL_TRIANGLES, m_points.size() * 6, GL_UNSIGNED_INT, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glDisableClientState(GL_VERTEX_ARRAY);
}

void Snake::onKeyDown(int key)
{
	switch (key)
	{
	case GLFW_KEY_UP:
		m_hdMovDirection = SNAKE_MOVE_DIRECTIION_UP;
		break;
	case GLFW_KEY_DOWN:
		m_hdMovDirection = SNAKE_MOVE_DIRECTIION_DOWN;
		break;
	case GLFW_KEY_LEFT:
		m_hdMovDirection = SNAKE_MOVE_DIRECTIION_LEFT;
		break;
	case GLFW_KEY_RIGHT:
		m_hdMovDirection = SNAKE_MOVE_DIRECTIION_RIGHT;
		break;
	}
}

void Snake::onTimeout()
{
	move();
	UpdateScene();
}