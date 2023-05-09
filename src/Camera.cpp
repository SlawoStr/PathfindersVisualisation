#include "Camera.h"

////////////////////////////////////////////////////////////
bool Camera::handleEvent(sf::Event e)
{
    switch (e.type)
    {
        case sf::Event::Resized:
        {
            // Handle window resize event
            sf::View view = m_window.getView();
            view.setSize(static_cast<float>(e.size.width), static_cast<float>(e.size.height));
            view.zoom(m_zoomLevel);
            m_window.setView(view);
            return true;
        }
        case sf::Event::MouseMoved:
        {
            // Handle mouse movement event for panning
            if (m_panning)
            {
                sf::View view = m_window.getView();
                sf::Vector2i mousePos = sf::Mouse::getPosition(m_window);
                sf::Vector2f pos = sf::Vector2f(mousePos - this->m_panningAnchor);
                view.move(-1.0f * pos * m_zoomLevel);
                m_window.setView(view);
                m_panningAnchor = mousePos;
                return true;
            }
            break;
        }
        case sf::Event::MouseButtonPressed:
        {
            // Handle mouse button press event for panning
            if (e.mouseButton.button == sf::Mouse::Middle)
            {
                m_panning = true;
                m_panningAnchor = sf::Mouse::getPosition(m_window);
                return true;
            }
            break;
        }
        case sf::Event::MouseButtonReleased:
        {
            // Handle mouse button release event for panning
            if (e.mouseButton.button == sf::Mouse::Middle)
            {
                m_panning = false;
                return true;
            }
            break;
        }
        case sf::Event::MouseWheelMoved:
        {
            // Handle mouse wheel event for zooming
            sf::View view = m_window.getView();
            if (e.mouseWheel.delta < 0)
            {
                view.zoom(2.0f);
                m_zoomLevel *= 2.0f;
            }
            else
            {
                view.zoom(0.5f);
                m_zoomLevel *= 0.5f;
            }
            m_window.setView(view);
            return true;
        }
    }
    return false;
}

////////////////////////////////////////////////////////////
sf::FloatRect Camera::getGlobalViewBounds() const
{
    sf::View view = m_window.getView();
    sf::FloatRect viewBounds(view.getCenter() - view.getSize() / 2.0f, view.getSize());
    return viewBounds;
}
