#include "stdafx.h"
#include "Snake.h"
#include "TextureManager.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>


Snake::Snake(Rectf &renderRegion, int rows, int cols, int movDirection, Shader *pShader) :
	m_rows(rows), m_cols(cols), m_hdMovDirection(movDirection),
	rowDistribution(0, rows), colDistribution(0, cols),
	m_renderRegion(renderRegion), m_pShader(pShader),
	isUpdated(false)
{
	m_colSpacing = m_renderRegion.width / m_cols;
	m_rowSpacing = m_renderRegion.height / m_rows;
}


Snake::~Snake()
{
	delete m_pShader;
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
		isUpdated = true;
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

	//if moving direction is opposed to head direction
	if (m_points.size() > 1 && 2 == (m_hdMovDirection - m_movDirection.front() + 4) % 4)
		m_hdMovDirection = m_movDirection.front();

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
	isUpdated = true;
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


void Snake::UpdateModel()
{
	int col, row, idx, i = 0;

	for (auto iter_point = m_points.begin(); iter_point != m_points.end(); iter_point++, i++)
	{
		col = iter_point->first;
		row = iter_point->second;
		idx = i % 35;

		m_pVtxPtArr[i * 16] = col * m_colSpacing + m_renderRegion.x;
		m_pVtxPtArr[i * 16 + 1] = row * m_rowSpacing + m_renderRegion.y;
		m_pVtxPtArr[i * 16 + 2] = (idx % 5) * 120 / 600.0f;
		m_pVtxPtArr[i * 16 + 3] = (idx / 5) * 120 / 840.0f;

		m_pVtxPtArr[i * 16 + 4] = (col + 1) * m_colSpacing + m_renderRegion.x;
		m_pVtxPtArr[i * 16 + 5] = row * m_rowSpacing + m_renderRegion.y;
		m_pVtxPtArr[i * 16 + 6] = ((idx % 5) * 120 + 120) / 600.0f;
		m_pVtxPtArr[i * 16 + 7] = (idx / 5) * 120 / 840.0f;

		m_pVtxPtArr[i * 16 + 8] = col * m_colSpacing + m_renderRegion.x;
		m_pVtxPtArr[i * 16 + 9] = (row + 1) * m_rowSpacing + m_renderRegion.y;
		m_pVtxPtArr[i * 16 + 10] = (idx % 5) * 120 / 600.0f;
		m_pVtxPtArr[i * 16 + 11] = ((idx / 5) * 120 + 120) / 840.0f;

		m_pVtxPtArr[i * 16 + 12] = (col + 1) * m_colSpacing + m_renderRegion.x;
		m_pVtxPtArr[i * 16 + 13] = (row + 1) * m_rowSpacing + m_renderRegion.y;
		m_pVtxPtArr[i * 16 + 14] = ((idx % 5) * 120 + 120) / 600.0f;
		m_pVtxPtArr[i * 16 + 15] = ((idx / 5) * 120 + 120) / 840.0f;

		m_pVtxIdxArr[i * 6] = i * 4;
		m_pVtxIdxArr[i * 6 + 1] = i * 4 + 1;
		m_pVtxIdxArr[i * 6 + 2] = i * 4 + 2;
		m_pVtxIdxArr[i * 6 + 3] = i * 4 + 2;
		m_pVtxIdxArr[i * 6 + 4] = i * 4 + 3;
		m_pVtxIdxArr[i * 6 + 5] = i * 4 + 1;
	}
}


void Snake::WillRender()
{
	m_pVtxPtArr = new float[m_cols * m_rows * 4 * 4];
	m_pVtxIdxArr = new int[m_cols * m_rows * 6];

	glActiveTexture(GL_TEXTURE0);
	TextureManager::Inst()->LoadTexture("./texture.jpg", 0, GL_BGR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	GL_PRINT_ERROR;
	
	glGenVertexArrays(1, &m_glVtxArr);
	glGenBuffers(1, &m_glVtxBuf);
	glGenBuffers(1, &m_glVtxIdxBuf);
	GL_PRINT_ERROR;

	glBindVertexArray(m_glVtxArr);

	glBindBuffer(GL_ARRAY_BUFFER, m_glVtxBuf);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_rows * m_cols * 16,
		0, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_glVtxIdxBuf);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * m_rows * m_cols * 6,
		0, GL_DYNAMIC_DRAW);
	GL_PRINT_ERROR;

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid *)(2 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	GL_PRINT_ERROR;

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	GL_PRINT_ERROR;
}

void Snake::DidRender()
{
	glDeleteVertexArrays(1, &m_glVtxArr);
	glDeleteBuffers(1, &m_glVtxIdxBuf);
	glDeleteBuffers(1, &m_glVtxBuf);

	TextureManager::Inst()->UnloadTexture(0);

	delete[] m_pVtxIdxArr;
	delete[] m_pVtxPtArr;
}

void Snake::Render()
{
	m_pShader->Use();

	if (true == isUpdated)
	{
		UpdateModel();
		isUpdated;
	}

	glActiveTexture(GL_TEXTURE0);
	TextureManager::Inst()->BindTexture(0);
	glUniform1i(glGetUniformLocation(m_pShader->program, "smpTexture"), 0);
	GL_PRINT_ERROR;

	glBindVertexArray(m_glVtxArr);

	glBindBuffer(GL_ARRAY_BUFFER, m_glVtxBuf);
	glBufferSubData(GL_ARRAY_BUFFER, 0, m_points.size() * sizeof(float) * 16, m_pVtxPtArr);
	GL_PRINT_ERROR;

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_glVtxIdxBuf);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, m_points.size() * sizeof(int) * 6, m_pVtxIdxArr);
	GL_PRINT_ERROR;

	glDrawElements(GL_TRIANGLES, m_points.size() * 6, GL_UNSIGNED_INT, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	GL_PRINT_ERROR;
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
}