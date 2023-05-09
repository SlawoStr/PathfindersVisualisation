#include "Mesh.h"

const int TEXT_SIZE = 20;

////////////////////////////////////////////////////////////
Mesh::Mesh(sf::Vector2f position, sf::Vector2i meshSize, unsigned cellSize)
	: m_positions{ position }, m_size{ meshSize }, m_cellSize{ cellSize }, m_quads{ sf::Quads }, m_lines{ sf::Lines }
{
	// Reserve memory for all elements
	m_squareVec.resize(meshSize.x * meshSize.y);
	m_quads.resize(meshSize.x * meshSize.y * 4);
	m_lines.resize((meshSize.x + 1) * (meshSize.y + 1) * 2);

	// Set up quads
	for (int i = 0; i < meshSize.y; ++i) 
	{
		for (int j = 0; j < meshSize.x; ++j) 
		{
			int index = i * meshSize.x + j;
			float x = position.x + j * m_cellSize;
			float y = position.y + i * m_cellSize;
			sf::Vector2f cellPosition(x, y);
			m_quads[index * 4].position = cellPosition;
			m_quads[index * 4 + 1].position = sf::Vector2f(x + m_cellSize, y);
			m_quads[index * 4 + 2].position = sf::Vector2f(x + m_cellSize, y + m_cellSize);
			m_quads[index * 4 + 3].position = sf::Vector2f(x, y + m_cellSize);

			// Set quad colors
			for (int k = 0; k < 4; ++k) {
				m_quads[index * 4 + k].color = sf::Color::White;
			}
		}
	}
	// Vertical lines
	for (int i = 0; i < meshSize.x + 1; ++i)
	{
		m_lines[i * 2].position = sf::Vector2f(position.x + i * m_cellSize, position.y);
		m_lines[i * 2 + 1].position = sf::Vector2f(position.x + i * m_cellSize, position.y + meshSize.y * m_cellSize);
		// Set line colors
		m_lines[i * 2].color = sf::Color::Black;
		m_lines[i * 2 + 1].color = sf::Color::Black;
	}
	// Horizontal lines
	for (int i = 0; i < meshSize.y + 1; ++i)
	{
		int index = meshSize.x + 1 + i;
		m_lines[index * 2].position = sf::Vector2f(position.x, position.y + i * m_cellSize);
		m_lines[index * 2 + 1].position = sf::Vector2f(position.x + meshSize.x * m_cellSize, position.y + i * m_cellSize);
		// Set line colors
		m_lines[index * 2].color = sf::Color::Black;
		m_lines[index * 2 + 1].color = sf::Color::Black;
	}
	// Create start and end point
	int startPoint = 0;
	int endPoint = static_cast<int>(m_squareVec.size() - 1);
	m_squareVec[startPoint].cellType = SquareType::START;
	m_squareVec[startPoint].distance = 0;
	m_squareVec[endPoint].cellType = SquareType::FINISH;
	setCellColor(SquareType::START, startPoint);
	setCellColor(SquareType::FINISH, endPoint);
	m_startIndex = startPoint;
	m_endIndex = endPoint;
}

////////////////////////////////////////////////////////////
void Mesh::draw(sf::RenderTarget& target, const sf::Font& font, bool drawSquares, bool drawLines, bool drawWeights)
{
	if (drawSquares)
	{
		target.draw(m_quads);
	}
	if (drawLines)
	{
		target.draw(m_lines);
	}
	if (drawWeights)
	{	
		sf::Text text;
		text.setFont(font);
		text.setFillColor(sf::Color::Red);
		text.setCharacterSize(m_cellSize);
		for (int i = 0; i < m_size.x; ++i)
		{
			for (int j = 0; j < m_size.y; ++j)
			{
				text.setString(std::to_string(m_squareVec[j * m_size.x + i].weight));
				text.setPosition(sf::Vector2f(m_positions.x + i * m_cellSize, m_positions.y + j * m_cellSize));
				target.draw(text);
			}
		}	
	}
}

////////////////////////////////////////////////////////////
void Mesh::drawData(sf::RenderTarget& target, const sf::Font& font, std::string solverName, int processedSquare, int pathLength, int pathWeight)
{
	float rectX = m_positions.x;
	float rectY = m_positions.y;
	float rectWidth = static_cast<float>(m_size.x * m_cellSize);
	float rectHeight = static_cast<float>(m_size.y * m_cellSize);

	sf::Text text(solverName, font, TEXT_SIZE);
	text.setFillColor(sf::Color::Black);

	float textX = rectX + (rectWidth / 2) - (text.getLocalBounds().width / 2);
	float textY = rectY - (text.getLocalBounds().height * 1.5f);
	// Draw solver name
	text.setPosition(textX, textY);
	text.move(0.0f, -TEXT_SIZE * 4);
	target.draw(text);
	// Draw processed squares
	text.move(0.0f, TEXT_SIZE);
	text.setString("Processed squares: " + std::to_string(processedSquare));
	target.draw(text);
	// Draw pathLength
	text.move(0.0f, TEXT_SIZE);
	text.setString("Path length: " + std::to_string(pathLength));
	target.draw(text);
	// Draw pathWeight
	text.move(0.0f, TEXT_SIZE);
	text.setString("Path weight: " + std::to_string(pathWeight));
	target.draw(text);
}

////////////////////////////////////////////////////////////
bool Mesh::isVisible(sf::FloatRect& windowBound)
{
	sf::Vector2f botRight = m_positions + sf::Vector2f(static_cast<float>(m_size.x * m_cellSize), static_cast<float>(m_size.y * m_cellSize));

	return windowBound.intersects(sf::FloatRect(m_positions, botRight - m_positions));
}

////////////////////////////////////////////////////////////
bool Mesh::isInRange(sf::Vector2f mousePos)
{
	sf::Vector2f botRight{ m_positions.x + m_size.x * m_cellSize,m_positions.y + m_size.y * m_cellSize };

	return (mousePos.x > m_positions.x) &&
		(mousePos.x < botRight.x) &&
		(mousePos.y > m_positions.y) &&
		(mousePos.y < botRight.y);
}

////////////////////////////////////////////////////////////
void Mesh::setCellType(sf::Vector2f mousePos, SquareType squareType)
{
	// Get cell index
	int col = static_cast<int>(mousePos.x - m_positions.x) / m_cellSize;
	int row = static_cast<int>(mousePos.y - m_positions.y) / m_cellSize;
	int index = row * m_size.x + col;

	// Modify cell if index is in range
	if (index >= 0 && index < m_squareVec.size())
	{
		SquareType modifiedSquareType = m_squareVec[index].cellType;

		if (modifiedSquareType != SquareType::START && modifiedSquareType != SquareType::FINISH)
		{
			if (squareType == SquareType::START)
			{
				// Reset current start cell
				m_squareVec[m_startIndex].cellType = SquareType::EMPTY;
				m_squareVec[m_startIndex].distance = std::numeric_limits<int>::max();
				setCellColor(SquareType::EMPTY, m_startIndex);
				// Reset new start cell
				m_squareVec[index].cellType = SquareType::START;
				m_squareVec[index].distance = 0;
				setCellColor(SquareType::START, index);
				// Change start position
				m_startIndex = index;
			}
			else if (squareType == SquareType::FINISH)
			{
				// Reset finish cell
				m_squareVec[m_endIndex].cellType = SquareType::EMPTY;
				setCellColor(SquareType::EMPTY, m_endIndex);
				// Reset start cell
				m_squareVec[index].cellType = SquareType::FINISH;
				setCellColor(SquareType::FINISH, index);
				// Change end position
				m_endIndex = index;
			}
			else
			{
				m_squareVec[index].cellType = squareType;
				setCellColor(squareType, index);
			}
		}
	}
}

////////////////////////////////////////////////////////////
void Mesh::addCellWeight(sf::Vector2f mousePos)
{
	// Get cell index
	int col = static_cast<int>(mousePos.x - m_positions.x) / m_cellSize;
	int row = static_cast<int>(mousePos.y - m_positions.y) / m_cellSize;
	int index = row * m_size.x + col;

	int currentWeight = m_squareVec[index].weight;
	if (currentWeight != 9)
	{
		m_squareVec[index].weight++;
	}
}

////////////////////////////////////////////////////////////
void Mesh::subCellWeight(sf::Vector2f mousePos)
{
	// Get cell index
	int col = static_cast<int>(mousePos.x - m_positions.x) / m_cellSize;
	int row = static_cast<int>(mousePos.y - m_positions.y) / m_cellSize;
	int index = row * m_size.x + col;

	int currentWeight = m_squareVec[index].weight;
	if (currentWeight != 1)
	{
		m_squareVec[index].weight--;
	}
}

////////////////////////////////////////////////////////////
void Mesh::copyMesh(const Mesh& mesh)
{
	for (int i = 0; i < mesh.m_squareVec.size(); ++i)
	{
		m_squareVec[i] = mesh.m_squareVec[i];
		m_startIndex = mesh.getStartPosition();
		m_endIndex = mesh.getFinishPosition();
		setCellColor(m_squareVec[i].cellType, i);
	}
}

////////////////////////////////////////////////////////////
void Mesh::resetMesh()
{
	for (int i = 0; i < m_squareVec.size(); ++i)
	{
		if (m_squareVec[i].cellType != SquareType::FINISH && m_squareVec[i].cellType != SquareType::START && m_squareVec[i].cellType != SquareType::FULL)
		{
			m_squareVec[i].cellType = SquareType::EMPTY;
			m_squareVec[i].distance = std::numeric_limits<int>::max();
			setCellColor(SquareType::EMPTY, i);
		}
	}
}

////////////////////////////////////////////////////////////
void Mesh::clearMesh()
{
	// Set all cells to empty
	for (int i = 0; i < m_squareVec.size(); ++i)
	{
		m_squareVec[i].cellType = SquareType::EMPTY;
		m_squareVec[i].distance = std::numeric_limits<int>::max();
		setCellColor(SquareType::EMPTY, i);
	}
	// Create start and finish
	int startPoint = 0;
	int endPoint = static_cast<int>(m_squareVec.size() - 1);
	m_squareVec[startPoint].cellType = SquareType::START;
	m_squareVec[startPoint].distance = 0;
	m_squareVec[endPoint].cellType = SquareType::FINISH;
	setCellColor(SquareType::START, startPoint);
	setCellColor(SquareType::FINISH, endPoint);
	m_startIndex = startPoint;
	m_endIndex = endPoint;
}

////////////////////////////////////////////////////////////
void Mesh::setCellColor(SquareType sqType, int index)
{
	// Get new color
	sf::Color color;
	switch (sqType)
	{
		case SquareType::EMPTY:
			color = sf::Color::White;
			break;
		case SquareType::FULL:
			color = sf::Color(110, 110, 110);
			break;
		case SquareType::START:
			color = sf::Color::Green;
			break;
		case SquareType::FINISH:
			color = sf::Color::Red;
			break;
		case SquareType::QUEUED:
			color = sf::Color(128, 0, 128);
			break;
		case SquareType::UNFINISHED:
			color = sf::Color(0, 191, 255);
			break;
		case SquareType::PROCESSED:
			color = sf::Color(160, 82, 45);
			break;
		case SquareType::PROCESSED2:
			color = sf::Color(244, 164, 96);
			break;
		case SquareType::PATH:
			color = sf::Color::Yellow;
			break;
		default:
			color = sf::Color::White;
			break;
	}
	// Change vertex color
	int vertexIndex = index * 4;
	m_quads[vertexIndex].color = color;
	m_quads[vertexIndex + 1].color = color;
	m_quads[vertexIndex + 2].color = color;
	m_quads[vertexIndex + 3].color = color;
}