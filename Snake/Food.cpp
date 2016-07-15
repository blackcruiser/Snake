#include "stdafx.h"
#include "Food.h"


Food::Food(int rows, int cols):
	m_cols(cols), m_rows(rows), m_col(-1), m_row(-1)
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