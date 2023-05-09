#include "DFSSolver.h"
#include <random>

////////////////////////////////////////////////////////////
DFSSolver::DFSSolver(Mesh* mesh) : MeshSolver(mesh)
{
	reset();
}

////////////////////////////////////////////////////////////
void DFSSolver::reset()
{
	resetStartPosition();
	resetSolver();
}

void DFSSolver::resetStartPosition()
{
	m_cellContainer = std::stack<int>();
	m_cellContainer.push(m_mesh->getStartPosition());
}

////////////////////////////////////////////////////////////
void DFSSolver::updateSolver()
{
	sf::Vector2i meshSize = m_mesh->getMeshSize();
	std::vector<Cell>& cellVec = m_mesh->getSquareVec();
	int topIndex = m_cellContainer.top();

	// Mark current cell
	if (cellVec[topIndex].cellType != SquareType::START)
	{
		cellVec[topIndex].cellType = SquareType::UNFINISHED;
		m_mesh->setCellColor(SquareType::UNFINISHED, topIndex);
	}
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
			if (cellVec[nextIndex].cellType == SquareType::FINISH)
			{
				m_isFound = true;
				break;
			}
			else if (cellVec[nextIndex].cellType == SquareType::EMPTY)
			{
				availableCells.push_back(nextIndex);
			}
		}
		if (m_isFound)
		{
			break;
		}
	}
	if (availableCells.empty())
	{
		m_squareProcessed++;
		if (cellVec[topIndex].cellType != SquareType::START)
		{
			cellVec[topIndex].cellType = SquareType::PROCESSED;
			m_mesh->setCellColor(SquareType::PROCESSED, topIndex);
		}
		m_cellContainer.pop();
		if (m_cellContainer.empty())
		{
			m_isFinished = true;
		}
	}
	else
	{
		// Shufle cells vector (to avoid picking only 1 path)
		std::random_device rd;
		std::mt19937 gen(rd());
		std::shuffle(availableCells.begin(), availableCells.end(), gen);
		for (auto val : availableCells)
		{
			cellVec[val].cellType = SquareType::QUEUED;
			m_mesh->setCellColor(SquareType::QUEUED, val);
			m_cellContainer.push(val);
		}
	}
}

////////////////////////////////////////////////////////////
void DFSSolver::updatePath()
{
	std::vector<Cell>& cellVec = m_mesh->getSquareVec();
	int topIndex = m_cellContainer.top();

	if (cellVec[topIndex].cellType == SquareType::UNFINISHED)
	{
		cellVec[topIndex].cellType = SquareType::PATH;
		m_mesh->setCellColor(SquareType::PATH, topIndex);
		m_pathLength++;
		m_pathWeight += cellVec[topIndex].weight;
	}
	m_cellContainer.pop();
	if (m_cellContainer.size() == 1)
	{
		m_isFinished = true;
	}
}
