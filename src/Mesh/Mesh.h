#pragma once
#include <SFML/Graphics.hpp>
#include <limits>

/// <summary>
/// Type of squares on mesh
/// </summary>
enum class SquareType
{
	EMPTY,
	START,
	FINISH,
	QUEUED,
	UNFINISHED,
	PROCESSED,
	PROCESSED2,
	PATH,
	FULL
};

/// <summary>
/// Representation of each cell in mesh
/// </summary>
struct Cell
{
	SquareType cellType{ SquareType::EMPTY };					//!< Type of cell
	int		   distance{ std::numeric_limits<int>::max() };		//!< Distance from start
	int		   weight{ 1 };										//!< Cell weight
};

class Mesh
{
public:
	/// <summary>
	/// Create mesh 
	/// </summary>
	/// <param name="position">Mesh position on world map</param>
	/// <param name="meshSize">Number of cells (X,Y)</param>
	/// <param name="squareSize">Size of each square</param>
	Mesh(sf::Vector2f position, sf::Vector2i meshSize, unsigned cellSize);
	/// <summary>
	/// Draw mesh
	/// </summary>
	/// <param name="target">Target to draw</param>
	void draw(sf::RenderTarget& target, const sf::Font& font, bool drawSquares, bool drawLines, bool drawWeights);
	/// <summary>
	/// Draw solver results
	/// </summary>
	void drawData(sf::RenderTarget& target, const sf::Font& font, std::string solverName, int processedSquare, int pathLength, int pathWeight);
	/// <summary>
	/// Check if mesh is visible in current view
	/// </summary>
	/// <param name="viewBox">View box containig left top point and size of current view</param>
	/// <returns>Is visisble in view</returns>
	bool isVisible(sf::FloatRect& windowBound);
	/// <summary>
	/// Check if mouse position is inside mesh
	/// </summary>
	/// <param name="mousePos">Mouse position</param>
	/// <returns>Is in range</returns>
	bool isInRange(sf::Vector2f mousePos);
	/// <summary>
	/// Activate cell
	/// </summary>
	/// <param name="mousePos">Position of cell to activate (mouse click pos)</param>
	/// <param name="squareType">Type of cell</param>
	void setCellType(sf::Vector2f mousePos,SquareType squareType);
	/// <summary>
	/// Add weight to cell
	/// </summary>
	/// <param name="mousePos">Position of cell to activate (mouse click pos)</param>
	void addCellWeight(sf::Vector2f mousePos);
	/// <summary>
	/// Sub weight from cell
	/// </summary>
	/// <param name="mousePos">Position of cell to activate (mouse click pos)</param>
	void subCellWeight(sf::Vector2f mousePos);
	/// <summary>
	/// Copy Mesh 
	/// </summary>
	/// <param name="mesh">Type of cell</param>
	void copyMesh(const Mesh& mesh);
	/// <summary>
	/// Reset mesh (Clear all cells changed my solver)
	/// </summary>
	void resetMesh();
	/// <summary>
	/// Restore mesh to its initial state
	/// </summary>
	void clearMesh();
	/// <summary>
	/// Change color of a cell with index
	/// </summary>
	/// <param name="sqType">New type of cell</param>
	/// <param name="index">Index of cell</param>
	void setCellColor(SquareType sqType, int index);
	/// <summary>
	/// Get size of mesh
	/// </summary>
	/// <returns>Size of mesh</returns>
	sf::Vector2i getMeshSize()const { return m_size; }
	/// <summary>
	/// Return start position
	/// </summary>
	/// <returns>Start position</returns>
	int getStartPosition()const { return m_startIndex; };
	/// <summary>
	/// Return start position
	/// </summary>
	/// <returns>Start position</returns>
	int getFinishPosition()const { return m_endIndex; };
	/// <summary>
	/// Get vector of square types
	/// </summary>
	/// <returns>Vector of square types</returns>
	std::vector<Cell>& getSquareVec() { return m_squareVec; }
private:
	sf::Vector2f m_positions;				//!< Mesh position in world
	sf::Vector2i m_size;					//!< Number of cells in mesh
	std::vector<Cell>	m_squareVec;		//!< Vector of square types (for drawing)
	unsigned m_cellSize;					//!< Size of each cell (square)
	int m_startIndex;						//!< Index of start point
	int m_endIndex;							//!< Index of end point
	// Drawing
	sf::VertexArray m_quads;				//!< Vertex array of quads for cell drawing
	sf::VertexArray m_lines;				//!< Vertex array of lines for inMesh line drawing
};