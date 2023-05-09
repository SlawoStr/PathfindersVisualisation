#pragma once
#include "MeshSolver.h"
#include <queue>

/// <summary>
/// A solver that uses the breadth-first search (BFS) algorithm to find the shortest path from a starting point to a destination point on a Mesh.
/// </summary>
class BFSSolver : public MeshSolver
{
public:
	/// <summary>
	/// Constructs an BFSSolver object that will operate on the given Mesh
	/// </summary>
	/// <param name="mesh">A pointer to the Mesh object to operate on.</param>
	BFSSolver(Mesh* mesh);
	/// <summary>
	/// Resets the solver to its initial state
	/// </summary>
	void reset() override;
	/// <summary>
	/// Reset starting position of solver;
	/// </summary>
	void resetStartPosition() override;
	/// <summary>
	/// Returns a string representing the name of the solver.
	/// </summary>
	/// <returns></returns>
	std::string getSolverName() override { return "BFSSolver"; }
private:
	/// <summary>
	/// Updates the solver by performing one iteration of the A* algorithm.
	/// This method is called repeatedly until the algorithm finds the destination point or determines that no path exists.
	/// </summary>
	virtual void updateSolver() override;
	/// <summary>
	/// Updates the path after the algorithm has finished.
	/// This method is called once the destination point has been reached
	/// </summary>
	virtual void updatePath() override;
private:
	std::queue<int> m_cellContainer;	 //!< A queue containing cells to be processed by the algorithm.
};