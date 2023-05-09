#pragma once
#include "MeshSolver.h"
#include <stack>

/// <summary>
/// A solver that uses the depth-first search (DFS) algorithm to find a path from a starting point to a destination point on a Mesh.
/// </summary>
class DFSSolver : public MeshSolver
{
public:
	/// <summary>
	/// Constructs an DFSSolver object that will operate on the given Mesh
	/// </summary>
	/// <param name="mesh">A pointer to the Mesh object to operate on.</param>
	DFSSolver(Mesh* mesh);
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
	std::string getSolverName() override { return "DFSSolver"; }
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
	std::stack<int> m_cellContainer;	//!< A stack containing cells to be processed by the algorithm.
};