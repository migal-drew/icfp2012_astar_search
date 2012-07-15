#include "AStarSearch.h"
#include <iostream>

using namespace std;

/*
* Heuristic
*/
int AStarSearch::getManhattenDistance(Node a, Node b)
{
	return (a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y);
}

/*
* Add into frontier surrounding undiscovered cells which are not walls or rocks
* and are undiscovered
*/
bool AStarSearch::addPossibleNeighbors(MineMap* m, Node &n, Node &dest,
	list<Node> &frontier, list<Node> &discovered, TCheckFunction checkFunc, char* forbidCells)
{
	bool isAdded = false;
	list<Point>* neighbours = new list<Point>();
	//Get points from MineMap
	m->GetListOrthogonalPoints(*neighbours, Point(n.x, n.y), checkFunc, forbidCells, false);

	list<Point>::const_iterator ip;
	list<Node>::const_iterator in;
	for (ip = neighbours->begin(); ip != neighbours->end(); ip++)
	{
		bool inDiscovered = false;
		bool inFrontier = false;
		//Verify that point already discovered
		
		for (in = discovered.begin(); in != discovered.end(); in++)
			if ((*ip).x == (*in).x && (*ip).y == (*in).y)
			{
				inDiscovered = true;
				break;
			}

		for (in = frontier.begin(); in != frontier.end(); in++)
			if ((*ip).x == (*in).x && (*ip).y == (*in).y)
			{
				inFrontier = true;
				break;
			}

		if (!inFrontier && !inDiscovered) 
		{
			Node newNode((*ip).x, (*ip).y);
			newNode.cost = n.cost + STEP_COST;
			newNode.heuristic = this->getManhattenDistance(newNode, dest);
			newNode.father_x = n.x;
			newNode.father_y = n.y;
			frontier.push_back(newNode);
			isAdded = true;
		}
	}

	return isAdded;
}

/*
* returns node with the minimal estimation (cost + heuristic)
*/
Node AStarSearch::getOptimalNode(list<Node> &nodes)
{
	list<Node>::const_iterator i;
	int best = 2147483647;
	Node candidate;

	for (i = nodes.begin(); i != nodes.end(); i++) 
	{
		if ((*i).cost + (*i).heuristic < best)
		{
			candidate = (*i);
			best = candidate.cost + candidate.heuristic;
		}
	}

	return candidate;
}

bool isDestination(Node &p, Node &dest)
{
	return p.x == dest.x && p.y == dest.y;
}

bool isStart(Node &p, Node &start)
{
	return p.x == start.x && p.y == start.y;
}

void AStarSearch::removeNode(Node &n, list<Node> &nodes)
{
	list<Node>::iterator i;
	for (i = nodes.begin(); i != nodes.end(); i++)
		if ((*i).x == n.x && (*i).y == n.y)
		{
			nodes.erase(i);
			return;
		}
}

Node AStarSearch::getNode(int x, int y, list<Node> &nodes)
{
	list<Node>::iterator i;
	for (i = nodes.begin(); i != nodes.end(); i++)
		if ((*i).x == x && (*i).y == y)
			return (*i);

	return Node(-1, -1);
}

void AStarSearch::getRoute(MineMap* m, Point &start, Point &dest, list<Point> &route,
	TCheckFunction checkFunc, char* forbidCells)
{
	list<Node>* frontier = new list<Node>();
	list<Node>* discovered = new list<Node>();

	//Destination
	Node d = Node(dest.x, dest.y);
	//Start position
	Node s = Node(start.x, start.y);
	//Initialization
	s.heuristic = this->getManhattenDistance(s, d);
	d.heuristic = 0;
	//Add neighbors for start node
 	this->addPossibleNeighbors(m, s, d, *frontier, *discovered, checkFunc, forbidCells);
	discovered->push_back(s);

	bool frontierChanged = true;
	bool success = false;
	int prevSize = -1;

	int i = 0;
	while (frontier->size() && (frontierChanged))
	{
		/*
		int fr = frontier->size();
		int di = discovered->size();
		cout << "Iteration# " << i++ << ": " << "fr " << fr << "di " << di << "all " << fr + di << endl;
		*/
		Node n = getOptimalNode(*frontier);
		if (!isDestination(n, d))
		{
			prevSize = frontier->size();
			//into frontier add possible neighbours
			bool isAdded = addPossibleNeighbors(m, n, d, *frontier, *discovered, checkFunc, forbidCells);
			this->removeNode(n, *frontier);
			discovered->push_back(n);
			bool isSizeChanged = prevSize != frontier->size();
			frontierChanged = isAdded || isSizeChanged;
		}
		else
		{
			//We found our goal!
			d.cost = n.cost;
			d.heuristic = n.heuristic;
			d.father_x = n.father_x;
			d.father_y = n.father_y;
			success = true;

			break;
		}

	}

	if (success)
	{
		Node tmp = d;
		//Reconstructing path
		while(!isStart(tmp, s))
		{
			route.push_front(Point(tmp.x, tmp.y));
			tmp = this->getNode(tmp.father_x, tmp.father_y, *discovered);
		}
	}

	delete frontier;
	delete discovered;
}