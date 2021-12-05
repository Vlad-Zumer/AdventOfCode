#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <utility>
#include "../utils/cpp-utils.hpp"
#include "../utils/cpp-maths-utils.hpp"

using namespace std;

using LineSeg = pair<MathsUtil::Vec2, MathsUtil::Vec2>;
using Board = pair<vector<vector<int>>, pair<int,int>>;

LineSeg parseLine(const string line)
{
    LineSeg seg = {MathsUtil::Vec2{},MathsUtil::Vec2{}};

    vector<string> pointStr = Utils::str_split(line, "->");

    vector<int> coords = Utils::vMap<string,int>(Utils::str_split(pointStr[0],","), [](string s){ return atoi(s.c_str()); });
    seg.first[0] = coords[0];
    seg.first[1] = coords[1];

    coords = Utils::vMap<string,int>(Utils::str_split(pointStr[1],","), [](string s){ return atoi(s.c_str()); });
    seg.second[0] = coords[0];
    seg.second[1] = coords[1];

    return seg;
}

Board makeBoard(int MaxX, int MaxY)
{
    vector<vector<int>>ret(MaxY);
    for (size_t i = 0; i < MaxY; i++)
    {
        ret[i] = vector<int>(MaxX,0);
    }
    return {ret,{MaxX,MaxY}};
}

int gcd(int a, int b) {
   
    if (b == 0)
    {
        return a;
    }

    return gcd(b, a % b);
}

MathsUtil::Vec2 getDir(const LineSeg seg)
{
    MathsUtil::Vec2 dir = seg.second - seg.first;
    int a = dir[0]<0?-dir[0] : dir[0];
    int b = dir[1]<0?-dir[1] : dir[1];
    int dirGcd = 0;
    if (a == 0)
    {
        dirGcd = b;
    }
    else if(b == 0)
    {
        dirGcd = a;
    }
    else
    {
        dirGcd = gcd(a,b);
    }
    // no direction to move in
    assert(dirGcd != 0);
    return dir/dirGcd;
}

Board addLineSeg(const Board old, LineSeg seg)
{
    int MaxX = old.second.first, MaxY = old.second.second;
    Board newBoard = old;

    MathsUtil::Vec2 dir = getDir(seg);
    
    // if dir = 0,0 => no direction
    assert(dir[0] != 0 || dir[1] != 0);

    MathsUtil::Vec2 point = seg.first;
    while ( point[0] >= 0 && point[0] < MaxX &&
            point[1] >= 0 && point[1] < MaxY &&
            point != (seg.second + dir))
    {
        newBoard.first[point[1]][point[0]]++;
        point = point + dir;
    }

    return newBoard;
}

bool isOrtSeg(const LineSeg seg)
{
    MathsUtil::Vec2 dir = getDir(seg);
    return (dir[0] == 0 || dir[1] == 0);
}

void dumpBoard(Board b)
{
    int MaxX = b.second.first, MaxY = b.second.second;

    for (size_t i = 0; i < MaxY; i++)
    {
        for (size_t j = 0; j < MaxX; j++)
        {
            cout<<b.first[i][j]<<"\t";
        }
        cout<<endl;
    }
}

unsigned long long solve(Board b)
{
    int MaxX = b.second.first, MaxY = b.second.second;
    unsigned long long count;
    for (size_t i = 0; i < MaxY; i++)
    {
        for (size_t j = 0; j < MaxX; j++)
        {
            if (b.first[i][j] > 1)
            {
                count++;
            }
        }
    }
    return count;
}

int main()
{
    // ifstream inFile("./inputs/test.input");
    ifstream inFile("./inputs/day-5.input");

    string strLine;
    vector<LineSeg> lines;
    while (getline(inFile,strLine))
    {
        lines.push_back(parseLine(strLine));
    }

    int MaxX = 0, MaxY = 0;

    for (auto const lineSeg : lines)
    {
        if(lineSeg.first.get(0) > MaxX)
        {
            MaxX = lineSeg.first.get(0);
        }

        if(lineSeg.second.get(0) > MaxX)
        {
            MaxX = lineSeg.second.get(0);
        }

        if(lineSeg.first.get(1) > MaxY)
        {
            MaxY = lineSeg.first.get(1);
        }

        if(lineSeg.second.get(1) > MaxY)
        {
            MaxY = lineSeg.second.get(1);
        }
    }
    
    Board initBoard = makeBoard(MaxX+1, MaxY+1);

    vector<LineSeg> ortSegs = Utils::vFilter<LineSeg>(lines,isOrtSeg);

    Board tmp = initBoard;
    for (size_t i = 0; i < ortSegs.size(); i++)
    {
        tmp = addLineSeg(tmp,ortSegs[i]);
    }
    cout<<"Sol1: "<<solve(tmp)<<endl;

    tmp = initBoard;
    for (size_t i = 0; i < lines.size(); i++)
    {
        tmp = addLineSeg(tmp,lines[i]);
    }
    cout<<"Sol2: "<<solve(tmp)<<endl;

    return 0;
}