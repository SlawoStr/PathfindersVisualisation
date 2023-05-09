#include "RandomFillGenerator.h"

////////////////////////////////////////////////////////////
RandomFillGenerator::RandomFillGenerator(Mesh* mesh) : MazeGenerator(mesh), m_generator(std::random_device()()), m_distribution(0.f, 1.0f)
{
}

////////////////////////////////////////////////////////////
void RandomFillGenerator::updateGenerator()
{
	std::vector<Cell>& cellVec = m_mesh->getSquareVec();
	int cellNumber = static_cast<int>(cellVec.size());
	if (!m_isFinished)
	{
		if (cellVec[m_currentIndex].cellType == SquareType::EMPTY)
		{
			if (m_distribution(m_generator) < WALL_PROBABILITY)
			{
				cellVec[m_currentIndex].cellType = SquareType::FULL;
				m_mesh->setCellColor(SquareType::FULL, m_currentIndex);
			}
		}
		m_currentIndex++;
		if (m_currentIndex == cellNumber)
		{
			m_isFinished = true;
		}
	}
}

////////////////////////////////////////////////////////////
bool RandomFillGenerator::isFinished()
{
	return m_isFinished;
}
