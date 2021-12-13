#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include "../utils/cpp-utils.hpp"
using namespace std;
using ull = unsigned long long;

typedef struct Node
{
    string Id="";
    vector<string> neighbours={};
    bool isSmallNode = false;
} Node;

bool isLowerCase(const string s)
{
    for (size_t i = 0; i < s.size(); i++)
    {
        if(s[i]>= 'A' && s[i]<='Z')
        {
            return false;
        }
    }
    return true;
}

map<string,Node> parseInput()
{
    map<string,Node> retVal = {};
    ifstream inFile("./inputs/day-12.input");
    // ifstream inFile("./inputs/test.input");
    string strLine;
    while (getline(inFile,strLine))
    {
        vector<string> conn = Utils::str_split(strLine,"-");
        retVal[conn[0]].Id = conn[0];
        retVal[conn[0]].neighbours.push_back(conn[1]);
        retVal[conn[0]].isSmallNode = isLowerCase(conn[0]);
        retVal[conn[1]].Id = conn[1];
        retVal[conn[1]].neighbours.push_back(conn[0]);
        retVal[conn[1]].isSmallNode = isLowerCase(conn[1]);
    }
    return retVal;
}

ull part1(string node, map<string,Node> nodeMap, set<string> seenSmalls)
{
    if (node == "end")
    {
        return 1;
    }

    if(seenSmalls.find(node) != seenSmalls.end())
    {
        return 0;
    }

    Node n = nodeMap[node];
    if(n.isSmallNode)
    {
        seenSmalls.insert(node);
    }

    ull sum = 0;
    for (size_t i = 0; i < n.neighbours.size(); i++)
    {
        sum += part1(n.neighbours[i],nodeMap,seenSmalls);
    }
    return sum;
}

ull part2(string node, map<string,Node> nodeMap, set<string> seenSmalls, string visitedTwice)
{
    if (node == "end")
    {
        return 1;
    }

    if(seenSmalls.find(node) != seenSmalls.end())
    {
        if(visitedTwice != "" || node == "start")
        {
            return 0;
        }
        else
        {
            visitedTwice = node;
        }
    }

    Node n = nodeMap[node];
    if(n.isSmallNode)
    {
        seenSmalls.insert(node);
    }

    ull sum = 0;
    for (size_t i = 0; i < n.neighbours.size(); i++)
    {
        sum += part2(n.neighbours[i],nodeMap,seenSmalls,visitedTwice);
    }
    return sum;
}

int main()
{
    map<string, Node>nodes = parseInput();
    cout<<part1("start",nodes,{})<<endl;
    cout<<part2("start",nodes,{},"")<<endl;
    return 0;
}