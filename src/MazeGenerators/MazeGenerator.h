#pragma once
#include "src/Mesh/Mesh.h"

/// <summary>
/// Abstract class for maze generators
/// </summary>
class MazeGenerator
{
public:
	MazeGenerator(Mesh* mesh) : m_mesh{ mesh } {}
	/// <summary>
	/// Run another step of pathfinding visualisation
	/// </summary>
	virtual void updateGenerator() = 0;
	/// <summary>
	/// Return whether generator finished work
	/// </summary>
	/// <returns>Is finished</returns>
	virtual bool isFinished() = 0;
protected:
	Mesh* m_mesh; //!< Pointer to mesh containing grid
};