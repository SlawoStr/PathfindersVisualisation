#pragma once
#include "Window.h"
#include "Camera.h"
#include "src/Mesh/GridManager.h"

/// <summary>
/// A template for a window that allows for basic movement, including camera movement and zooming.
/// </summary>
class MovingWindow : public Window
{
public:
	/// <summary>
	/// Creates a window for the application.
	/// </summary>
	/// <param name="windowWidth">The width of the window in pixels.</param>
	/// <param name="windowHeight">The height of the window in pixels.</param>
	/// <param name="windowTitle">The title of the window.</param>
	/// <param name="framerate">The maximum frame rate of the window in frames per second.</param>
	MovingWindow(unsigned windowWidth, unsigned windowHeight, std::string windowTitle, unsigned framerate);
	/// <summary>
	///  Runs the simulation by executing the game loop.
	/// </summary>
	void run() override;
private:
	/// <summary>
	/// Handles user interactions, such as keyboard and mouse input.
	/// </summary>
	void pollEvent() override;
	/// <summary>
	/// Updates elements of the simulation for the next frame.
	/// </summary>
	void update() override;
	/// <summary>
	/// Draws elements of the simulation on the screen.
	/// </summary>
	void draw() override;
private:
	Camera m_camera;									//!< Handles camera movement and zooming
	GridManager m_gridManager;							//!< Manager for the grids
	bool m_isPaused{ true };							//!< Block update flag
	bool m_isSpawning{ false };							//!< Block activation flag
	bool m_isEditing{ true };							//!< Edit/Modify mode
	// Mesh settings
	SquareType m_squareSpawnType{ SquareType::EMPTY };	//!< Type of square to spawn
	sf::Vector2i m_brushSize{ 1,1 };					//!< Size of brush to draw 
	sf::Vector2i m_meshSize{ 50,50 };					//!< Size of mesh
	int m_squareSize{ 10 };								//!< Size of each square in mesh
};