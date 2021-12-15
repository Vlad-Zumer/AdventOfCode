#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <unordered_map>
#include <queue>
#include "../utils/cpp-utils.hpp"
using namespace std;
using ull = unsigned long long;

struct Node
{
    struct Comparer
    {
        bool operator() (const Node& lhs, const Node& rhs)
        {
            return lhs.fScore > rhs.fScore;
        }
    };

    int x=-1,y=-1,val=-1;
    ull fScore = ULLONG_MAX;
    ull gScore = ULLONG_MAX;

    vector<Node> GetNeighboursDirs(const int maxX, const int maxY) const
    {
        vector<Node> dirs;
        for (int i = -1; i <= 1 ; i++)
        {
            for (int j = -1; j <= 1 ; j++)
            {
                if (i == 0 ^ j == 0)
                {
                    if (y+i >= 0 && y+i < maxY &&
                        x+j >= 0 && x+j < maxX)
                    {
                        Node n;
                        n.x = x+j;
                        n.y = y+i;
                        dirs.push_back(n);
                    }
                }
            }
        }
        return dirs;
    }

    bool operator==(const Node& other) const
    {
        return x==other.x && y==other.y && val==other.val;
    }
};

ostream& operator<< (ostream& os, const Node n)
{
    os<<"Node: "<<"{x: "<<n.x<<", y: "<<n.y<<", val: "<<n.val<<"}";
    return os;
}

namespace std {
    template<>
    struct hash<Node>
    {
        size_t operator()(const Node& node) const
        {
            size_t outVal = (node.x^node.y) << node.val;
            return outVal;
        }
    };
}


vector<vector<Node>> parseInput()
{
    ifstream inFile("./inputs/day-15.input");
    // ifstream inFile("./inputs/test.input");
    string strLine;
    vector<vector<Node>> cave = {};
    while (getline(inFile,strLine))
    {
        cave.push_back({});
        for (size_t i = 0; i < strLine.size(); i++)
        {
            Node node;
            node.val = strLine[i]-'0';
            node.x = i;
            node.y = cave.size()-1;
            cave.back().push_back(node);
        }
    }

    return cave;
}

ull calculateHeuristic(const Node node, const Node dest)
{
    return llabs(dest.x-node.x) + llabs(dest.y-node.y);
}

vector<Node> A_Star(vector<vector<Node>> nodes, Node start, Node end)
{
    int maxX, maxY;
    maxY=nodes.size();
    maxX=nodes[0].size();

    unordered_map<Node,Node> cameFrom = {};
    priority_queue<Node,vector<Node>, Node::Comparer> queue; 

    for (size_t i = 0; i < nodes.size(); i++)
    {
        for (size_t j = 0; j < nodes[i].size(); j++)
        {
            nodes[i][j].gScore= ULLONG_MAX;
            nodes[i][j].fScore = ULLONG_MAX;
        }
    }

    Node& startNode = nodes[start.y][start.x];
    
    startNode.gScore = 0;
    startNode.fScore = calculateHeuristic(start,end);
    queue.push(startNode);

    while (queue.size() > 0)
    {
        const Node Current = queue.top();

        if (Current == end)
        {
            vector<Node> res;
            Node aux = Current;
            res.insert(res.begin(),aux);
            while (cameFrom.find(aux) != cameFrom.end())
            {
                aux = cameFrom[aux];
                res.insert(res.begin(),aux);
            }

            return res;
        }

        queue.pop();

        vector<Node> neighbourDirs = Current.GetNeighboursDirs(maxX, maxY);
        for (const auto n : neighbourDirs)
        {
            Node& neighbour = nodes[n.y][n.x];
            ull tentativeGScore = Current.gScore + neighbour.val;
            if (tentativeGScore < neighbour.gScore)
            {
                cameFrom[neighbour] = Current;
                neighbour.gScore = tentativeGScore;
                neighbour.fScore = tentativeGScore + calculateHeuristic(neighbour,end);
                queue.push(neighbour);
            }
        }
    }
    
    assert(false);
    return {};
}

ull part1(vector<vector<Node>> cave)
{
    Node start = cave[0][0];
    Node end = cave.back().back();
    vector<Node> path = A_Star(cave, start, end);

    ull sum = 0;
    for (size_t i = 1; i < path.size(); i++)
    {
        sum+=path[i].val;
    }
    return sum;
}

vector<vector<Node>> expand(vector<vector<Node>> cave)
{
    int maxX, maxY;
    maxY=cave.size();
    maxX=cave[0].size();
    vector<vector<Node>> retVal(maxY*5, vector<Node>(maxX*5));
    for (size_t i = 0; i < maxY*5; i++)
    {
        int offsetY = i / maxY;
        for (size_t j = 0; j < maxX*5; j++)
        {
            int offsetX = j / maxX;
            if(offsetX == 0 && offsetY == 0)
            {
                retVal[i][j] = cave[i][j];
            }
            else
            {
                Node n;
                n = cave[i % maxY][j % maxX];
                n.x = j;
                n.y = i;
                n.val += offsetY + offsetX;
                if(n.val > 9)
                {
                    n.val -= 9;
                }
                retVal[i][j] = n;
            }
        }
    }
    return retVal;
}

ull part2(vector<vector<Node>> cave)
{
    cave = expand(cave);
    return part1(cave);
}

int main()
{
    vector<vector<Node>> input = parseInput();
    cout<<"Part 1:"<<part1(input)<<endl;
    cout<<"Part 2:"<<part2(input)<<endl;

    return 0;
}