#pragma once
#include "src/Mesh/Mesh.h"

/// <summary>
/// Mapa index from 1D to 2D
/// </summary>
/// <param name="index">Index in 1D vector</param>
/// <param name="width">Width of array</param>
/// <returns>Index in 2D</returns>
inline std::pair<int, int> mapIndex2D(int index, int width) {
	int row = index / width;
	int col = index % width;
	return std::make_pair(row, col);
}

/// <summary>
/// Map index from 2D to 1D
/// </summary>
/// <param name="row">Row number</param>
/// <param name="col">Column number</param>
/// <param name="width">Width of array</param>
/// <returns>Index in 1D</returns>
inline int mapIndex1D(int row, int col, int width) {
	return row * width + col;
}

/// <summary>
/// Type of implemented solvers
/// </summary>
enum class SolverType
{
	DFS,
	BFS,
	DJIKSTRA,
	ASTAR,
};

class MeshSolver
{
public:
	MeshSolver(Mesh* mesh) : m_mesh{ mesh } {}
	/// <summary>
	/// Update visualiser ( update loop )
	/// </summary>
	void update()
	{
		m_iterNumber++;
		if (!m_isFinished)
		{
			if (!m_isFound)
			{
				updateSolver();
			}
			else
			{
				updatePath();
			}
		}
	}
	/// <summary>
	/// Reset solver
	/// </summary>
	virtual void reset() = 0;
	/// <summary>
	/// Is solver cleared
	/// </summary>
	/// <returns>True if solver is cleared</returns>
	bool isClear() { return !m_iterNumber; }
	/// <summary>
	/// Get number of processed squares
	/// </summary>
	/// <returns>Processed squares number</returns>
	int getProcessedSquares()const { return m_squareProcessed; }
	/// <summary>
	/// Get number of cells that create path
	/// </summary>
	/// <returns>Length of path</returns>
	int getPathLength() const { return m_pathLength; }
	/// <summary>
	/// Get path weight
	/// </summary>
	/// <returns>Path weight</returns>
	int getPathWeight() const { return m_pathWeight; }
	/// <summary>
	/// Get solver name
	/// </summary>
	/// <returns>Solver name</returns>
	virtual std::string getSolverName() = 0;
	/// <summary>
	/// Reset solver
	/// </summary>
	void resetSolver()
	{
		m_isFound = false;
		m_isFinished = false;
		m_squareProcessed = 0;
		m_pathLength = 0;
		m_pathWeight = 0;
		m_iterNumber = 0;
	}
	/// <summary>
	/// Reset starting position of solver
	/// </summary>
	virtual void resetStartPosition() = 0;
private:
	/// <summary>
	/// Run another step of pathfinding visualisation
	/// </summary>
	virtual void updateSolver() = 0;
	/// <summary>
	/// Run another step of path visualisation
	/// </summary>
	virtual void updatePath() = 0;
protected:
	Mesh* m_mesh;					//!< Pointer to mesh containing grid
	bool m_isFound{ false };		//!< Flag indicating whether a path has been found
	bool m_isFinished{ false };		//!< Flag indicating whether the algorithm has finished processing
	// Results
	int m_squareProcessed{};		//!< Number of cells processed by the algorithm
	int m_pathLength{};				//!< Length of the path (in cells) from the starting point to the destination point
	int m_pathWeight{};				//!< Total weight of the path (sum of weights of all cells in the path)
	int m_iterNumber{};				//!< Number of iterations (i.e., iterations of the main loop) performed by the algorithm
};