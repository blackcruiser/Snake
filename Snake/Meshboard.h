#pragma once

#include "BaseModule.h"
#include "Shader.h"

class Meshboard
{
private:
	Rectf m_renderRegion;
	int m_width, m_height, m_cols, m_rows;
	float m_rowSpacing, m_colSpacing;

	Shader *m_pShader;
	GLuint m_glVertexArr, m_glVertexBuf;
	float *m_pVertexPtArr;

public:
	Meshboard(Rectf &renderRegion, int rows, int cols, Shader *pShader);
	~Meshboard();

	void WillRender();
	void DidRender();
	void Render();
};

