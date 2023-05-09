#include "DjikstraSolver.h"
#include <limits>

////////////////////////////////////////////////////////////
DjikstraSolver::DjikstraSolver(Mesh* mesh) : MeshSolver(mesh)
{
	reset();
}

////////////////////////////////////////////////////////////
void DjikstraSolver::reset()
{
	resetStartPosition();
	resetSolver();
}

void DjikstraSolver::resetStartPosition()
{
	m_cellContainer = pairPrioQueue();
	m_cellContainer.push({ m_mesh->getStartPosition(),0 });
}

////////////////////////////////////////////////////////////
void DjikstraSolver::updateSolver()
{
	sf::Vector2i meshSize = m_mesh->getMeshSize();
	std::vector<Cell>& cellVec = m_mesh->getSquareVec();
	auto [topIndex, _] = m_cellContainer.top();

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
				cellVec[nextIndex].distance = cellVec[topIndex].distance + cellVec[nextIndex].weight;
				m_mesh->setCellColor(SquareType::QUEUED, nextIndex);
				m_cellContainer.push({ nextIndex,cellVec[nextIndex].distance });
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
		m_cellContainer = pairPrioQueue();
		m_cellContainer.push({ topIndex,cellVec[topIndex].distance });
	}
}

////////////////////////////////////////////////////////////
void DjikstraSolver::updatePath()
{
	sf::Vector2i meshSize = m_mesh->getMeshSize();
	std::vector<Cell>& cellVec = m_mesh->getSquareVec();
	auto [topIndex, distance] = m_cellContainer.top();

	m_pathLength++;
	m_pathWeight += cellVec[topIndex].weight;

	auto [row, column] = mapIndex2D(topIndex, meshSize.x);
	int minDistance{ std::numeric_limits<int>::max() };
	int cellIndex{};
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
			if (cellVec[nextIndex].distance < minDistance)
			{
				cellIndex = nextIndex;
				minDistance = cellVec[nextIndex].distance;
			}
		}
	}
	if (cellVec[topIndex].distance == 0)
	{
		m_isFinished = true;
	}
	else
	{
		cellVec[topIndex].cellType = SquareType::PATH;
		m_mesh->setCellColor(SquareType::PATH, topIndex);
		m_cellContainer.pop();
		m_cellContainer.push({ cellIndex,minDistance });
	}
}
