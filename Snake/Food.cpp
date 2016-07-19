#include "stdafx.h"
#include "Food.h"


Food::Food(int rows, int cols):
	m_cols(cols), m_rows(rows), m_col(0), m_row(0), 
	m_colDistribution(0, cols), m_rowDistribution(0, rows)
{
	m_colSpacing = 2.0f / cols;
	m_rowSpacing = 2.0f / rows;
}


Food::~Food()
{
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
	int err;

	glGenVertexArrays(1, &m_glVertexArr);
	glGenBuffers(1, &m_glVertexBuf);
	glGenBuffers(1, &m_glVertexIdx);

	glBindVertexArray(m_glVertexArr);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_glVertexIdx);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_vertexIdxArr), m_vertexIdxArr, GL_STATIC_DRAW);
	err = glGetError();

	glBindBuffer(GL_ARRAY_BUFFER, m_glVertexBuf);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
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
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertexPtArr), m_vertexPtArr, GL_DYNAMIC_DRAW);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	int err = glGetError();
}