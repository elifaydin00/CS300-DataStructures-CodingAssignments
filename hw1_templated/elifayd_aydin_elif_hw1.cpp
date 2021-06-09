/*Written by Elif Aydin CS300 Homework1*/

#include <iostream>
#include <string>
#include <vector>
#include <fstream> 
#include "Stack.h"
#include "Stack.cpp"
using namespace std;

int n; //board size

struct pawn {
	int x;
	int y;
	int indexx;
	int indexy;
	pawn(int horizontal, int vertical) {
		x = horizontal;
		y = vertical;
		indexx = n - y;
		indexy = x - 1;
	}
	pawn() {

	}
};

struct cellNode {
	int x;
	int y;
	int indexx;
	int indexy;
	bool has_pawn;
	bool available;
	bool visited;
	int initialavaibs;  
	int unavaibs; 

	bool dontplace;
};

//reset cellNode's avail status while placing pawns and finding path for the knight
void reset_avail(vector<vector<cellNode>>& board) {
	for (int i = 0; i < board.size(); i++) {
		for (int j = 0; j < board.size(); j++) {
			board[i][j].available = true;
		}
	}
}

//reset cellNode's initialavaibs and unavaibs to 0 and visited status before findingPath
void reset_numbers(vector<vector<cellNode>>& board) {
	for (int i = 0; i < board.size(); i++) {
		for (int j = 0; j < board.size(); j++) {
			board[i][j].initialavaibs = 0;
			board[i][j].unavaibs = 0;
			board[i][j].visited = false;
		}
	}
}


//check horizontally if it is fine to put pawn on board[x][y]
bool rowCheck(vector<vector<cellNode>>& board, int x, int y) {
	int counter = 0;
	int size = board.size();
	int right = y + 1;
	//count right
	while (right <= size - 1) {
		if (board[x][right].has_pawn) {
			counter++;
		}
		right++;
	}

	int left = y - 1;
	while (left >= 0) {
		if (board[x][left].has_pawn) {
			counter++;
		}
		left--;
	}
	if (counter < 3) {
		return true;
	}
	else {
		return false;
	}
}

//check vertically if it is fine to put pawn on board[x][y]
bool colCheck(vector<vector<cellNode>>& board, int x, int y) {
	int counter = 0;
	int size = board.size();
	int down = x + 1;
	//count down
	while (down <= size - 1) {
		if (board[down][y].has_pawn) {
			counter++;
		}
		down++;
	}
	//count up
	int up = x - 1;
	while (up >= 0) {
		if (board[up][y].has_pawn) {
			counter++;
		}
		up--;
	}
	if (counter < 3) {
		return true;
	}
	else {
		return false;
	}
}
//check diag1
bool diagOneCheck(vector<vector<cellNode>>& board, int x, int y) {
	int counter = 0;
	int size = board.size();
	int up = x - 1;
	int right = y + 1;
	//check up and right
	while (up >= 0 && right <= size - 1) {
		if (board[up][right].has_pawn) {
			counter++;
		}
		up--;
		right++;
	}
	//check down and left
	int down = x + 1;
	int left = y - 1;
	while (down <= size - 1 && left >= 0) {
		if (board[down][left].has_pawn) {
			counter++;
		}
		down++;
		left--;
	}
	if (counter < 3) {
		return true;
	}
	else {
		return false;
	}
}
//check diag2
bool diagTwoCheck(vector<vector<cellNode>>& board, int x, int y) {
	int counter = 0;
	int size = board.size();
	int up = x - 1;
	int left = y - 1;
	//check up left
	while (up >= 0 && left >= 0) {
		if (board[up][left].has_pawn) {
			counter++;
		}
		up--;
		left--;
	}
	//check down and right
	int down = x + 1;
	int right = y + 1;
	while (down <= size - 1 && right <= size - 1) {
		if (board[down][right].has_pawn) {
			counter++;
		}
		down++;
		right++;
	}
	if (counter < 3) {
		return true;
	}
	else {
		return false;
	}
}

//returns available adjacents for placing pawns
vector<cellNode> available_adjacents(vector<vector<cellNode>>& board, cellNode& curr) {
	vector<cellNode> available_adjacents;
	//get the indexes of the current cell
	int size = board.size();
	int x = curr.indexx;
	int y = curr.indexy;

	int up = x - 1;
	int down = x + 1;
	int right = y + 1;
	int left = y - 1;

	if (up >= 0) { //cell has up adjacent
		if (!board[up][y].has_pawn && !board[up][y].dontplace) { //up has dont have a pawn
			if (rowCheck(board, up, y) && colCheck(board, up, y) && diagOneCheck(board, up, y) && diagTwoCheck(board, up, y) && board[up][y].available) {
				available_adjacents.push_back(board[up][y]);
			}
		}
		//check up and left
		if (left >= 0) { //cell has up&left adjacent
			if (!board[up][left].has_pawn && !board[up][left].dontplace) { //up left doesnt have a pawn
				if (rowCheck(board, up, left) && colCheck(board, up, left) && diagOneCheck(board, up, left) && diagTwoCheck(board, up, left) && board[up][left].available) {
					available_adjacents.push_back(board[up][left]);
				}
			}
		}
		//check up and right
		if (right <= size - 1) { //cell has up&right adjacent
			if (!board[up][right].has_pawn && !board[up][right].dontplace) { //up&right doesnt have a pawn
				if (rowCheck(board, up, right) && colCheck(board, up, right) && diagOneCheck(board, up, right) && diagTwoCheck(board, up, right) && board[up][right].available) {
					available_adjacents.push_back(board[up][right]);
				}
			}
		}
	}
	if (down <= size - 1) { //cell has down adjacent
		if (!board[down][y].has_pawn && !board[down][y].dontplace) { //down has dont have a pawn
			if (rowCheck(board, down, y) && colCheck(board, down, y) && diagOneCheck(board, down, y) && diagTwoCheck(board, down, y) && board[down][y].available) {
				available_adjacents.push_back(board[down][y]);
			}
		}
		//check down and left
		if (left >= 0) { //cell has left&down adjacent
			if (!board[down][left].has_pawn && !board[down][left].dontplace) { //left&down has dont have a pawn
				if (rowCheck(board, down, left) && colCheck(board, down, left) && diagOneCheck(board, down, left) && diagTwoCheck(board, down, left) && board[down][left].available) {
					available_adjacents.push_back(board[down][left]);
				}
			}
		}
		//check down and right
		if (right <= size - 1) { //cell has right&down adjacent
			if (!board[down][right].has_pawn && !board[down][right].dontplace) { //right&down has dont have a pawn
				if (rowCheck(board, down, right) && colCheck(board, down, right) && diagOneCheck(board, down, right) && diagTwoCheck(board, down, right) && board[down][right].available) {
					available_adjacents.push_back(board[down][right]);
				}
			}
		}
	}

	if (left >= 0) { //cell has left adjacent
		if (!board[x][left].has_pawn && !board[x][left].dontplace) { //left has dont have a pawn
			if (rowCheck(board, x, left) && colCheck(board, x, left) && diagOneCheck(board, x, left) && diagTwoCheck(board, x, left) && board[x][left].available) {
				available_adjacents.push_back(board[x][left]);
			}
		}
	}
	if (right <= size - 1) { //cell has right adjacent
		if (!board[x][right].has_pawn && !board[x][right].dontplace) { //right has dont have a pawn
			if (rowCheck(board, x, right) && colCheck(board, x, right) && diagOneCheck(board, x, right) && diagTwoCheck(board, x, right) && board[x][right].available) {
				available_adjacents.push_back(board[x][right]);
			}
		}
	}
	return available_adjacents;
}

void printMatrix(const vector<vector<cellNode>>& board) {
	cout << "The matrix:" << endl;
	for (int i = 0; i < board.size(); i++) {
		for (int j = 0; j < board.size(); j++) {
			if (board[i][j].has_pawn) {
				cout << "1 ";
			}
			else {
				cout << "0 ";
			}
		}
		cout << endl;
	}
}


//replaced is currently 1 because we replaced the first pawn into left bottom corner. Places other p-1 pawns
void place_pawns(vector<vector<cellNode>>& board, Stack<pawn>& pawns, int p, int replaced) {
	cellNode currCell = board[n-1][0];
	cellNode chosenCell;
	pawn currPawn;
	int size = board.size();
	int lower = 0;
	int upper, rndmi, index1, index2, initials;
	while (replaced < p) {
		vector<cellNode> available = available_adjacents(board, currCell); 

		if ((available.size() == 0)|| ((currCell.unavaibs!=0) && (currCell.unavaibs==currCell.initialavaibs) )) {
			index1 = pawns.top_return().indexx;
			index2 = pawns.top_return().indexy;			
			board[index1][index2].available = false;
			board[index1][index2].has_pawn = false;
			board[index1][index2].unavaibs = 0;
			pawns.pop();
			index1 = pawns.top_return().indexx;
			index2 = pawns.top_return().indexy;
			board[index1][index2].unavaibs++;
			replaced--;
		}
		
		else { //choose random cell
			upper = available.size() - 1;
			rndmi = (rand() % (upper - lower + 1)) + lower;
			chosenCell = available[rndmi];
			pawn new_pawn2(chosenCell.x, chosenCell.y);
			pawns.push(new_pawn2);
			index1 = new_pawn2.indexx;
			index2 = new_pawn2.indexy;
			board[index1][index2].has_pawn = true;
			board[index1][index2].visited = true;
			reset_avail(board);
			available = available_adjacents(board, board[index1][index2]);
			initials = available.size();
			board[index1][index2].initialavaibs = initials;
			replaced++;
		}
		index1 = pawns.top_return().indexx;
		index2 = pawns.top_return().indexy;
		currCell = board[index1][index2];		
	}
}

vector<cellNode>available_knight(vector<vector<cellNode>>& board, cellNode& curr) {
	vector<cellNode> available_moves;
	//get the index in terms of matrix
	int x = curr.indexx;
	int y = curr.indexy;
	int twoUpp = x - 2;
	int oneUpp = x - 1;
	int twoDown = x + 2;
	int oneDown = x + 1;
	int twoLeft = y - 2;
	int oneLeft = y - 1;
	int twoRight = y + 2;
	int oneRight = y + 1;

	//possible knightmoves: 1)valid index 2) marked as avaib 3)no pawn 4)not visited
	//2 up 1 left
	if (twoUpp >= 0 && oneLeft >= 0) {
		if (board[twoUpp][oneLeft].available && (!board[twoUpp][oneLeft].has_pawn) && (!board[twoUpp][oneLeft].visited)) {
			available_moves.push_back(board[twoUpp][oneLeft]);
		}
	}
	//2 up 1 right
	if (twoUpp >= 0 && oneRight < board.size()) {
		if (board[twoUpp][oneRight].available && (!board[twoUpp][oneRight].has_pawn) && (!board[twoUpp][oneRight].visited)) {
			available_moves.push_back(board[twoUpp][oneRight]);
		}
	}
	//2 down 1 left
	if (twoDown < board.size() && oneLeft >= 0) {
		if (board[twoDown][oneLeft].available && (!board[twoDown][oneLeft].has_pawn) && (!board[twoDown][oneLeft].visited)) {
			available_moves.push_back(board[twoDown][oneLeft]);
		}
	}

	//2 down 1 right
	if (twoDown < board.size() && oneRight < board.size()) {
		if (board[twoDown][oneRight].available && (!board[twoDown][oneRight].has_pawn) && (!board[twoDown][oneRight].visited)) {
			available_moves.push_back(board[twoDown][oneRight]);
		}
	}

	//1 up 2 right
	if (oneUpp >= 0 && twoRight < board.size()) {
		if (board[oneUpp][twoRight].available && (!board[oneUpp][twoRight].has_pawn) && (!board[oneUpp][twoRight].visited)) {
			available_moves.push_back(board[oneUpp][twoRight]);
		}
	}

	//1 up 2 left
	if (oneUpp >= 0 && twoLeft >= 0) {
		if (board[oneUpp][twoLeft].available && (!board[oneUpp][twoLeft].has_pawn) && (!board[oneUpp][twoLeft].visited)) {
			available_moves.push_back(board[oneUpp][twoLeft]);
		}
	}

	//1 down 2 right
	if (oneDown < board.size() && twoRight < board.size()) {
		if (board[oneDown][twoRight].available && (!board[oneDown][twoRight].has_pawn) && (!board[oneDown][twoRight].visited)) {
			available_moves.push_back(board[oneDown][twoRight]);
		}
	}
	//1 down 2 left
	if (oneDown < board.size() && twoLeft >= 0) {
		if (board[oneDown][twoLeft].available && (!board[oneDown][twoLeft].has_pawn) && (!board[oneDown][twoLeft].visited)) {
			available_moves.push_back(board[oneDown][twoLeft]);
		}
	}
	return available_moves;
}

bool findPath(vector<vector<cellNode>>& board, Stack<pawn>& knightmoves, int begx, int begy, int endx, int endy) {
	if (begx == endx && begy == endy) { //no need to search, already here
		return true;
	}
	cellNode firstCell= board[begx][begy];
	cellNode currCell = board[begx][begy];
	cellNode chosenCell;
	int index1, index2, initials;
	while (currCell.indexx != endx || currCell.indexy != endy) { //while not found
		vector<cellNode> available = available_knight(board, currCell);
		if ((currCell.x == firstCell.x && currCell.y == firstCell.y) && (available.size()==0)) {
			return false; //impossible
		}
		else { //continue searching
			if ((available.size() == 0) || ((currCell.unavaibs != 0) && (currCell.unavaibs == currCell.initialavaibs))) { //pop
				index1 = knightmoves.top_return().indexx;
				index2 = knightmoves.top_return().indexy;
				board[index1][index2].available = false;
				board[index1][index2].unavaibs = 0; //we are gonna pop this cell, so reset its unavaibs
				knightmoves.pop();
				index1 = knightmoves.top_return().indexx;
				index2 = knightmoves.top_return().indexy;
				board[index1][index2].unavaibs++; //this cells unavaibs increased by 1
			}
			else { //push to stack
				chosenCell = available[0];
				pawn new_pawn2(chosenCell.x, chosenCell.y);
				knightmoves.push(new_pawn2);
				index1 = new_pawn2.indexx;
				index2 = new_pawn2.indexy;
				board[index1][index2].visited = true;
				available = available_adjacents(board, board[index1][index2]);
				initials = available.size();
				board[index1][index2].initialavaibs = initials;	//update the number of initials		
				reset_avail(board);
			}
			index1 = knightmoves.top_return().indexx;
			index2 = knightmoves.top_return().indexy;
			currCell = board[index1][index2];
		}
	}
	return true;
}


int main() {
	int p; //n for matrix size, p for number of pawns
	cout << "Please enter the size of the board (n): ";
	cin >> n;
	int maxPawns = 3 * n;
	cout << "Please enter the number of the pawns (p): ";
	cin >> p;
	while (p > maxPawns) {
		cout << "Please enter the number of the pawns (p): ";
		cin >> p;
	}

	//board = each element is a cellNode
	vector<vector<cellNode>> board;
	for (int i = 0; i < n; i++) {
		vector<cellNode> rows;
		for (int j = 0; j < n; j++) {
			cellNode cell;
			cell.indexx = i; //in terms of vector index
			cell.indexy = j; //in terms of vector index
			cell.x = j + 1; //in terms of cartesian
			cell.y = n - i; //in terms of cartesian
			cell.has_pawn = false;
			cell.visited = false;
			cell.available = true;
			cell.initialavaibs = 0;
			cell.unavaibs = 0;

			cell.dontplace = false;
			rows.push_back(cell);
		}
		board.push_back(rows);
	}
	Stack<pawn> pawns;
	pawn firstPawn(1, 1);
	pawns.push(firstPawn);
	board[n-1][0].has_pawn = true;
	board[n-1][0].visited = true;

	//set (3,1) , (5,5) dontplaces true because we are gonna place knight and king!
	pawn knighttt(2, 1);
	pawn kinggg(6, 6);
	int xknight = knighttt.indexx;
	int yknight = knighttt.indexy;
	int xking = kinggg.indexx;
	int yking = kinggg.indexy;
	board[xknight][yknight].dontplace = true; //dont place, there is a knight
	board[xking][yking].dontplace = true; //dont place, there is a king

	int replaced = 1; //replaced the first pawn
	place_pawns(board, pawns, p, replaced); //replace the rest 	
	printMatrix(board);

	int knightx, knighty, kingx, kingy;
	cout << "Enter the position of knight(x,y): ";
	cin >> knightx >> knighty;
	cout << "Enter the position of king(x,y): ";
	cin >> kingx >> kingy;

	Stack<pawn> knightmoves;
	pawn knight(knightx, knighty);
	pawn king(kingx, kingy);
	
	knightmoves.push(knight); //push the initial coordinate of knight to knightmoves
	int beginx = knight.indexx;
	int beginy = knight.indexy;
	int endx = king.indexx;
	int endy = king.indexy;
	reset_avail(board); //reset availb and visited status
	reset_numbers(board);//reset no_unavaibs and initial avaibs to 0
	board[beginx][beginy].visited = true;

	bool finded = findPath(board, knightmoves, beginx, beginy, endx, endy);
	ofstream outfile("output.txt");
	outfile << "N = "<<n<< endl;
	outfile << "P = " << p << endl;
	outfile << endl;
	outfile << "Pawn Locations" << endl;
	while (!pawns.isEmpty()) {
		outfile << pawns.top_return().x << " " << pawns.top_return().y << endl;
		pawns.pop();
	}
	outfile << endl;
	outfile << "Knight = " <<knightx<<" "<<knighty<< endl;
	outfile << "King = " << kingx<<" "<<kingy<< endl;

	if(finded) {
		cout << "Path found, see the output file!" << endl;
		outfile << endl;
		Stack<pawn> reverse;
		pawn move;
		while (!knightmoves.isEmpty()) {
			move = knightmoves.top_return();
			reverse.push(move);
			knightmoves.pop();
		}
		while (!reverse.isEmpty()) {
			outfile << reverse.top_return().x << " " << reverse.top_return().y << endl;
			reverse.pop();
		}
	}
	else {
		cout << "No way!" << endl;
		outfile << endl;
		outfile << "No way!" << endl;
	}

	outfile << endl;
	for (int i = 0; i < board.size(); i++) {
		for (int j = 0; j < board.size(); j++) {
			if (board[i][j].has_pawn) {
				outfile << "1 ";
			}
			else {
				outfile << "0 ";
			}
		}
		if (i != board.size() - 1) {
			outfile << endl;
		}
	}
	outfile.close();
	return 0;
}