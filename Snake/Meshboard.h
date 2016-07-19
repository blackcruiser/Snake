#pragma once

#include "BaseModule.h"

class Meshboard
{
private:
	int m_width, m_height, m_cols, m_rows;
	float m_rowSpacing, m_colSpacing;

	GLuint m_glVertexArr, m_glVertexBuf;

	float *mp_vertexPtArr;

public:
	Meshboard(int rows, int cols);
	~Meshboard();

	void WillRender();
	void DidRender();
	void Render();
};

