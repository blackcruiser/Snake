#pragma once

#include <random>

#include "BaseModule.h"

class Food :
	public BaseModule
{
private:
	int m_cols, m_rows, m_col, m_row;
	float m_colSpacing, m_rowSpacing;
	
	std::default_random_engine m_randomEngine;
	std::uniform_int_distribution<int> m_rowDistribution, m_colDistribution;

	GLuint m_glVertexBuf, m_glVertexArr, m_glVertexIdx;
	float m_vertexPtArr[8];
	int m_vertexIdxArr[6] = { 0, 1, 2, 2, 3, 1 };

public:
	Food(int rows, int cols);
	~Food();

	void Reset();

	void WillRender();
	void DidRender();
	void Render();
};

