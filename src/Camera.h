#pragma once
#include <SFML/Graphics.hpp>

// Camera class that handles panning and zooming of the view in a SFML window
class Camera
{
public:
	// Constructor that takes a reference to the SFML window
	Camera(sf::RenderWindow& window) : m_window{ window } {}
	// Handle SFML events for camera panning and zooming, returns true if event was handled by camera
	bool handleEvent(sf::Event e);

	sf::FloatRect getGlobalViewBounds() const;
private:
	sf::RenderWindow& m_window;				//!< Reference to the SFML window that the camera is controlling.
	float m_zoomLevel{ 1.0f };				//!< The current zoom level of the camera.
	bool m_panning{ false };				//!< A boolean flag indicating whether the camera is currently in panning mode.
	sf::Vector2i m_panningAnchor{ 0,0 };	//!< The screen coordinates of the anchor point for camera panning, used for calculating camera movement
};