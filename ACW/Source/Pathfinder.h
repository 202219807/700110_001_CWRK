#pragma once
#include <vector>
#include <unordered_map>
#include <queue>
#include "Node.h"

class Pathfinder
{
public:
    Pathfinder(int, int) {};
    std::vector<Node> findPath(int, int, int, int);
    std::unordered_map<int, int> visited;
    std::priority_queue<Node, std::vector<Node>, NodeCompare> openList;

private:
    int m_mapWidth;
    int m_mapHeight;
    std::vector<Node> m_map;

    void addNeighborsToOpenList(Node, int, int);
    int calculateManhattanDistance(int, int, int, int);
    int getNodeIndex(int, int);
};

