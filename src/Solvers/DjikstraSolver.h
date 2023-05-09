#pragma once
#include "MeshSolver.h"
#include <queue>

/// <summary>
/// A solver that uses the Djikstra algorithm to find a path from a starting point to a destination point on a Mesh.
/// </summary>
class DjikstraSolver : public MeshSolver
{
	// Comparator struct to sort pairs in the priority queue by their second element in ascending order
	struct CompareSecond {
		bool operator()(const std::pair<int, int>& a, const std::pair<int, int>& b) const {
			return a.second > b.second;
		}
	};
	using pairPrioQueue = std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, CompareSecond>;
public:
	DjikstraSolver(Mesh* mesh);
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
	std::string getSolverName() override { return "DJIKSTRA Solver"; }
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
	pairPrioQueue m_cellContainer;		//!< The priority queue of cell IDs and their distances from the starting point
};