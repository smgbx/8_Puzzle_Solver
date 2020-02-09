#include <vector>
#include <string>
using namespace std;

class Node {
public:
	Node* parent;
	vector<int> grid;
	int depth; //our G value
	int heuristic; //our H value
	int f;
	vector<Node*> childNodes;

	friend bool operator < (const Node& lhs, const Node& rhs);
	friend bool operator > (const Node& lhs, const Node& rhs);

	Node(vector<int> newGrid, vector<int> goalGrid, int parentDepth, Node *parent);

private:
	int getHeuristic(vector<int> goalGrid) const;
};

