#include "MovingWindow.h"
#include "src/Utility/Timer.h"
#include <iostream>

////////////////////////////////////////////////////////////
MovingWindow::MovingWindow(unsigned windowWidth, unsigned windowHeight, std::string windowTitle, unsigned framerate) 
	: Window{windowWidth,windowHeight,windowTitle,framerate}, m_camera{ m_window }
{
	m_gridManager.addMesh(sf::Vector2f(static_cast<float>(((m_meshSize.x + 10) * m_squareSize) * 0), 750.0f), m_meshSize, m_squareSize, SolverType::DFS);
	m_gridManager.addMesh(sf::Vector2f(static_cast<float>(((m_meshSize.x + 10) * m_squareSize) * 1), 750.0f), m_meshSize, m_squareSize, SolverType::BFS);
	m_gridManager.addMesh(sf::Vector2f(static_cast<float>(((m_meshSize.x + 10) * m_squareSize) * 2), 750.0f), m_meshSize, m_squareSize, SolverType::DJIKSTRA);
	m_gridManager.addMesh(sf::Vector2f(static_cast<float>(((m_meshSize.x + 10) * m_squareSize) * 3), 750.0f), m_meshSize, m_squareSize, SolverType::ASTAR);
	m_gridManager.addMesh(sf::Vector2f(static_cast<float>(((m_meshSize.x + 10) * m_squareSize) * 1.5), 0.0f), m_meshSize, m_squareSize);
}

////////////////////////////////////////////////////////////
void MovingWindow::run()
{
	Timer t;
	while (m_window.isOpen())
	{
		t.start();
		pollEvent();
		if (!m_isPaused)
		{
			update();
		}
		m_window.clear(sf::Color(128, 128, 128));
		draw();
		m_window.display();
		t.stop();
		std::cout << "Frame time: " << t.measure() << std::endl;
	}
}

////////////////////////////////////////////////////////////
void MovingWindow::pollEvent()
{
	sf::Event e;
	while (m_window.pollEvent(e))
	{
		if (m_camera.handleEvent(e))
		{
			break;
		}
		switch (e.type)
		{
			case sf::Event::Closed:
			{
				m_window.close();
				break;
			}
			case sf::Event::KeyPressed:
			{
				switch (e.key.code)
				{
					case sf::Keyboard::Escape:
					{
						m_window.close();
						break;
					}
					case sf::Keyboard::Z:
					{
						m_gridManager.switchSquares();
						break;
					}
					case sf::Keyboard::X:
					{
						m_gridManager.switchLines();
						break;
					}
					case sf::Keyboard::D:
					{
						m_gridManager.switchWeights();
						break;
					}
					case sf::Keyboard::C:
					{
						m_gridManager.copyTemplateMesh();
						break;
					}
					case sf::Keyboard::P:
					{
						m_isPaused = !m_isPaused;
						break;
					}
					case sf::Keyboard::M:
					{
						m_isEditing = !m_isEditing;
						break;
					}
					case sf::Keyboard::S:
					{
						m_squareSpawnType = SquareType::START;
						m_isSpawning = true;
						break;
					}
					case sf::Keyboard::E:
					{
						m_squareSpawnType = SquareType::FINISH;
						m_isSpawning = true;
						break;
					}
					case sf::Keyboard::G:
					{
						m_gridManager.generateMaze();
						break;
					}
					case sf::Keyboard::Up:
					{
						m_brushSize.y++;
						break;
					}
					case sf::Keyboard::Down:
					{
						m_brushSize.y--;
						if (m_brushSize.y <= 0)
						{
							m_brushSize.y = 1;
						}
						break;
					}
					case sf::Keyboard::Right:
					{
						m_brushSize.x++;
						break;
					}
					case sf::Keyboard::Left:
					{
						m_brushSize.x--;
						if (m_brushSize.x <= 0)
						{
							m_brushSize.x = 1;
						}
						break;
					}
					default:
						break;
					}
				break;
			}
			case sf::Event::KeyReleased:
			{
				if (e.key.code == sf::Keyboard::S)
				{
					m_isSpawning = false;
				}
				else if (e.key.code == sf::Keyboard::E)
				{
					m_isSpawning = false;
				}
				break;
			}
			case sf::Event::MouseButtonPressed:
			{
				if (e.key.code == sf::Mouse::Button::Left)
				{
					m_squareSpawnType = SquareType::FULL;
					m_isSpawning = true;
				}
				else if (e.key.code == sf::Mouse::Button::Right)
				{
					m_squareSpawnType = SquareType::EMPTY;
					m_isSpawning = true;
				}
				break;
			}
			case sf::Event::MouseButtonReleased:
			{
				if (e.key.code == sf::Mouse::Button::Left)
				{
					m_isSpawning = false;
				}
				else if (e.key.code == sf::Mouse::Button::Right)
				{
					m_isSpawning = false;
				}
				break;
			}
		}
	}
	if (m_isSpawning)
	{
		sf::Vector2f mousePos = m_window.mapPixelToCoords(sf::Mouse::getPosition(m_window));

		if (m_squareSpawnType == SquareType::START || m_squareSpawnType == SquareType::FINISH)
		{
			m_gridManager.setSquareType(mousePos, m_squareSpawnType);
		}
		else
		{
			for (int i = 0; i < m_brushSize.x; ++i)
			{
				float mousePosY = mousePos.y;
				for (int j = 0; j < m_brushSize.y; ++j)
				{
					if (m_isEditing)
					{
						m_gridManager.setSquareType(mousePos, m_squareSpawnType);
					}
					else
					{
						if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
						{
							m_gridManager.changeSquareWeight(mousePos, true);
						}
						else if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
						{
							m_gridManager.changeSquareWeight(mousePos, false);
						}
					}
					mousePos.y += m_squareSize;
				}
				mousePos.x += m_squareSize;
				mousePos.y = mousePosY;
			}
		}
	}
}

////////////////////////////////////////////////////////////
void MovingWindow::update()
{
	m_gridManager.update();
}

////////////////////////////////////////////////////////////
void MovingWindow::draw()
{
	sf::FloatRect viewBound = m_camera.getGlobalViewBounds();
	m_gridManager.draw(m_window, viewBound);
}
