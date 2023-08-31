#include "Pathfinder.h"
#include <queue>
#include <unordered_map>

std::vector<Node> Pathfinder::findPath(int startX, int startY, int endX, int endY)
{
    // Clear any previous data in the open list and visited map
    std::priority_queue<Node, std::vector<Node>, NodeCompare> openList;
    std::unordered_map<int, int> visited;

    // Get the starting node and add it to the open list
    Node startNode = m_map[getNodeIndex(startX, startY)];
    startNode.gCost = 0;
    startNode.hCost = calculateManhattanDistance(startX, startY, endX, endY);
    startNode.fCost = startNode.gCost + startNode.hCost;
    openList.push(startNode);

    // While the open list is not empty
    while (!openList.empty())
    {
        // Get the node with the lowest fCost from the open list
        Node currentNode = openList.top();
        openList.pop();

        // Mark the node as visited
        visited[getNodeIndex(currentNode.x, currentNode.y)] = 1;

        // If the current node is the end point, reconstruct the path and return it
        if (currentNode.x == endX && currentNode.y == endY)
        {
            std::vector<Node> path;
            // reconstruct the path and return it
            return path;
        }

        // Add unvisited and unblocked neighbors to the open list
        addNeighborsToOpenList(currentNode, endX, endY);
    }

    // If the open list is empty and the end point has not been reached, return an empty vector
    return std::vector<Node>();
}

void Pathfinder::addNeighborsToOpenList(Node currentNode, int endX, int endY)
{
    // Check north, south, east, and west neighbors
    int dx[4] = { 0, 0, 1, -1 };
    int dy[4] = { 1, -1, 0, 0 };

    for (int i = 0; i < 4; i++)
    {
        int x = currentNode.x + dx[i];
        int y = currentNode.y + dy[i];

        // Check if the neighbor is within the map bounds and not blocked
        if (x >= 0 && x < m_mapWidth && y >= 0 && y < m_mapHeight && !m_map[getNodeIndex(x, y)].blocked)
        {
            Node neighbor = m_map[getNodeIndex(x, y)];
            // Check if the neighbor has been visited
            // if (visited.count(getNodeIndex(x, y)) == 0)
            {
                // Set the gCost, hCost, and fCost for the neighbor
                neighbor.gCost = currentNode.gCost + 1;
                neighbor.hCost = calculateManhattanDistance(x, y, endX, endY);
                neighbor.fCost = neighbor.gCost + neighbor.hCost;
                // Add the neighbor to the open list
                // openList.push(neighbor);
            }
        }
    }
}

int Pathfinder::calculateManhattanDistance(int x1, int y1, int x2, int y2)
{
    // Calculate the Manhattan distance between two points
    return abs(x1 - x2) + abs(y1 - y2);
}

int Pathfinder::getNodeIndex(int x, int y)
{
    // Return the index of a node in the map vector based on its x and y coordinates
    return y * m_mapWidth + x;
}