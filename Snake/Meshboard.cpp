#include "stdafx.h"
#include "Meshboard.h"


Meshboard::Meshboard(int rows, int cols) :
	m_rows(rows), m_cols(cols)
{
	m_rowSpacing = 2.0f / m_rows;
	m_colSpacing = 2.0f / m_cols;
}


Meshboard::~Meshboard()
{
}

void Meshboard::WillRender()
{
	mp_vertexPtArr = new float[(m_rows + m_cols + 2) * 4];

	for (int i = 0; i < m_rows + 1; i++)
	{
		mp_vertexPtArr[i * 4] = -1.0f;
		mp_vertexPtArr[i * 4 + 1] = i * m_rowSpacing - 1.0f;
		mp_vertexPtArr[i * 4 + 2] = 1.0f;
		mp_vertexPtArr[i * 4 + 3] = i * m_rowSpacing - 1.0f;
	}

	for (int i = m_rows + 1; i < m_rows + m_cols + 2; i++)
	{
		mp_vertexPtArr[i * 4] = (i - m_rows - 1) * m_colSpacing  - 1.0f;
		mp_vertexPtArr[i * 4 + 1] = -1.0f;
		mp_vertexPtArr[i * 4 + 2] = (i - m_rows - 1) * m_colSpacing - 1.0f;
		mp_vertexPtArr[i * 4 + 3] = 1.0f;
	}

	glGenVertexArrays(1, &m_glVertexArr);
	glGenBuffers(1, &m_glVertexBuf);

	glBindVertexArray(m_glVertexArr);

	glBindBuffer(GL_ARRAY_BUFFER, m_glVertexBuf);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * (m_rows + m_cols + 2) * 4,
		mp_vertexPtArr, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Meshboard::DidRender()
{
	glDeleteVertexArrays(1, &m_glVertexArr);
	glDeleteBuffers(1, &m_glVertexBuf);

	delete[] mp_vertexPtArr;
}

void Meshboard::Render()
{
	glBindVertexArray(m_glVertexArr);

	glDrawArrays(GL_LINES, 0, (m_rows + m_cols + 2) * 2);

	glBindVertexArray(0);
}


