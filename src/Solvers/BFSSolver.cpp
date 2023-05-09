#include "BFSSolver.h"
#include <random>

////////////////////////////////////////////////////////////
BFSSolver::BFSSolver(Mesh* mesh) : MeshSolver(mesh)
{
	reset();
}

////////////////////////////////////////////////////////////
void BFSSolver::reset()
{
	resetStartPosition();
	resetSolver();
}

void BFSSolver::resetStartPosition()
{
	m_cellContainer = std::queue<int>();
	m_cellContainer.push(m_mesh->getStartPosition());
}

////////////////////////////////////////////////////////////
void BFSSolver::updateSolver()
{
	sf::Vector2i meshSize = m_mesh->getMeshSize();
	std::vector<Cell>& cellVec = m_mesh->getSquareVec();
	int topIndex = m_cellContainer.front();

	m_squareProcessed++;
	auto [row, column] = mapIndex2D(topIndex, meshSize.x);
	for (int i = -1; i <= 1; i++)
	{
		if (row + i < 0 || row + i >= meshSize.y)
		{
			continue;
		}
		for (int j = -1; j <= 1; j++)
		{
			if (i == 0 && j == 0)
				continue;
			if (column + j < 0 || column + j >= meshSize.x)
			{
				continue;
			}
			int nextIndex = mapIndex1D(row + i, column + j, meshSize.x);
			if (cellVec[nextIndex].cellType == SquareType::FINISH)
			{
				m_isFound = true;
				break;
			}
			else if (cellVec[nextIndex].cellType == SquareType::EMPTY)
			{
				cellVec[nextIndex].cellType = SquareType::QUEUED;
				cellVec[nextIndex].distance = cellVec[topIndex].distance + 1;
				m_mesh->setCellColor(SquareType::QUEUED, nextIndex);
				m_cellContainer.push(nextIndex);
			}
		}
		if (m_isFound)
		{
			break;
		}
	}
	// Mark current Cell as finished
	if (cellVec[topIndex].cellType != SquareType::START)
	{
		cellVec[topIndex].cellType = SquareType::PROCESSED;
		m_mesh->setCellColor(SquareType::PROCESSED, topIndex);
	}
	if (!m_isFound)
	{
		m_cellContainer.pop();
		if (m_cellContainer.empty())
		{
			m_isFinished = true;
		}
	}
	else
	{
		m_cellContainer = std::queue<int>();
		m_cellContainer.push(topIndex);
	}
}

////////////////////////////////////////////////////////////
void BFSSolver::updatePath()
{
	sf::Vector2i meshSize = m_mesh->getMeshSize();
	std::vector<Cell>& cellVec = m_mesh->getSquareVec();
	int topIndex = m_cellContainer.front();

	m_pathLength++;
	m_pathWeight += cellVec[topIndex].weight;
	std::vector<int> availableCells;
	auto [row, column] = mapIndex2D(topIndex, meshSize.x);
	for (int i = -1; i <= 1; i++)
	{
		if (row + i < 0 || row + i >= meshSize.y)
		{
			continue;
		}
		for (int j = -1; j <= 1; j++)
		{
			if (i == 0 && j == 0)
				continue;
			if (column + j < 0 || column + j >= meshSize.x)
			{
				continue;
			}
			int nextIndex = mapIndex1D(row + i, column + j, meshSize.x);
			if (cellVec[nextIndex].distance < cellVec[topIndex].distance)
			{
				availableCells.push_back(nextIndex);
			}
		}
	}
	if (availableCells.empty())
	{
		m_isFinished = true;
	}
	else
	{
		// Shufle cells vector (to avoid picking only 1 path)
		std::random_device rd;
		std::mt19937 gen(rd());
		std::shuffle(availableCells.begin(), availableCells.end(), gen);
		// First element in vector will create path
		cellVec[topIndex].cellType = SquareType::PATH;
		m_mesh->setCellColor(SquareType::PATH, topIndex);
		m_cellContainer.pop();
		m_cellContainer.push(availableCells[0]);
	}
}
