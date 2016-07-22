#include "stdafx.h"
#include "Food.h"


Food::Food(int rows, int cols):
	m_cols(cols), m_rows(rows), m_col(0), m_row(0), 
	m_colDistribution(0, cols - 1), m_rowDistribution(0, rows - 1)
{
	m_colSpacing = 2.0f / cols;
	m_rowSpacing = 2.0f / rows;
}


Food::~Food()
{
}

int Food::Row()
{
	return m_row;
}

int Food::Col()
{
	return m_col;
}

void Food::Reset()
{
	m_row = m_rowDistribution(m_randomEngine);
	m_col = m_colDistribution(m_randomEngine);

	m_vertexPtArr[0] = (m_col + 0.25) * m_colSpacing;
	m_vertexPtArr[1] = (m_row + 0.25)* m_rowSpacing;
	m_vertexPtArr[2] = (m_col + 0.75) * m_colSpacing;
	m_vertexPtArr[3] = (m_row + 0.25) * m_rowSpacing;
	m_vertexPtArr[4] = (m_col + 0.25) * m_colSpacing;
	m_vertexPtArr[5] = (m_row + 0.75) * m_rowSpacing;
	m_vertexPtArr[6] = (m_col + 0.75) * m_colSpacing;
	m_vertexPtArr[7] = (m_row + 0.75) * m_rowSpacing;

	for (int i = 0; i < 8; i++)
		m_vertexPtArr[i] -= 1.0f;
}

void Food::WillRender()
{
	//glGenVertexArrays(1, &m_glVertexArr);
	glGenBuffers(1, &m_glVertexBuf);
	//glGenBuffers(1, &m_glVertexIdx);

	/*
	glBindVertexArray(m_glVertexArr);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_glVertexIdx);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_vertexIdxArr), m_vertexIdxArr, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, m_glVertexBuf);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 8, NULL, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	*/
	glDeleteBuffers(0, &m_glVertexBuf);

	int err = glGetError();
}

void Food::DidRender()
{
	glDeleteVertexArrays(1, &m_glVertexArr);
	glDeleteBuffers(1, &m_glVertexBuf);
	glDeleteBuffers(1, &m_glVertexIdx);
}

void Food::Render()
{
	glBindVertexArray(m_glVertexArr);

	glBindBuffer(GL_ARRAY_BUFFER, m_glVertexBuf);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 8, m_vertexPtArr);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}