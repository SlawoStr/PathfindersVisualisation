#pragma once
#include "MazeGenerator.h"
#include <random>
#include <queue>

/// <summary>
/// Create maze using recursive division
/// </summary>
class RecursiveDivisionGenerator : public MazeGenerator
{
public:
	RecursiveDivisionGenerator(Mesh* mesh);
	/// <summary>
	/// Update next step of generator
	/// </summary>
	void updateGenerator() override;
	/// <summary>
	/// Is maze generation finished
	/// </summary>
	/// <returns></returns>
	bool isFinished() override;
private:
	/// <summary>
	/// Generate next maze wall and next segments
	/// </summary>
	/// <param name="x">Starting position X</param>
	/// <param name="y">Starting position Y</param>
	/// <param name="sectionWidth">Width of section</param>
	/// <param name="sectionHeight">Height of section</param>
	/// <param name="orientation">Wall orientation</param>
	void divideMaze(int x, int y, int sectionWidth, int sectionHeight, int orientation);
	/// <summary>
	/// Get orientation on next wall
	/// </summary>
	/// <param name="width">Section width</param>
	/// <param name="height">Section height</param>
	/// <returns>Wall orientation</returns>
	int getOrientation(int width, int height);
	/// <summary>
	/// Generate random in in range[min,max]
	/// </summary>
	/// <param name="min">Minimum value</param>
	/// <param name="max">Maximum value</param>
	/// <returns>Random in in given range</returns>
	int getRandomInt(int min, int max);
private:
	std::mt19937 m_generator;												//!< Random number generator
	bool m_isFinished{ false };												//!< Is generation finished 
	// Visualisation
	std::queue<int> m_wallIndexes;											//!< Indexes of walls to draw
	std::queue<std::tuple<int, int, int, int, int>> m_divisionSegments;		//!< Division segments to process
};
