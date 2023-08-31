#include "Node.h"

Node::Node(int x, int y)
    : x(x), y(y), gCost(0), hCost(0), fCost(0), blocked(false), parent(nullptr)
{}

