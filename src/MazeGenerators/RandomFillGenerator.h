#pragma once
#include "MazeGenerator.h"
#include <random>

/// <summary>
/// Maze Generator - Use random numbers to decide whether cell should stay empty or be filled with wall
/// </summary>
class RandomFillGenerator : public MazeGenerator
{
public:
	RandomFillGenerator(Mesh* mesh);
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
	// Random generation
	std::mt19937 m_generator;								//!< Random number generator
	std::uniform_real_distribution<float> m_distribution;	//!< Random float distribution
	// Other
	int m_currentIndex{};									//!< Currently processed cell
	bool m_isFinished{ false };								//!< Is generation finished 
	const float WALL_PROBABILITY = 0.33f;					//!< Probability of filling cell with wall
};