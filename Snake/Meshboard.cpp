#include "stdafx.h"
#include "Meshboard.h"


Meshboard::Meshboard(int width, int height, int rows, int cols) :
	m_width(width), m_height(height), m_rows(rows), m_cols(cols)
{
	m_rowSpacing = m_height / m_rows;
	m_colSpacing = m_width / m_cols;
}


Meshboard::~Meshboard()
{
}

void Meshboard::Render()
{

}
