#include "reversi.h"


int main() {
	int w = 8, h = 8;
	reversi R(w, h);
	string init;
	string colour;
	cin >> init;
	cin >> colour;

	if (init == "init" && colour == "black")
		R.init(1);
	else if (init == "init" && colour == "white")
		R.init(2);
}

void reversi::init(int colour) {
	B->cells[4][3] = B->cells[3][4] = 1;
	B->cells[3][3] = B->cells[4][4] = 2;
	B->colouredCells[0].insert(std::make_pair(4, 3));
	B->colouredCells[0].insert(std::make_pair(3, 4));
	B->colouredCells[1].insert(std::make_pair(4, 4));
	B->colouredCells[1].insert(std::make_pair(3, 3));

	play(colour);
}

int intFromChar(char x) {
	return (int)x - (int)'a';
}

void reversi::play(int colour) {
	/*std::ofstream o;
	o.open("log.txt");*/
	clock_t startTime;
	multiset<move> moves;
	string command;
	cin >> command; 
	string a = "abcdefgh";
	vector<char> al(a.begin(), a.end());

	while (command != "win" || command != "lose" || command == "draw") {
		if (command == "bad") {
			//o << command;
			return;
		}
		if (command == "move") {
			board b = *B;
			multiset<move> oppositePosiibleMoves;
			findMoves(b, 3 - colour, oppositePosiibleMoves);
			char x;
			int y;
			cin >> x;
			cin >> y;
			y--;
			move m(intFromChar(x), y, 3 - colour);
			move realM = *(oppositePosiibleMoves.find(m));
			applyMove(*B, realM, oppositePosiibleMoves);
			//o << "opposite move " << al[realM.x] << " " << realM.y + 1 << endl;
		}
		else if (command == "turn") {
			//o << "turn";
			startTime = clock();
			board b = *B;
			moves.clear();
			move bestMove = minimax(b, colour, maxDepth, 0, 0, false, moves, startTime);
			applyMove(*B, bestMove, moves);
			cout << "move " << al[bestMove.x] << " " << bestMove.y + 1 << endl;
			//o << " my move " << al[bestMove.x] << " " << bestMove.y + 1 << endl;
		}
		cin >> command;
	}
}