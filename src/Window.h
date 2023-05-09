#pragma once
#include <SFML/Graphics.hpp>

/// <summary>
/// Abstract class for all simulations.
/// </summary>
class Window
{
public:
    Window(unsigned windowWidth, unsigned windowHeight, std::string windowTitle, unsigned framerate) : m_window(sf::VideoMode(windowWidth, windowHeight), windowTitle)
    {
        m_window.setFramerateLimit(framerate);
    }
    virtual ~Window() {}
    /// <summary>
    /// Runs the simulation.
    /// </summary>
    virtual void run() = 0;
private:
    /// <summary>
    /// Handles user interactions.
    /// </summary>
    virtual void pollEvent() = 0;
    /// <summary>
    /// Updates elements for the next frame.
    /// </summary>
    virtual void update() = 0;
    /// <summary>
    /// Draws elements on the screen.
    /// </summary>
    virtual void draw() = 0;
protected:
    sf::RenderWindow m_window;        //!< The main window of the application.
};