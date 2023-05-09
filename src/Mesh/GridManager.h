#pragma once
#include <list>
#include <memory>
#include "Mesh.h"
#include "src/Solvers/MeshSolver.h"
#include "src/MazeGenerators/MazeGenerator.h"

/// <summary>
/// The GridManager class manages a collection of Mesh objects and provides methods for drawing and adding meshes,
/// as well as toggling the display of squares and lines and handling mouse clicks.
/// </summary>
class GridManager
{
public:
	GridManager();
	/// <summary>
	/// Draws all meshes within the specified window bounds to the specified render target.
	/// </summary>
	/// <param name="target">The render target to draw to.</param>
	/// <param name="windowBound">The current camera view.</param>
	void draw(sf::RenderTarget& target, sf::FloatRect& windowBound);
	/// <summary>
	/// Adds a new Mesh object with the specified position, mesh size, and cell size to the Mesh vector and solver
	/// </summary>
	/// <param name="position">The position of the Mesh object.</param>
	/// <param name="meshSize">The size of the Mesh object.</param>
	/// <param name="cellSize">The size of each cell in the Mesh object.</param>
	/// <param name="solverType">Type of solver used for this mesh</param>
	void addMesh(sf::Vector2f position, sf::Vector2i meshSize, unsigned cellSize,SolverType solverType);
	/// <summary>
	/// Adds a new Mesh object with the specified position, mesh size, and cell size to the Mesh vector.
	/// </summary>
	/// <param name="position">The position of the Mesh object.</param>
	/// <param name="meshSize">The size of the Mesh object.</param>
	/// <param name="cellSize">The size of each cell in the Mesh object.</param>
	void addMesh(sf::Vector2f position, sf::Vector2i meshSize, unsigned cellSize);
	/// <summary>
	/// Toggles the drawing of squares on and off.
	/// </summary>
	void switchSquares() { drawSquares = !drawSquares; };
	/// <summary>
	/// Toggles the drawing of lines on and off.
	/// </summary>
	void switchLines() { drawLines = !drawLines; };
	/// <summary>
	/// Turn off drawing cell weights
	/// </summary>
	void switchWeights() { drawWeights = !drawWeights; };
	/// <summary>
	/// Handles a mouse click at the specified position, highlighting the cell at that position in the Mesh vector.
	/// </summary>
	/// <param name="mousePos">The position of the mouse click.</param>
	/// <param name="squareType">Type of cell</param>
	void setSquareType(sf::Vector2f mousePos, SquareType squareType);
	/// <summary>
	/// Change square weight (add or sub)
	/// </summary>
	/// <param name="mousePos">The position of the mouse click.</param>
	/// <param name="isPositive">Should add or sub</param>
	void changeSquareWeight(sf::Vector2f mousePos,bool isPositive);
	/// <summary>
	/// Copy elements from template mesh to other meshes
	/// </summary>
	void copyTemplateMesh();
	/// <summary>
	/// Update solvers
	/// </summary>
	void update();
	/// <summary>
	/// Generate maze on scratch maze
	/// </summary>
	void generateMaze();
private:
	/// <summary>
	/// Clear mesh
	/// </summary>
	/// <param name="index">Index of mesh</param>
	void clearMesh(int index);
private:
	std::list<Mesh> m_meshVec;							//!< The vector of Mesh objects managed by the GridManager.
	std::vector<std::unique_ptr<MeshSolver>> m_solvers; //!< The vector of mesh solvers (pathfinders)
	std::unique_ptr<MazeGenerator> m_mazeGenerator;		//!< Maze generator
	bool drawSquares{ true };							//!< A flag indicating whether to draw squares or not.
	bool drawLines{ true };								//!< A flag indicating whether to draw lines or not.
	bool drawWeights{ true };							//!< A flag indicating whether to draw cell weights or not
	sf::Font m_font;									//!< Font for drawing cells weights
};