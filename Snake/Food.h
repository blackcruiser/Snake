#pragma once

#include <random>

#include "BaseModule.h"

class Food :
	public BaseModule
{
private:
	int m_cols, m_rows, m_col, m_row, m_colInterval, m_rowInterval;
	std::default_random_engine m_randomEngine;
	std::uniform_int_distribution<int> m_rowDistribution, m_colDistribution;

	float vertexPoint[8];

public:
	Food(int rows, int cols, int colInterval, int rowInterval);
	~Food();

	void Reset();
	void Render();
};

