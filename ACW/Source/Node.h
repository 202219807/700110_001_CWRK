#pragma once

class Node
{
public:
    int x;
    int y;
    int gCost;
    int hCost;
    int fCost;
    bool blocked;
    Node* parent;

    Node(int x, int y);
};

class NodeCompare
{
public:
    bool operator() (const Node& a, const Node& b) const
    {
        return a.fCost > b.fCost;
    }
};