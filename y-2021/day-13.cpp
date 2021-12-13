#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include "../utils/cpp-utils.hpp"
#include "../utils/cpp-maths-utils.hpp"
using namespace std;
using ull = unsigned long long;

typedef struct
{
    enum Type {X,Y};
    Type type;
    int val = 0;
} Fold;

pair<vector<MathsUtil::Vec2>,vector<Fold>> parseInput()
{
    ifstream inFile("./inputs/day-13.input");
    // ifstream inFile("./inputs/test.input");
    
    vector<MathsUtil::Vec2> points = {};
    vector<Fold> folds = {};
    int parsingType = 0; // 0 - points / 1 - folds
    string strLine;
    while (getline(inFile,strLine))
    {
        if (strLine == "")
        {
            parsingType = 1;
            continue;
        }
        if (parsingType == 0)
        {
            vector<int> vals = Utils::vMap<string,int>(Utils::str_split(strLine,","),[](const string s ){ return atoi(s.c_str());});
            points.push_back({vals[0],vals[1]});
        }

        if (parsingType == 1)
        {
            vector<string> foldStrs = Utils::str_split(strLine,"=");
            Fold f;
            f.val = atoi(foldStrs[1].c_str());
            f.type = foldStrs[0].back() == 'x'? Fold::Type::X : Fold::Type::Y;
            folds.push_back(f);
        }
    }

    return {points, folds};
}

MathsUtil::Vec2 doFold(const MathsUtil::Vec2 vec, const Fold fold)
{
    MathsUtil::Vec2 retVal = vec;
    
    if (fold.type == Fold::Type::X && fold.val < vec.get(0))
    {
        retVal[0] = 2 * fold.val - vec.get(0); 
        retVal[1] = vec.get(1);
    }
    else if (fold.type == Fold::Type::Y && fold.val < vec.get(1))
    {
        retVal[0] = vec.get(0);
        retVal[1] = 2 * fold.val - vec.get(1); 
    }

    if ((fold.type == Fold::Type::X && fold.val == vec.get(0)) || (fold.type == Fold::Type::Y && fold.val == vec.get(1)))
    {
        assert(false);
    }
    
    return retVal;
}

ull part1(const vector<MathsUtil::Vec2> points, const Fold fold)
{
    vector<MathsUtil::Vec2> newPoints;
    for (size_t i = 0; i < points.size(); i++)
    {
        newPoints.push_back(doFold(points.at(i),fold));
        if (newPoints.back()[0] < 0 || newPoints.back()[1] < 0)
        {
            assert(false);
        }
    }

    unordered_set<MathsUtil::Vec2>vecSet(newPoints.begin(),newPoints.end()); 
    return vecSet.size();
}

MathsUtil::Vec2 getElemWiseMax(const MathsUtil::Vec2 a, const MathsUtil::Vec2 b)
{
    return { max(a.get(0),b.get(0)), max(a.get(1),b.get(1)) };
}

void dumpBoard(const vector<MathsUtil::Vec2> points)
{
    MathsUtil::Vec2 MaxXY = Utils::vReduceL<MathsUtil::Vec2, MathsUtil::Vec2>(points, getElemWiseMax, {0,0});
    MaxXY[0]++;
    MaxXY[1]++;

    vector<vector<string>> board = {};
    for (size_t i = 0; i < MaxXY[1]; i++)
    {
        board.push_back({});
        for (size_t j = 0; j < MaxXY[0]; j++)
        {
            board.back().push_back(" ");
        }
    }
    
    for (size_t i = 0; i < points.size(); i++)
    {
        MathsUtil::Vec2 pos = points[i];
        board[pos[1]][pos[0]] = "#";
    }
    
    for (size_t i = 0; i < MaxXY[1]; i++)
    {
        for (size_t j = 0; j < MaxXY[0]; j++)
        {
            cout<<board[i][j];
        }
        cout<<endl;
    }
}

void part2(const vector<MathsUtil::Vec2> points, const vector<Fold> folds)
{
    vector<MathsUtil::Vec2> newPoints = points;
    for (size_t i = 0; i < folds.size(); i++)
    {
        for (size_t j = 0; j < newPoints.size(); j++)
        {
            newPoints[j] = doFold(newPoints[j], folds.at(i));
            if (newPoints[j][0] < 0 || newPoints[j][1] < 0)
            {
                assert(false);
            }
        }
    }
    dumpBoard(newPoints);
}

int main()
{
    auto p = parseInput();
    vector<MathsUtil::Vec2> points = p.first;
    vector<Fold> folds = p.second;
    cout<<"Part 1:"<<part1(points,folds.front())<<endl;
    cout<<"Part 2:"<<endl;
    part2(points,folds);
    return 0;
}