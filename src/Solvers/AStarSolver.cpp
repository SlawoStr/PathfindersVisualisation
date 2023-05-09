#include "AStarSolver.h"
#include <iostream>

/// <summary>
/// Heuristic function for A* algorithm
/// </summary>
/// <param name="x1">The x-coordinate of the current node</param>
/// <param name="y1">The y-coordinate of the current node</param>
/// <param name="x2">The x-coordinate of the target node</param>
/// <param name="y2">The y-coordinate of the target node</param>
/// <param name="weight">The weight of the edge connecting the current node and the target node</param>
/// <returns>The heuristic value based on the Euclidean distance between the two nodes and the weight provided</returns>
float heuristicFunction(float x1, float y1, float x2, float y2, float weight) {
	float dx = x2 - x1;
	float dy = y2 - y1;
	return std::sqrt(dx * dx + dy * dy) * (10 * weight);
}

////////////////////////////////////////////////////////////
AStarSolver::AStarSolver(Mesh* mesh) : MeshSolver(mesh)
{
	reset();
}

////////////////////////////////////////////////////////////
void AStarSolver::reset()
{
	resetStartPosition();
	resetSolver();
}

void AStarSolver::resetStartPosition()
{
	m_cellContainer = pairPrioQueue();
	m_cellContainer.push({ m_mesh->getStartPosition(),0.0f });
}

////////////////////////////////////////////////////////////
void AStarSolver::updateSolver()
{
	sf::Vector2i meshSize = m_mesh->getMeshSize();
	std::vector<Cell>& cellVec = m_mesh->getSquareVec();
	auto [topIndex, euWeight] = m_cellContainer.top();
	auto [finishX, finishY] = mapIndex2D(m_mesh->getFinishPosition(), meshSize.x);
	m_cellContainer.pop();
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

				m_cellContainer.push({ nextIndex,heuristicFunction(float(column + j), float(row + i), float(finishY),float(finishX),float(cellVec[nextIndex].weight))});
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
		if (m_cellContainer.empty())
		{
			m_isFinished = true;
		}
	}
	else
	{
		m_cellContainer = pairPrioQueue();
		m_cellContainer.push({ topIndex,static_cast<float>(cellVec[topIndex].distance) });
	}
}

////////////////////////////////////////////////////////////
void AStarSolver::updatePath()
{
	sf::Vector2i meshSize = m_mesh->getMeshSize();
	std::vector<Cell>& cellVec = m_mesh->getSquareVec();
	auto [topIndex, _] = m_cellContainer.top();
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

			if ((cellVec[nextIndex].cellType == SquareType::PROCESSED || cellVec[nextIndex].cellType == SquareType::START) && cellVec[nextIndex].distance < minDistance)
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
		m_cellContainer.push({ cellIndex,float(minDistance) });
	}
}
