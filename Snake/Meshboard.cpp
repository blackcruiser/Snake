#include "stdafx.h"
#include "Meshboard.h"

#include <iostream>


Meshboard::Meshboard(Rectf &renderRegion, int rows, int cols, Shader *pShader) :
	m_rows(rows), m_cols(cols), m_pShader(pShader),
	m_renderRegion(renderRegion)
{
	m_rowSpacing = m_renderRegion.height / m_rows;
	m_colSpacing = m_renderRegion.width / m_cols;
}

Meshboard::~Meshboard()
{}


void Meshboard::WillRender()
{
	m_pVertexPtArr = new float[(m_rows + m_cols + 2) * 4];

	for (int i = 0; i < m_rows + 1; i++)
	{
		m_pVertexPtArr[i * 4] = m_renderRegion.x;
		m_pVertexPtArr[i * 4 + 1] = i * m_rowSpacing + m_renderRegion.y;
		m_pVertexPtArr[i * 4 + 2] = m_renderRegion.x + m_renderRegion.width - 1;
		m_pVertexPtArr[i * 4 + 3] = i * m_rowSpacing + m_renderRegion.y;
	}

	for (int i = m_rows + 1; i < m_rows + m_cols + 2; i++)
	{
		m_pVertexPtArr[i * 4] = (i - m_rows - 1) * m_colSpacing  + m_renderRegion.x;
		m_pVertexPtArr[i * 4 + 1] = m_renderRegion.y;
		m_pVertexPtArr[i * 4 + 2] = (i - m_rows - 1) * m_colSpacing + m_renderRegion.x;
		m_pVertexPtArr[i * 4 + 3] = m_renderRegion.y + m_renderRegion.height - 1;
	}

	glGenVertexArrays(1, &m_glVertexArr);
	glGenBuffers(1, &m_glVertexBuf);
	GL_PRINT_ERROR;

	glBindVertexArray(m_glVertexArr);
	GL_PRINT_ERROR;

	glBindBuffer(GL_ARRAY_BUFFER, m_glVertexBuf);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * (m_rows + m_cols + 2) * 4,
		m_pVertexPtArr, GL_STATIC_DRAW);
	GL_PRINT_ERROR;

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);
	GL_PRINT_ERROR;

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	GL_PRINT_ERROR;
}

void Meshboard::DidRender()
{
	glDeleteVertexArrays(1, &m_glVertexArr);
	glDeleteBuffers(1, &m_glVertexBuf);

	delete[] m_pVertexPtArr;
}

void Meshboard::Render()
{
	m_pShader->Use();

	glBindVertexArray(m_glVertexArr); 
	glDrawArrays(GL_LINES, 0, (m_rows + m_cols + 2) * 2);
	glBindVertexArray(0);
}


