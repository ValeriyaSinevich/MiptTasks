#include <cstdio>
#include <cstdlib>
#include <vector>
#include <map>
#include <unordered_set>
#include <deque>
#include <cassert>
#include <set>
#include <iostream>
#include <string>
#include <ctime>
#include <fstream>

using std::cout;
using std::cin;
using std::string;
using std::endl;

using std::ofstream;

using std::set;
using std::multiset;
using std::vector;
using std::pair;
using std::unordered_set;


//struct hashMove{
//	size_t operator()(const move & m) const{
//		size_t ans = 0;
//		ans = m.x * (1 << 24) + m.y * (1 << 21) + m.colour * (1 << 18) + m.score * (1 << 15) + m.direction.first * (1 << 12)
//			+ m.direction.second * (1 << 9) + m.seed.first * (1 << 6) + m.seed.second * (1 << 3);
//		return ans;
//	}
//};

struct board {
	board(int _w, int _h) : width(_w), height(_h) {
		cells.resize(width, vector<int>(height, 0));
	}
	int width, height;
	vector <vector <int> > cells;
	set<pair<int, int>> colouredCells[2];
};

struct cell {
	int x, y, colour;
	cell() {};
	cell(int _x, int _y, int _c) : x(_x), y(_y), colour(_c) {};
};

struct move : cell {
	int score;
	pair<int, int> direction;
	pair<int, int> seed;
	bool nullMove;
	move(int _x, int _y, int _c) : cell(_x, _y, _c) { nullMove = false; };
	move(int _x, int _y, int _sc, int _c, pair<int, int> _s, pair<int, int> _d) :
		cell(_x, _y, _c), score(_sc), seed(_s), direction(_d) {
		nullMove = false;
	};
	move(bool _nullmove) : nullMove(_nullmove) {};
	bool operator == (move const& m) {
		if (m.x == this->x && m.y == this->y)
			return true;
		else
			return false;
	}

	friend bool operator < (move const & l, move const& r) {
		if (l.x < r.x || (l.x == r.x && l.y < r.y))
			return true;
		else
			return false;
	}

	bool operator << (move const& r) {
		if (x < r.x || (x == r.x && y < r.y))
			return true;
		else
			return false;
	}
};

class reversi {
public:
	reversi(int _w, int _h) {
		B = new board(_w, _h);
		generateDirections(directions);
		maxDepth = 6;
	}
	void init(int colour);
	void play(int colour);
	
private:
	void applyMove(board & b, move m, multiset<move> & moves);
	int maxDepth;
	move minimax(board & b, int colour, int maxDepth, int curDepth, int alreadyFoundExtremum, bool ExtremumFound,
		multiset<move> & moves, clock_t startTime);
	vector <pair<int, int> > directions;
	board *B;
	void getCells(board & b, int colour, set<pair<int, int> > & cells);
	void findMoves(board & b, int colour, multiset<move> & moves);
	//move findBestMove(board & b, vector<move> & moves, int colour/*, evaluationFunc*/);
	int evaluateBoard(board & b, int colour);
	void generateDirections(vector <pair<int, int> > & directions);
	int countCorners(board & b, int colour);
	int countPieces(board & b, int colour);
	int countClosenessToCorners(board & b, int colour);
	int countPossibleMovesAndStability(board & b, int colour);
	pair<int, int> countWeighted(board & b, int colour);
};