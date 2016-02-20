#include "reversi.h"
#include <iomanip>


void reversi::getCells(board & b, int colour, set<pair<int, int> > & cells) {
	cells.clear();
	cells =  b.colouredCells[colour - 1];
}

void reversi::applyMove(board & b, move m, multiset<move> & moves) {
	move sameMove(m.x, m.y, m.colour);
	//move moreSeedsMove(true);
	int x = m.seed.first;
	int y = m.seed.second;
	b.cells[m.x][m.y] = b.cells[x][y];
	b.colouredCells[b.cells[x][y] - 1].insert(std::make_pair(m.x, m.y));
	multiset<move>::iterator moreSeedsMove = moves.find(sameMove);
	/*if (moreSeedsMove == moves.end()) {
		o << "fuck something is wrong" << endl;
		for (int i = 0; i < b.cells[0].size(); ++i) {
			for (int j = 0; j < b.cells.size(); ++j)
				cout << std::setw(2) << b.cells[j][i];
		}
		cout << std::endl;
	}*/
	while (moreSeedsMove != moves.end()) {
		x = moreSeedsMove->seed.first;
		y = moreSeedsMove->seed.second;
		x += moreSeedsMove->direction.first;
		y += moreSeedsMove->direction.second;
		//b.cells[x][y] = 3 - b.cells[x][y];
		//set<pair<int, int> >::iterator k
		//	= b.colouredCells[3 - b.cells[x][y] - 1].find(std::make_pair(x, y));
		//b.colouredCells[3 - b.cells[x][y] - 1].erase(k);
		//b.colouredCells[b.cells[x][y] - 1].insert(std::make_pair(x, y));
		while (x != moreSeedsMove->x || y != moreSeedsMove->y) {
			b.cells[x][y] = 3 - b.cells[x][y];
			set<pair<int, int> >::iterator k
				= b.colouredCells[3 - b.cells[x][y] - 1].find(std::make_pair(x, y));
			b.colouredCells[3 - b.cells[x][y] - 1].erase(k);
			b.colouredCells[b.cells[x][y] - 1].insert(std::make_pair(x, y));
			x += moreSeedsMove->direction.first;
			y += moreSeedsMove->direction.second;
		}
		moves.erase(moreSeedsMove);
		moreSeedsMove = moves.find(sameMove);
	}
}

std::deque<vector<int> > generator(vector<int> alphabet, int length) {
	std::deque < vector<int> > sq;
	vector<int> a;
	sq.push_back(a);
	for (int i = 0; i < length; ++i) {
		int n = sq.size();
		for (int k = 0; k < n; ++k) {
			vector<int> a = sq.front();
			sq.pop_front();
			for (int j = 0; j < alphabet.size(); ++j) {
				vector<int> b = a;
				b.push_back(alphabet[j]);
				if (i == length - 1 && b[0] == 0 && b[1] == 0)
					continue;
				sq.push_back(b);
			}
		}
	}
	return sq;
}


void reversi::generateDirections(vector <pair<int, int> > & directions) {
	directions.clear();
	vector<int> a;
	a.push_back(-1);
	a.push_back(0);
	a.push_back(1);
	std::deque<vector<int> > directionsDeck = generator(a, 2);
	//assert(directionsDeck.size() == 8);
	for (int i = 0; i < directionsDeck.size(); ++i) {
		directions.push_back(std::make_pair(directionsDeck[i][0], directionsDeck[i][1]));
	}
}

move reversi::minimax(board & b, int colour, int maxDepth, int curDepth, int alreadyFoundExtremum,
	bool extremumFound, multiset<move> & moves, clock_t startTime/*, evaluationFunc func*/) {
	move bestMove(true);
	move cur(true);
	move nullMove(true);
	if (curDepth == maxDepth || (double(clock() - startTime) / (double)CLOCKS_PER_SEC > 2.5)) {
		nullMove.score = evaluateBoard(b, colour);
		return nullMove;
	}
	else {
		if (curDepth % 2 == 0) {// my colour
			bestMove.score = INT_MIN;
			findMoves(b, colour, moves);
			if (moves.empty()) {
				board bClone = b;
				multiset<move> movesNextLevel;
				bestMove = minimax(b, colour, maxDepth + 1, curDepth + 1, 0, false, movesNextLevel, startTime);
			}
			else{
				int k = 0;
				for (multiset<move> ::iterator m = moves.begin(); m != moves.end(); ++m) {
					board bClone = b;
					multiset<move> thisLevelMoves = moves;
					applyMove(bClone, (*m), thisLevelMoves);
					if (k == 0) {
						multiset<move> movesNextLevel;
						cur = minimax(bClone, colour, maxDepth, curDepth + 1, bestMove.score, false, movesNextLevel, startTime);
					}
					else {
						multiset<move> movesNextLevel;
						cur = minimax(bClone, colour, maxDepth, curDepth + 1, bestMove.score, true, movesNextLevel, startTime);
					}
					/*if (ans == nullMove)
						continue;*/
					if(cur.score > bestMove.score) {
						bestMove = (*m);
						bestMove.score = cur.score;
					}
					if (extremumFound && bestMove.score > alreadyFoundExtremum) // alpha-beta cutoff
						break;
					k++;
				}
			}
		}
		else {//opposite
			bestMove.score = INT_MAX;
			findMoves(b, 3 - colour, moves);
			if (moves.empty()) {
				multiset<move> movesNextLevel;
				move ans = minimax(b, colour, maxDepth, curDepth + 1, 0, false, movesNextLevel, startTime);
			}
			else {
				int k = 0;
				for (multiset<move> ::iterator m = moves.begin(); m != moves.end(); ++m) {
					board bClone = b;
					multiset<move> thisLevelMoves = moves;
					applyMove(bClone, (*m), thisLevelMoves);
					if (k == 0) {
						multiset<move> movesNextLevel;
						cur = minimax(bClone, colour, maxDepth, curDepth + 1, bestMove.score, false, movesNextLevel, startTime);
					}
					else {
						multiset<move> movesNextLevel;
						cur = minimax(bClone, colour, maxDepth, curDepth + 1, bestMove.score, true, movesNextLevel, startTime);
					}
					/*if (ans == nullMove)
						continue;*/
					if (cur.score < bestMove.score) {
						bestMove = (*m);
						bestMove.score = cur.score;
					}

					if (extremumFound && bestMove.score < alreadyFoundExtremum) // alpha-beta cutoff
						break;
					k++;
				}
			}
		}
	}
	return bestMove;
}


void reversi::findMoves(board & b, int colour, multiset<move> & moves) {
	moves.clear();
	set<pair<int, int> > myCells;
	getCells(b, colour, myCells);
	int x, y;
	int oppositeColour = 3 - colour;
	for (set<pair<int, int> >:: iterator i = myCells.begin(); i != myCells.end(); ++i) {
		for (int j = 0; j < directions.size(); ++j) {
			x = (*i).first;
			y = (*i).second;
			int count = 0;
			x += directions[j].first;
			y += directions[j].second;
			while (x < b.width && y < b.height && x >= 0 && y >= 0 && b.cells[x][y] == oppositeColour) {
				x += directions[j].first;
				y += directions[j].second;
				count++;
			}
			if (x == b.width || y == b.height || x == -1 || y == -1 || b.cells[x][y] != 0)
				continue;
			else if (count > 0) {
				move m(x, y, colour, count, (*i), directions[j]);
				moves.insert(m);
			}
		}
	}
}
