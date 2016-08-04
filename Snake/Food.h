#pragma once

#include <random>

#include "BaseModule.h"
#include "Shader.h"

class Food :
	public BaseModule
{
private:
	Rectf m_renderRegion;
	int m_cols, m_rows, m_col, m_row;
	float m_colSpacing, m_rowSpacing;
	
	std::default_random_engine m_randomEngine;
	std::uniform_int_distribution<int> m_rowDistribution, m_colDistribution;

	Shader *m_pShader;
	GLuint m_glVertexBuf, m_glVertexArr, m_glVertexIdx;
	float m_vertexPtArr[8];
	int m_vertexIdxArr[6] = { 0, 1, 2, 2, 3, 1 };

public:
	Food(Rectf &renderRegion, int rows, int cols, Shader *pShader);
	~Food();

	int Row();
	int Col();
	
	void Reset();

	void WillRender();
	void DidRender();
	void Render();
};

