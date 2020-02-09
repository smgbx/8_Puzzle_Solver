/*
*Shelby Mohar
*CS 461 Spring 2020
*Program 1 - 8 Puzzle
*
*A program that takes a text file of unsolved 8 puzzles. If the puzzle is solveable, it will print each step require to solve it.
*/
#include <iostream>
#include <fstream>
#include <map>
#include <unordered_map>
#include <queue>
#include <stack>
#include <cstdlib>
#include "Node.h"
using namespace std;

bool isSolveable(vector<int> puzzleArray) {
	int inversions = 0;

	for (int i = 0; i < 9; ++i) {
		for (int j = i + 1; j < 9; ++j) {
			if ((puzzleArray[i] > puzzleArray[j]) && (puzzleArray[i] && puzzleArray[j])) { //if either digit is a 0, will not count
				++inversions;
			}
		}
	}

	return !(inversions % 2);
}

vector<vector<int>> getUnsolvedPuzzles(string fileOfPuzzles) {
	ifstream fin;
	fin.open(fileOfPuzzles);
	if (!fin) {
		cout << "Unable to open file." << endl;
		system("pause");
		exit(1);
	}

	int digit;
	string data;
	vector<int> tempGrid;
	vector<vector<int>> gridsToBeSolved;

	while (fin >> digit) {
		data += digit;
	}

	for (int i = 0; i < data.length(); i++) {
		if (tempGrid.size() == 9) {
			gridsToBeSolved.push_back(tempGrid);
			tempGrid.clear();
		}
		digit = (int)data.at(i);
		tempGrid.push_back(digit);
	}

	fin.close();

	return gridsToBeSolved;
}

vector<int> setGoal(vector<int> startState) {
	sort(startState.begin(), startState.end());
	vector<int> goalState;

	for (int i = 0; i < 9; i++) {
		if (startState[i] == 0) {
			continue;
		}
		else {
			goalState.push_back(startState[i]);
		}
	}
	goalState.push_back(0);

	return goalState;
}

string getMapKey(Node node) {
	string key = "";

	for (int i = 0; i < 9; i++) {
		key += to_string(node.grid[i]);
	}

	return key;
}

void getChildNodes(Node &node, vector<int> goalState) { 
	int blankIndex = 0;
	vector<int> tempChildGrid;

	for (int i = 0; i < 9; i++) {
		if (node.grid[i] == 0) {
			blankIndex = i;
		}
	}

	if (blankIndex - 3 >= 0) {
		tempChildGrid = node.grid;
		tempChildGrid[blankIndex] = tempChildGrid[blankIndex - 3];
		tempChildGrid[blankIndex - 3] = 0;
		
		node.childNodes.push_back(new Node(tempChildGrid, goalState, node.depth, &node));
	}
	if (blankIndex + 3 <= 8) {
		tempChildGrid = node.grid;
		tempChildGrid[blankIndex] = tempChildGrid[blankIndex + 3];
		tempChildGrid[blankIndex + 3] = 0;

		node.childNodes.push_back(new Node(tempChildGrid, goalState, node.depth, &node));
	}
	if ((blankIndex - 1 >= 0) && (blankIndex % 3 != 0)) {
		tempChildGrid = node.grid;
		tempChildGrid[blankIndex] = tempChildGrid[blankIndex - 1];
		tempChildGrid[blankIndex - 1] = 0;

		node.childNodes.push_back(new Node(tempChildGrid, goalState, node.depth, &node));
	}
	if ((blankIndex + 1 <= 8) && (blankIndex % 3 != 2)) {
		tempChildGrid = node.grid;
		tempChildGrid[blankIndex] = tempChildGrid[blankIndex + 1];
		tempChildGrid[blankIndex + 1] = 0;

		node.childNodes.push_back(new Node(tempChildGrid, goalState, node.depth, &node));
	}
}

void printGrid(vector<int> grid, ofstream &fout) {
	for (int i = 0; i < 9; i++) {
		fout << grid[i];
		cout << grid[i];

		if ((i + 1) % 3 == 0) {
			fout << endl;
			cout << endl;
		}
		else {
			fout << " ";
			cout << " ";
		}
	}
}

void writeResults() {


}

void retraceSteps(Node currentNode, vector<int> startGrid, ofstream &fout) {

	stack<vector<int>> steps;
	fout << "Moves to solve puzzle ";
	cout << "Moves to solve puzzle ";

	while (currentNode.grid != startGrid) {
		steps.push(currentNode.grid);
		currentNode = *currentNode.parent;
	}
	steps.push(currentNode.grid);
	fout << "(" << steps.size() << ") :" << endl;
	cout << "(" << steps.size() << ") :" << endl;

	while (!steps.empty()) {
		printGrid(steps.top(), fout);
		fout << endl;
		cout << endl;
		steps.pop();
	}

	return;
}

void Puzzle(vector<int> unsolvedPuzzle, ofstream &fout) {
	unordered_map<string, Node*> closedList;
	unordered_map<string, Node*> openListMap; //O(1) checker
	priority_queue<Node*, vector<Node*>, greater<Node*>> openListQueue; //organized by lowest F

	vector<int> goalState = setGoal(unsolvedPuzzle);  //establish goal state
	Node startState(unsolvedPuzzle, goalState, 0, nullptr); //create root node

	openListMap.insert({ getMapKey(startState), &startState }); //add starting state to openList O(1)
	openListQueue.push(&startState); //add starting state to openList Queue

	int steps = 0;
	while (!openListQueue.empty()) {

		Node *currentNode = openListQueue.top(); //get unexpanded node with lowest f
		openListQueue.pop(); //remove from unchecked list
		openListMap.erase(getMapKey(*currentNode)); //erases node from map

		string key = getMapKey(*currentNode);

		if (currentNode->grid == goalState) {
			retraceSteps(*currentNode, startState.grid, fout); //go up tree through parents from goal state to start state
			return;
		}

		closedList.insert({ getMapKey(*currentNode), currentNode }); //add current node to checked list
		getChildNodes(*currentNode, goalState);

		for (Node *childNode : currentNode->childNodes) {
			
			string childMapKey = getMapKey(*childNode);

			if (closedList.find(childMapKey) != closedList.end()) { //if child in closedList
				continue;
			}

			if (openListMap.find(childMapKey) == openListMap.end()) { //if child is not in openList; haven't seen this node at all
				
				openListMap.insert({ childMapKey, childNode });
				openListQueue.push(childNode);

			}
			else { //node has been seen previously

				Node* firstInstanceOfNode = openListMap.find(childMapKey)->second;

				if (childNode->depth < firstInstanceOfNode->depth) {

					firstInstanceOfNode->depth = childNode->depth;
					firstInstanceOfNode->f = childNode->f;
					firstInstanceOfNode->parent = childNode->parent;
				}

			}
		}
	}
}

void EightPuzzleSolver(string filename) {
	vector<vector<int>> unsolvedPuzzles = getUnsolvedPuzzles(filename);

	ofstream fout;
	fout.open("puzzle_results.txt");

	if (!fout) {
		cout << "Unable to open output file." << endl;
		exit(1);
	}

	int puzzle = 0;

	for (vector<int> unsolvedPuzzle : unsolvedPuzzles) {

		fout << "Unsolved puzzle " << ++puzzle << "/" << unsolvedPuzzle.size() << " :" << endl;
		cout << "Unsolved puzzle " << puzzle << "/" << unsolvedPuzzle.size() << " :" << endl;

		printGrid(unsolvedPuzzle, fout);

		fout << endl;
		cout << endl;

		if (isSolveable(unsolvedPuzzle)) {
			fout << "This puzzle is solveable. Please hold..." << endl;
			cout << "This puzzle is solveable. Please hold..." << endl;
			Puzzle(unsolvedPuzzle, fout);
		}
		else {
			fout << "This puzzle is not solveable." << endl;
			cout << "This puzzle is not solveable." << endl;
		}

		system("pause");
		fout << endl;
		cout << endl;
	}

	fout.close();

	return;
}

int main() {

	//Enter file name of puzzles to solve
	EightPuzzleSolver("program_1_data.txt");

	return 0;
}


