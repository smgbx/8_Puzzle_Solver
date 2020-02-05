#include <vector>
#include <algorithm>
#include <iostream>
#include <map>
#include <unordered_map>
#include <queue>
#include <string>
#include "Node.h"
using namespace std;

bool isSolveable(int puzzleArray[]) {
	int inversions = 0;

	for (int i = 0; i < 9; ++i) {
		for (int j = 0; j <= 9; ++j) {
			if ((puzzleArray[i] == 0 && i != 9) || puzzleArray[i] > puzzleArray[j]) {
				++inversions;
			}
		}
	}

	return !(inversions % 2);
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
	int blankIndex;
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

void Puzzle(vector<int> unsolvedPuzzle) {
	unordered_map<string, Node> closedList;
	unordered_map<string, Node> openListMap; //O(1) checker
	priority_queue<Node, vector<Node>, greater<Node>> openListQueue; //organized by lowest F
	vector<int> goalState = setGoal(unsolvedPuzzle);  //establish goal state
	Node startState(unsolvedPuzzle, goalState, 0, NULL); //create root node

	openListMap.insert({ getMapKey(startState), startState }); //add starting state to openList O(1)
	openListQueue.push(startState); //add starting state to openList Queue

	while (!openListQueue.empty()) {

		Node currentNode = openListQueue.top(); //get unexpanded node with lowest f

		openListQueue.pop(); //remove from unchecked list
		openListMap.erase(getMapKey(currentNode)); //erases node from map

		if (currentNode.grid == goalState) {
			cout << "Puzzle has been solved!" << endl;
			//cout << retracePath(currentNode);
			return;
		}

		closedList.insert({ getMapKey(currentNode), currentNode }); //add current node to checked list
		getChildNodes(currentNode, goalState);

		for (Node *childNode : currentNode.childNodes) {
			vector<int> childGrid = childNode->grid;
			string childMapKey = getMapKey(*childNode);

			if (closedList.find(childMapKey) != closedList.end()) { //if child in closedList
				continue;
			}

			if (openListMap.find(childMapKey) == openListMap.end()) { //if child is not in openList; haven't seen this node at all

				openListMap.insert({ childMapKey, *childNode });
				openListQueue.push(*childNode);

			}
			else { //node has been seen previously
				Node firstInstanceOfNode = openListMap.find(childMapKey)->second; 

				if (childNode->depth < firstInstanceOfNode.depth) {
					firstInstanceOfNode.depth = childNode->depth;
					firstInstanceOfNode.parent = childNode->parent;
				}
			}
		}
	}
}


int main() {
	vector<int> unsolvedPuzzle = { 1, 8, 2, 0, 4, 3, 7, 6, 5 };
	vector<int> oneOff = { 1, 2, 3, 4, 5, 6, 7, 0, 8 };
	vector<int> correct = { 1, 2, 3, 4, 5, 6, 7, 8, 0 };
	
	Puzzle(unsolvedPuzzle);

	system("pause");

	return 0;
}


