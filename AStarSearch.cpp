#include "AStarSearch.h"

/*
* Heuristic
*/
int AStarSearch::getManhattenDistance(Node a, Node b)
{
	return abs(a.x - b.x) + abs(a.y - b.y);
}

/*
* Add into frontier surrounding undiscovered cells which are not walls or rocks
* and are undiscovered
*/
void AStarSearch::addPossibleNeighbors(MineMap* m, Node &n, Node &dest, list<Node> &frontier, list<Node> &discovered)
{
	list<Point>* neighbours = new list<Point>();
	//Get points from MineMap
	m->GetListOrthogonalPoints(*neighbours, Point(n.x, n.y), NULL, "*#", false);

	list<Point>::const_iterator ip;
	list<Node>::const_iterator in;
	for (ip = neighbours->begin(); ip != neighbours->end(); ip++)
	{
		bool isExist = false;
		//Verify that point already discovered
		for (in = discovered.begin(); in != discovered.end(); in++)
			if ((*ip).x == (*in).x && (*ip).y == (*in).y)
				isExist = true;

		if (!isExist) 
		{
			Node newNode((*ip).x, (*ip).y);
			newNode.cost = n.cost + STEP_COST;
			newNode.heuristic = this->getManhattenDistance(newNode, dest);
			newNode.father_x = n.x;
			newNode.father_y = n.y;
			frontier.push_back(newNode);
		}
	}
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

void AStarSearch::getRoute(MineMap* m, Point &start, Point &dest, list<Point> &route)
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
 	this->addPossibleNeighbors(m, s, d, *frontier, *discovered);
	discovered->push_back(s);

	while (frontier->size())
	{
		Node n = getOptimalNode(*frontier);
		if (!isDestination(n, d))
		{
			//into frontier add possible neighbours
			addPossibleNeighbors(m, n, d, *frontier, *discovered);
			this->removeNode(n, *frontier);
			discovered->push_back(n);
			//this->removeNode(n, *frontier);
		}
		else
		{
			d.cost = n.cost;
			d.heuristic = n.heuristic;
			d.father_x = n.father_x;
			d.father_y = n.father_y;
			break;
			/*
			if (final.cost == 0)
			{
				final = n;
				discovered->push_back(n);
			}
			//If optimal node is destination again
			else
			{
				if (final.cost == n.cost)
					break;
			}
			*/
		}

	}

	Node tmp = d;
	//Reconstructing path
	while(!isStart(tmp, s))
	{
		route.push_front(Point(tmp.x, tmp.y));
		tmp = this->getNode(tmp.father_x, tmp.father_y, *discovered);
	}

	delete frontier;
	delete discovered;
}