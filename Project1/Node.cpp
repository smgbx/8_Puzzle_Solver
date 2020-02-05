#include "Node.h"

bool operator < (const Node& lhs, const Node& rhs) {
	return lhs.f < rhs.f;
}

bool operator > (const Node& lhs, const Node& rhs) {
	return lhs.f > rhs.f;
}

Node::Node(vector<int> newGrid, vector<int> goalGrid, int parentDepth, Node* parentAddress = NULL) {
	this->parent = parentAddress;
	this->depth = parentDepth + 1;
	this->grid = newGrid; 
	this->heuristic = getHeuristic(goalGrid);
	this->f = this->depth + this->heuristic;
}

//int Node::getF() const {
//	int f = depth + heuristic;
//	return f;
//}
//
//void Node::setF(int f) {
//	this->f = f;
//	return;
//}

int Node::getHeuristic(vector<int> goal) const { 
	int moves = 0;
	int distance = 0;

	for (int i = 0; i < 9; i++) {
		if (this->grid[i] == 0) {
			continue;
		}
		if (this->grid[i] != (i + 1)) {
			distance = this->grid[i] - (i + 1);
			if (distance > 0) { //positive difference, so moving forward
				while (distance > 2) { //down
					distance -= 3;
					++moves;
				}
				while (distance > 0) {
					distance -= 1;
					++moves;
				}
			}
			else { //negative difference, so moving backward
				while (distance < -2) { //up
					distance += 3;
					++moves;
				}
				while (distance < 0) {
					distance += 1;
					++moves;
				}
			}
		}
	}

	return moves;
}

