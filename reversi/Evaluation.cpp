#include "reversi.h"

pair<int, int> reversi::countWeighted(board & b, int colour) {
	int myFronPieces = 0, oppositeFrontPieces = 0, x, y, i, j, k;
	int X1[] = { -1, -1, 0, 1, 1, 1, 0, -1 };
	int Y1[] = { 0, 1, 1, 1, 0, -1, -1, -1 };
	int V[8][8] =
	{ { 20, -3, 11, 8, 8, 11, -3, 20 },
	{ -3, -7, -4, 1, 1, -4, -7, -3 },
	{ 11, -4, 2, 2, 2, 2, -4, 11 },
	{ 8, 1, 2, -3, -3, 2, 1, 8 },
	{ 8, 1, 2, -3, -3, 2, 1, 8 },
	{ 11, -4, 2, 2, 2, 2, -4, 11 },
	{ -3, -7, -4, 1, 1, -4, -7, -3 },
	{ 20, -3, 11, 8, 8, 11, -3, 20 } };

	int c = 0, l = 0, m = 0, f = 0, d = 0;

	for (i = 0; i<8; i++)
	for (j = 0; j<8; j++)  {
		if (b.cells[i][j] == colour)  {
			d += V[i][j];
		}
		else if (b.cells[i][j] == 3 - colour)  {
			d -= V[i][j];
		}
		if (b.cells[i][j] != 0)   {
			for (int k = 0; k < 8; k++)  {
				int x = i + X1[k]; int y = j + Y1[k];
				if (x >= 0 && x < 8 && y >= 0 && y < 8 && b.cells[x][y] == 0) {
					if (b.cells[i][j] == colour)  myFronPieces++;
					else oppositeFrontPieces++;
					break;
				}
			}
		}
	}
	
	if (myFronPieces > oppositeFrontPieces)
		f = -(100.0 * myFronPieces) / (myFronPieces + oppositeFrontPieces);
	else if (myFronPieces < oppositeFrontPieces)
		f = (100.0 * oppositeFrontPieces) / (myFronPieces + oppositeFrontPieces);
	else f = 0;
	return std::make_pair(d, f);
}

int reversi::countCorners(board & b, int colour) {
	int my, opposite;
	my = opposite = 0;

	if (b.cells[0][0] == colour)
		my++;
	else if (b.cells[0][0] == 3 - colour)
		opposite++;

	if (b.cells[b.width - 1][0] == colour)
		my++;
	else if (b.cells[b.width - 1][0] == 3 - colour)
		opposite++;

	if (b.cells[0][b.height - 1] == colour)
		my++;
	else if (b.cells[0][b.height - 1] == 3 - colour)
		opposite++;

	if (b.cells[b.width - 1][b.height - 1] == colour)
		my++;
	else if (b.cells[b.width - 1][b.height - 1] == 3 - colour)
		opposite++;

	//return 25 * (my - opposite);
	int p;
	if (my > opposite)
		p = (100.0 * my) / (my + opposite);
	else if (my < opposite)
		p = -(100.0 * opposite) / (my + opposite);
	else p = 0;
	return p;
	/*if (my + opposite != 0)
		return (my - opposite) / (my + opposite);
	else return 0;*/
}


int reversi::countPossibleMovesAndStability(board & b, int colour) {
	int my, opposite;
	my = opposite = 0;
	int myStable = 0;
	int myUnStable = 0;
	int oppositeStable = 0;
	int oppositeUnStable = 0;
	multiset<move> myMoves;
	findMoves(b, colour, myMoves);
	for (multiset<move>::iterator m = myMoves.begin(); m != myMoves.end(); ++m) {
		oppositeUnStable += m->score;
	}
	oppositeStable += (b.colouredCells[3 - colour - 1].size() - oppositeUnStable);

	multiset<move> oppositeMoves;
	findMoves(b, 3 - colour, oppositeMoves);
	for (multiset<move>::iterator m = oppositeMoves.begin(); m != oppositeMoves.end(); ++m) {
		myUnStable += m->score;
	}
	myStable += (b.colouredCells[colour - 1].size() - myUnStable);

	my = myMoves.size();
	opposite = oppositeMoves.size();

	int s;
	if (myStable > oppositeStable)
		s = (100.0 * myStable) / (myStable + oppositeStable);
	else if (my < opposite)
		s = -(100.0 * oppositeStable) / (myStable + oppositeStable);
	else s = 0;

	int p;
	if (my > opposite)
		p = (100.0 * my) / (my + opposite);
	else if (my < opposite)
		p = -(100.0 * opposite) / (my + opposite);
	else p = 0;

	return p + s;
}


int reversi::countClosenessToCorners(board & b, int colour) {
	int my, opposite;
	my = opposite = 0;
	if (b.cells[0][0] == 0) {
		if (b.cells[0][1] == colour) my++;
		else if (b.cells[0][1] == 3 - colour) opposite++;
		if (b.cells[1][0] == colour) my++;
		else if (b.cells[1][0] == 3 - colour) opposite++;
	}

	if (b.cells[b.width - 1][0] == 0) {
		if (b.cells[b.width - 1][1] == colour) my++;
		else if (b.cells[b.width - 1][1] == 3 - colour) opposite++;
		if (b.cells[b.width - 2][0] == colour) my++;
		else if (b.cells[b.width - 2][0] == 3 - colour) opposite++;
	}

	if (b.cells[0][b.height - 1] == 0) {
		if (b.cells[0][b.height - 2] == colour) my++;
		else if (b.cells[0][b.height - 2] == 3 - colour) opposite++;
		if (b.cells[1][b.height - 1] == colour) my++;
		else if (b.cells[1][b.height - 1] == 3 - colour) opposite++;
	}

	if (b.cells[b.width - 1][b.height - 1] == 0) {
		if (b.cells[b.width - 2][b.height - 1] == colour) my++;
		else if (b.cells[b.width - 2][b.height - 1] == 3 - colour) opposite++;
		if (b.cells[b.width - 1][b.height - 2] == colour) my++;
		else if (b.cells[b.width - 1][b.height - 2] == 3 - colour) opposite++;
	}
	
	int p;
	if (my > opposite)
		p = -(100.0 * my) / (my + opposite);
	else if (my < opposite)
		p = (100.0 * opposite) / (my + opposite);
	else p = 0;
	return p;
}

int reversi::countPieces(board & b, int colour) {
	int my, opposite;
	my = b.colouredCells[colour - 1].size();
	opposite = b.colouredCells[3 - colour - 1].size();
	int p = 0;
	if (my > opposite)
		p = (100.0 * my) / (my + opposite);
	else if (my < opposite)
		p = -(100.0 * opposite) / (my + opposite);
	else p = 0;
	return p;
}

int reversi::evaluateBoard(board & b, int colour) {
	int m = countPossibleMovesAndStability(b, colour);
	int c = countCorners(b, colour);
	int cc = countClosenessToCorners(b, colour);
	int p = countPieces(b, colour);
	pair<int, int> df = countWeighted(b, colour);
	//int score = (10 * p) + (801.724 * c) + (382.026 * cc) + (78.922 * m) + (74.396 * df.second) + (10 * df.first);
	//return score;
	double score = m + c * 1.5 + p + cc;
	return score;
}