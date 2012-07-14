#include "AStarSearch.h"

/*
* Heuristic
*/
int AStarSearch::getManhattenDistance(Node a, Node b)
{
	return abs(a.x - b.x) + abs(b.x - b.y);
}

/*
* Add into frontier surrounding undiscovered cells which are not walls or rocks
* and are undiscovered
*/
void AStarSearch::addPossibleNeighbors(MineMap* m, Node &n, Node &dest, list<Node> &frontier, list<Node> &discovered)
{
	list<Point>* neighbours = new list<Point>(); //Get points from MineMap

	list<Point>::const_iterator ip;
	list<Node>::const_iterator in;
	for (ip = neighbours->begin(); ip != neighbours->end(); ip++)
	{
		//if point is discovered, just continue
		for (in = discovered.begin(); in != discovered.end(); in++)
			if ((*ip).x == (*in).x && (*ip).y == (*in).y)
				continue;

		Node newNode((*ip).x, (*ip).y);
		newNode.cost = n.cost + STEP_COST;
		newNode.heuristic = this->getManhattenDistance(newNode, dest);
		newNode.father_x = n.x;
		newNode.father_y = n.y;
		frontier.push_back(newNode);
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
			candidate.x = (*i).x;
			candidate.y = (*i).y;
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

	while (frontier->size())
	{
		Node n = getOptimalNode(*frontier);
		if (!isDestination(n, d))
		{
			//into frontier add possible neighbours
			addPossibleNeighbors(m, s, d, *frontier, *discovered);
			discovered->push_back(n);
			frontier->remove(n);
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
		tmp.x = tmp.father_x;
		tmp.y = tmp.father_y;
	}

	delete frontier;
	delete discovered;
}