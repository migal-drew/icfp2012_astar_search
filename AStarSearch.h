/*
* Class for searching optimal possible path between two points
*/
#include "Point.h"
#include "MineMap.h"
#include <list>
#include <math.h>

#define STEP_COST 1

using namespace std;

class Node
{
public:
	int x;
	int y;
	int cost;
	int heuristic;
	
	int father_x;
	int father_y;

	Node()
	{
		this->x = 0;
		this->y = 0;
		this->cost = 0;
		this->heuristic = 0;
	}

	Node(int x, int y)
	{
		this->x = x;
		this->y = y;
		this->cost = 0;
		this->heuristic = 0;
	}

	Node(const Node& n)
	{
		this->x = n.x;
		this->y = n.y;
		this->cost = n.cost;
		this->heuristic = n.heuristic;
		this->father_x = n.father_x;
		this->father_y = n.father_y;
	}
};

class AStarSearch
{
public:
	int getManhattenDistance(Node a, Node b);
	void getRoute(MineMap* m, Point &start, Point &dest, list<Point> &route);
	void addPossibleNeighbors(MineMap* m, Node &n, Node &dest, list<Node> &frontier, list<Node> &discovered);
	Node getOptimalNode(list<Node> &nodes);
	void removeNode(Node &n, list<Node> &frontier);
	Node getNode(int x, int y, list<Node> &nodes);
};