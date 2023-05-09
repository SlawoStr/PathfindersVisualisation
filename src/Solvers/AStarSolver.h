#pragma once
#include "MeshSolver.h"
#include <queue>

/// <summary>
/// * A solver that uses the A* algorithm to find the shortest path from a starting point to a destination pointon a Mesh.
/// </summary>
class AStarSolver : public MeshSolver
{
	/// <summary>
	/// A functor that compares pairs of integers and floats based on their second element.
	/// </summary>
	struct CompareSecond {
		bool operator()(const std::pair<int, float>& a, const std::pair<int, float>& b) const {
			return a.second > b.second;
		}
	};
	using pairPrioQueue = std::priority_queue<std::pair<int, float>, std::vector<std::pair<int, float>>, CompareSecond>;
public:
	/// <summary>
	/// Constructs an AStarSolver object that will operate on the given Mesh
	/// </summary>
	/// <param name="mesh">A pointer to the Mesh object to operate on.</param>
	AStarSolver(Mesh* mesh);
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
	std::string getSolverName() override { return "A* Solver"; }
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
	pairPrioQueue m_cellContainer;		//!< A priority queue containing cells to be processed by the algorithm.
};