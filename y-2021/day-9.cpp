#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include "../utils/cpp-utils.hpp"
using namespace std;
using ull = unsigned long long;

struct LowPoint
{
    int val;
    long x,y;
};

vector<vector<int>> ParseInput()
{
    vector<vector<int>> retVal = {};
    ifstream inFile("./inputs/day-9.input");
    // ifstream inFile("./inputs/test.input");
    string strLine;
    while (getline(inFile,strLine))
    {
        vector<int>line = {};

        for (size_t i = 0; i < strLine.size(); i++)
        {
            line.push_back(strLine[i] - '0');
        }

        retVal.push_back(line);
    }
    return retVal;
}

vector<LowPoint> GetLowPoints(const vector<vector<int>> heightMap)
{
    vector<LowPoint> retVal = {};

    for (size_t i = 0; i < heightMap.size(); i++)
    {
        for (size_t j = 0; j < heightMap[i].size(); j++)
        {
            bool lowPoint = true;
            if (i > 0 && heightMap[i][j] >= heightMap[i-1][j])
            {
                lowPoint  = false;
            }

            if (i < heightMap.size()-1 && heightMap[i][j] >= heightMap[i+1][j])
            {
                lowPoint  = false;
            }


            if (j > 0 && heightMap[i][j] >= heightMap[i][j-1])
            {
                lowPoint  = false;
            }

            if (j < heightMap[i].size()-1 && heightMap[i][j] >= heightMap[i][j+1])
            {
                lowPoint  = false;
            }

            if (lowPoint)
            {
                LowPoint l;
                l.val = heightMap[i][j];
                l.y = i;
                l.x = j;
                retVal.push_back(l);
            }
        }
    }
    

    return retVal;
}

ull part1(const vector<vector<int>> heightMap)
{
    ull retVal = 0;
    vector<LowPoint> lows = GetLowPoints(heightMap);

    for (size_t i = 0; i < lows.size(); i++)
    {
        retVal +=lows[i].val+1;
    }
    
    return retVal;
}

ull part2(const vector<vector<int>> heightMap)
{
    vector<LowPoint> lows = GetLowPoints(heightMap);
    ull basins[3] = {0,0,0};

    auto notExplored = [](vector<LowPoint> explored, LowPoint point) -> bool
    {
        for (size_t i = 0; i < explored.size(); i++)
        {
            if(explored[i].x == point.x && explored[i].y == point.y)
            {
                return false;
            }
        }

        return true;
    };

    for (size_t i = 0; i < lows.size(); i++)
    {
        ull basinSize = 0;
        vector<LowPoint>toExplore = {};
        vector<LowPoint>explored = {};
        toExplore.push_back(lows[i]);
        while (toExplore.size() > 0)
        {
            basinSize++;
            LowPoint node = *toExplore.begin();
            toExplore.erase(toExplore.begin());
            explored.push_back(node);
            LowPoint nextNode = {};
            if (node.x > 0 && node.val < heightMap[node.y][node.x-1] && heightMap[node.y][node.x-1] != 9)
            {
                nextNode.x = node.x -1;
                nextNode.y = node.y;
                nextNode.val = heightMap[node.y][node.x-1];
                
                if(notExplored(explored, nextNode) && notExplored(toExplore, nextNode))
                {
                    toExplore.push_back(nextNode);
                }
            }

            if (node.x < heightMap[node.y].size()-1 && node.val < heightMap[node.y][node.x+1] && heightMap[node.y][node.x+1] != 9)
            {
                nextNode.x = node.x+1;
                nextNode.y = node.y;
                nextNode.val = heightMap[node.y][node.x+1];
                
                if(notExplored(explored, nextNode) && notExplored(toExplore, nextNode))
                {
                    toExplore.push_back(nextNode);
                }
            }

            if (node.y > 0 && node.val < heightMap[node.y-1][node.x] && heightMap[node.y-1][node.x] != 9)
            {
                nextNode.x = node.x;
                nextNode.y = node.y-1;
                nextNode.val = heightMap[node.y-1][node.x];
                
                if(notExplored(explored, nextNode) && notExplored(toExplore, nextNode))
                {
                    toExplore.push_back(nextNode);
                }
            }

            if (node.y < heightMap.size()-1 && node.val < heightMap[node.y+1][node.x] && heightMap[node.y+1][node.x] != 9)
            {
                nextNode.x = node.x;
                nextNode.y = node.y+1;
                nextNode.val = heightMap[node.y+1][node.x];
                
                if(notExplored(explored, nextNode) && notExplored(toExplore, nextNode))
                {
                    toExplore.push_back(nextNode);
                }
            }
        }

        basins[2] = max(basins[2],basinSize);
        if(basins[2] > basins[1])
        {
            swap(basins[2],basins[1]);
        }
        if(basins[1] > basins[0])
        {
            swap(basins[1],basins[0]);
        }
    }
    
    return basins[0]*basins[1]*basins[2];
}

int main()
{
    vector<vector<int>> heightMap = ParseInput();
    cout<<"Part1: "<<part1(heightMap)<<endl;
    cout<<"Part2: "<<part2(heightMap)<<endl;
    return 0;
}