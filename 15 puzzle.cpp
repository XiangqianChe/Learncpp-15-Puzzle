#include <iostream>

// Increase amount of new lines if your board isn't
// at the very bottom of the console
constexpr int g_consoleLines{ 25 };

class Tile
{
public:
	Tile() = default;
	explicit Tile(int num)
		:m_num{ num }
	{}
	friend std::ostream& operator<<(std::ostream& out, Tile tile);
	int getNum() const { return m_num; }
	bool isEmpty() const { return m_num == 0; }
private:
	int m_num{};
};
std::ostream& operator<<(std::ostream& out, Tile tile)
{
	if (tile.m_num > 9)
		out << " " << tile.m_num << " ";
	else if (tile.m_num > 0)
		out << "  " << tile.m_num << " ";
	else if (tile.m_num == 0)
		out << "    ";
	return out;
}

class Board
{
public:
	Board() = default;
	friend std::ostream& operator<<(std::ostream& out, const Board& board);
private:
	static constexpr int SIZE{ 4 };
	Tile m_tiles[SIZE][SIZE]{
		Tile{1}, Tile{2},Tile{3},Tile{4},
		Tile{5}, Tile{6},Tile{7},Tile{8},
		Tile{9}, Tile{10},Tile{11},Tile{12},
		Tile{13}, Tile{14},Tile{15},Tile{0}
	};
};
std::ostream& operator<<(std::ostream& out, const Board& board)
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

namespace UserInput
{
	bool isValidCommand(char command)
	{
		return command == 'w'
			|| command == 'a'
			|| command == 's'
			|| command == 'd'
			|| command == 'q';
	}
	char getCommandFromUser()
	{
		char command{};
		std::cin >> command;
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		if (!isValidCommand(command))
			command = getCommandFromUser();
		return command;
	}
}
int main()
{
	Board board{};
	std::cout << board;
	while (true)
	{
		char command{ UserInput::getCommandFromUser() };
		std::cout << "Valid command: " << command << '\n';
		if (command == 'q')
		{
			std::cout << "\n\nBye!\n\n";
			return 0;
		}
	}
	return 0;
}