#pragma once

#include "BaseModule.h"

class Meshboard
{
private:
	int m_width, m_height, m_cols, m_rows;
	int m_rowSpacing, m_colSpacing;

public:
	Meshboard(int width, int height, int rows, int cols);
	~Meshboard();

	void Render();
};

