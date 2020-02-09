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

void getChildNodes(Node &node, vector<int> goalState) { //CHANGED &NODE to NODE for new node creation
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

void printGrid(vector<int> grid) {
	for (int i = 0; i < 9; i++) {
		cout << grid[i];

		if ((i + 1) % 3 == 0) {
			cout << endl;
		}
		else {
			cout << " ";
		}
	}
}

//void printPathFromStart(const Node &startNode, const vector<int> goalState) {
//	Node currentNode = startNode;
//	printGrid(currentNode.grid);
//	cout << currentNode.childNodes.size();
//
//	while (currentNode.grid != goalState) {
//
//		for (Node* child : currentNode.childNodes) {
//			//if (child->f < currentNode.f) {
//			//	currentNode.grid = child->grid;
//			//}
//			vector<int> kid = child->grid;
//			printGrid(kid);
//		}
//		system("pause");
//	}
//
//}
////
////void retraceStepsRecursively(Node currentNodePtr, Node startNode, int count) {
////	if (currentNodePtr.grid == startNode.grid) {
////		return;
////	}
////
////	for (int i = 0; i < 9; i++) {
////		cout << currentNodePtr.parent->grid[i];
////	}
////	cout << endl;
////	cout << "Count: " << count << endl;
////	system("pause");
////
////	retraceStepsRecursively(*currentNodePtr.parent, startNode, ++count);
////	printGrid(currentNodePtr.grid);
////}

void retraceSteps(Node currentNode, vector<int> startGrid) {
	//Node tempNode = *currentNode->parent; 
	//stack<vector<int>> steps;
	//string key = getMapKey(currentNode);
	//currentNode = &closedList.find(key)->second; 
	//cout << "IN RETRACE: " << endl;

	//if (currentNode.grid == currentNode.parent->grid) {
	//	cout << "Child and parent match." << endl;
	//}
	//else {
	//	cout << "Parent and child are different." << endl;
	//}
	stack<vector<int>> steps;

	while (currentNode.grid != startGrid) {
		steps.push(currentNode.grid);
		currentNode = *currentNode.parent;
	}
	steps.push(currentNode.grid);

	while (!steps.empty()) {
		printGrid(steps.top());
		cout << endl;
		steps.pop();
	}


	//while (tempNode.grid != startNode.grid) {
	//	steps.push(tempNode.grid);
	//	cout << "Pushed to stack." << endl;

	//	for (int i = 0; i < 9; i++) {
	//		cout << tempNode.grid[i] << " ";
	//	}
	//	system("pause");
	//	tempNode = &(tempNode->parent);
	//}
	//steps.push(startNode.grid); 

	//cout << "Final startNode pushed to stack." << endl;
	//while (!steps.empty()) {
	//	printGrid(steps.top());
	//	steps.pop();
	//}

	return;
}

void Puzzle(vector<int> unsolvedPuzzle) {
	unordered_map<string, Node*> closedList;
	unordered_map<string, Node*> openListMap; //O(1) checker
	priority_queue<Node*, vector<Node*>, greater<Node*>> openListQueue; //organized by lowest F

	vector<int> goalState = setGoal(unsolvedPuzzle);  //establish goal state
	Node startState(unsolvedPuzzle, goalState, 0, nullptr); //create root node

	openListMap.insert({ getMapKey(startState), &startState }); //add starting state to openList O(1)
	openListQueue.push(&startState); //add starting state to openList Queue

	int steps = 0;
	while (!openListQueue.empty()) {
		//cout << "Steps: " << ++steps << endl;

		Node *currentNode = openListQueue.top(); //get unexpanded node with lowest f

		//openListQueue.pop(); //remove from unchecked list
		//openListMap.erase(getMapKey(currentNode)); //erases node from map


		if (currentNode->parent != NULL) {
			//cout << "Checking after getting top from queue: ";
			//if (currentNode->grid == currentNode->parent->grid) {
			//	cout << "Parent and child SAME." << endl;
			//}
			//else {
			//	cout << "Parent and child DIFFERENT." << endl;
			//}
		}

		//cout << "Checking children BEFORE LOOPS: " << endl;
		//for (Node* child : currentNode->childNodes) {
		//	printGrid(child->grid);
		//	cout << endl;
		//}

		openListQueue.pop(); //remove from unchecked list
		openListMap.erase(getMapKey(*currentNode)); //erases node from map
		//cout << "Printing from queue: " << endl;
		//cout << "CurrentState: " << endl;
		//printGrid(currentNode.grid);
		//if (currentNode.parent != NULL) {
		//	cout << "CurrentState parent: " << endl;
		//	printGrid(currentNode.parent->grid);
		//}

		string key = getMapKey(*currentNode);

		//cout << "Printing from Map: " << endl;
		//cout << "CurrentState: " << endl;
		//printGrid(currentNode.grid);
		//cout << "CurrentState parent: " << endl;
		//if (currentNode.parent != NULL) {
		//	printGrid(currentNode.parent->grid);
		//}

		if (currentNode->grid == goalState) {
			cout << "Puzzle has been solved!" << endl;
			//closedList.insert({ getMapKey(*currentNode), currentNode });
			//printPathFromStart(startState, goalState);
			//retraceStepsRecursively(currentNode, startState, 0);

			//startState and currentState (goalState) are correct - why do they have the same parent?
			//cout << "StartState: " << endl;
			//printGrid(startState.grid);
			//cout << "CurrentState: " << endl;
			//printGrid(currentNode.grid);
			//cout << "CurrentState parent: " << endl;
			//printGrid(currentNode.parent->grid); 

			retraceSteps(*currentNode, startState.grid); //go up tree through parents from goal state to start state
			return;
		}

		closedList.insert({ getMapKey(*currentNode), currentNode }); //add current node to checked list
		getChildNodes(*currentNode, goalState);

		for (Node *childNode : currentNode->childNodes) {
			//TEST - to see if children can correctly access parent node
			//cout << "CH: ";
			//for (int i = 0; i < 9; i++) {
			//	cout << childNode->grid[i] << " ";
			//}
			//cout << endl;
			//cout << "PA: ";
			//for (int i = 0; i < 9; i++) {
			//	cout << childNode->parent->grid[i] << " ";
			//}
			//cout << endl;
			//system("pause");
			//END TEST - PASS

			string childMapKey = getMapKey(*childNode);

			if (closedList.find(childMapKey) != closedList.end()) { //if child in closedList
				continue;
			}

			if (openListMap.find(childMapKey) == openListMap.end()) { //if child is not in openList; haven't seen this node at all
				
				openListMap.insert({ childMapKey, childNode });
				openListQueue.push(childNode);

				//Child and parent grids are different here!!
				//cout << "Child has been addd to open list. Checking child after added to list: " << endl;
				Node *test = openListMap.find(childMapKey)->second;
				//cout << "Child grid from MAP: " << endl;
				//printGrid(test.grid);
				//cout << "Parent grid from MAP: " << endl;
				//printGrid(test.parent->grid);


				//if (currentNode.parent != NULL) {
				//	cout << "Checking after adding new child to map: ";
				//	if (test.grid == test.parent->grid) {
				//		cout << "Parent and child SAME." << endl;
				//	}
				//	else {
				//		cout << "Parent and child DIFFERENT." << endl;
				//	}
				//}

				Node* testNode = openListQueue.top();

				//if (currentNode->parent != NULL) {
				//	cout << "Checking top after adding new child to queue: ";
				//	if (testNode->grid == testNode->parent->grid) {
				//		cout << "Parent and child SAME." << endl;
				//	}
				//	else {
				//		cout << "Parent and child DIFFERENT." << endl;
				//	}
				//}

			}
			else { //node has been seen previously
				//cout << "Node has been seen previously." << endl;

				//unordered_map<string, Node>::iterator iter = openListMap.find(childMapKey);

				Node* firstInstanceOfNode = openListMap.find(childMapKey)->second;
				//use an iterator here instead? Memory error?

				if (childNode->depth < firstInstanceOfNode->depth) {

					//TEST - are children and parent node different?
					//cout << "Firstinstance parent before: ";
					//for (int i = 0; i < 9; i++) {
					//	cout << firstInstanceOfNode->parent->grid[i] << " ";
					//}
					//cout << "Child parent before: ";
					//for (int i = 0; i < 9; i++) {
					//	cout << childNode->parent->grid[i] << " ";
					//}
					//cout << endl;
					//cout << "BEFORE: First d: " << firstInstanceOfNode->depth << " First f: " << firstInstanceOfNode->f << endl;
					//cout << "Child d: " << childNode->depth << " Child f: " << childNode->f << endl;
					//cout << "change in depth" << endl;
					//END

					firstInstanceOfNode->depth = childNode->depth;
					firstInstanceOfNode->f = childNode->f;
					firstInstanceOfNode->parent = childNode->parent;

					//openListQueue.push(firstInstanceOfNode); //note that push inserts and resorts PQ

					//TEST
					//cout << "Firstinstance parent after: ";
					//for (int i = 0; i < 9; i++) {
					//	cout << firstInstanceOfNode->parent->grid[i] << " ";
					//}
					//cout << "Child parent after: ";
					//for (int i = 0; i < 9; i++) {
					//	cout << childNode->parent->grid[i] << " ";
					//}
					//cout << endl;
					//cout << "AFTER: First d: " << firstInstanceOfNode->depth << " First f: " << firstInstanceOfNode->f << endl;
					//cout << "Child d: " << childNode->depth << " Child f: " << childNode->f << endl;

					//system("pause");
					//END

				}

			}
		}
	}
}

int main() {

	vector<vector<int>> unsolvedPuzzles = getUnsolvedPuzzles("program_1_data.txt");

	for (vector<int> unsolvedPuzzle : unsolvedPuzzles) {
		for (int i = 0; i < 9; i++) {
			cout << unsolvedPuzzle[i] << " ";
			
		}
		if (isSolveable(unsolvedPuzzle)) {
			cout << "This puzzle is solveable: " << endl;
			Puzzle(unsolvedPuzzle);
		}
		else {
			cout << "This puzzle is not solveable. Would you like to try another one?" << endl;
			system("pause");
		}
	}

	//vector<int> unsolvedPuzzle = { 1, 8, 2, 0, 4, 3, 7, 6, 5 };
	//vector<int> puz = { 8, 1, 2, 0, 4, 3, 7, 6, 5 };
	//vector<int> oneOff = { 1, 2, 3, 4, 5, 6, 7, 0, 8 };
	//vector<int> correct = { 1, 2, 3, 4, 5, 6, 7, 8, 0 };

	return 0;
}


