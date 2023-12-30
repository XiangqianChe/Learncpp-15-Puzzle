#include <iostream>
#include <cassert>
#include "Random.h"

constexpr int g_consoleLines{ 25 };

class Direction
{
public:
	enum Type
	{
		up,
		down,
		left,
		right,
		max_directions
	};
	Direction(Type type)
		:m_type{ type }
	{}
	Type getType() const
	{
		return m_type;
	}
	Direction operator-() const
	{
		switch (m_type)
		{
		case up: return Direction{ down };
		case down: return Direction{ up };
		case left: return Direction{ right };
		case right: return Direction{ left };
		}
		assert(0 && "Unsupported direction!");
		return Direction{ up };
	}
	friend std::ostream& operator<<(std::ostream& out, Direction direction)
	{
		switch (direction.m_type)
		{
		case Direction::up: return (out << "up");
		case Direction::down: return (out << "down");
		case Direction::left: return (out << "left");
		case Direction::right: return (out << "right");
		default: return (out << "unknown direction");
		}
	}
	static Direction getRandomDirection()
	{
		return Direction{ static_cast<Type>(Random::get(0, max_directions - 1)) };
	}
private:
	Type m_type{};
};

struct Point
{
public:
	int x{};
	int y{};
	Point getAdjacentPoint(Direction direction) const
	{
		switch (direction.getType())
		{
		case Direction::up: return Point{ x, y - 1 };
		case Direction::down: return Point{ x, y + 1 };
		case Direction::left: return Point{ x - 1, y };
		case Direction::right: return Point{ x + 1, y };
		default: return *this;
		}
	}
	friend bool operator==(Point p1, Point p2)
	{
		return (p1.x == p2.x) && (p1.y == p2.y);
	}
	friend bool operator!=(Point p1, Point p2)
	{
		return !(p1 == p2);
	}
};

namespace UserInput
{
	char getCommandFromUser()
	{
		char command{};
		std::cin >> command;
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		if ((command != 'w') && (command != 'a') &&
			(command != 's') && (command != 'd') &&
			(command != 'q'))
			command = getCommandFromUser();
		return command;
	}
	Direction charToDir(char command)
	{
		switch (command)
		{
		case 'w': return Direction{ Direction::up };
		case 's': return Direction{ Direction::down };
		case 'a': return Direction{ Direction::left };
		case 'd': return Direction{ Direction::right };
		}
		assert(0 && "Unsupported direction!");
		return Direction{ Direction::up };
	}
}

class Tile
{
public:
	Tile() = default;
	explicit Tile(int num)
		:m_num{ num }
	{}
	int getNum() const { return m_num; }
	bool isEmpty() const { return m_num == 0; }
	friend std::ostream& operator<<(std::ostream& out, Tile tile)
	{
		if (tile.m_num > 9)
			out << " " << tile.m_num << " ";
		else if (tile.m_num > 0)
			out << "  " << tile.m_num << " ";
		else if (tile.m_num == 0)
			out << "    ";
		return out;
	}
private:
	int m_num{};
};

class Board
{
public:
	Board() = default;
	bool isValidPoint(Point point)
	{
		return (point.x >= 0 && point.x < SIZE)
			&& (point.y >= 0 && point.y < SIZE);
	}
	Point findEmptyTilePoint() const
	{
		for (int tileRow{}; tileRow < SIZE; ++tileRow)
			for (int tileCol{}; tileCol < SIZE; ++tileCol)
				if (m_tiles[tileRow][tileCol].isEmpty())
					return { tileCol, tileRow };
		assert(0 && "No empty tile!");
		return { -1, -1 };
	}
	void swapTiles(Point p1, Point p2)
	{
		std::swap(m_tiles[p1.y][p1.x], m_tiles[p2.y][p2.x]);
	}
	bool moveTile(Direction direction)
	{
		Point emptyTilePoint{ findEmptyTilePoint() };
		Point ajacentPoint{ emptyTilePoint.getAdjacentPoint(-direction) };
		if (!isValidPoint(ajacentPoint)) return false;
		swapTiles(emptyTilePoint, ajacentPoint);
		return true;
	}
	friend std::ostream& operator<<(std::ostream& out, const Board& board)
	{
		for (int lineCount{}; lineCount < g_consoleLines; ++lineCount)
			std::cout << '\n';
		for (int tileRow{}; tileRow < Board::SIZE; ++tileRow)
		{
			for (int tileCol{}; tileCol < Board::SIZE; ++tileCol)
				out << board.m_tiles[tileRow][tileCol];
			out << '\n';
		}
		return out;
	}
private:
	static constexpr int SIZE{ 4 };
	Tile m_tiles[SIZE][SIZE]{
		Tile{1}, Tile{2},Tile{3},Tile{4},
		Tile{5}, Tile{6},Tile{7},Tile{8},
		Tile{9}, Tile{10},Tile{11},Tile{12},
		Tile{13}, Tile{14},Tile{15},Tile{0}
	};
};

int main()
{
	Board board{};
	std::cout << board;
	std::cout << "Enter a command: ";
	while (true)
	{
		char ch{ UserInput::getCommandFromUser() };
		if (ch == 'q')
		{
			std::cout << "\n\nBye!\n\n";
			return 0;
		}
		Direction dir{ UserInput::charToDir(ch) };
		bool tileMoved{ board.moveTile(dir) };
		if (tileMoved)
			std::cout << board;
	}

	return 0;
}