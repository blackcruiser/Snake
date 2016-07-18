#include "stdafx.h"
#include "Food.h"


Food::Food(int rows, int cols, int colInterval, int rowInterval):
	m_cols(cols), m_rows(rows), m_col(-1), m_row(-1), m_colInterval(colInterval), m_rowInterval(rowInterval)
{
}


Food::~Food()
{
}

void Food::Reset()
{
	m_row = m_rowDistribution(m_randomEngine);
	m_col = m_colDistribution(m_randomEngine);
}

void Food::Render()
{
	GLuint glVertexBuffer;
	GLuint glVertexArray;

	glGenVertexArrays(0, &glVertexArray);
	glGenBuffers(0, &glVertexBuffer);

	glBindVertexArray(glVertexArray);
	glBindBuffer(GL_ARRAY_BUFFER, glVertexBuffer);

	vertexPoint[0] = (m_row - 1) * m_rowInterval;
	vertexPoint[1] = (m_col - 1) * m_colInterval;
	vertexPoint[2] = (m_row - 1) * m_rowInterval;
	vertexPoint[3] = (m_col) * m_colInterval;
	vertexPoint[4] = (m_row) * m_rowInterval;
	vertexPoint[5] = (m_col - 1) * m_colInterval;
	vertexPoint[6] = (m_row) * m_rowInterval;
	vertexPoint[7] = (m_col) * m_colInterval;

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPoint), vertexPoint, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);
}