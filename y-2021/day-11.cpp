#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_set>
#include "../utils/cpp-utils.hpp"
#include "../utils/cpp-maths-utils.hpp"
using namespace std;
using ull = unsigned long long;

typedef struct
{
    unsigned int blinks = 0;
    vector<vector<int>> newBoard = {};
} StepResult;

void dumpBoard(const vector<vector<int>> board)
{
    for (size_t i = 0; i < board.size(); i++)
    {
        for (size_t j = 0; j < board[i].size(); j++)
        {
            cout<<board[i][j];
        }
        cout<<endl;
    }
}

vector<vector<int>> parseInput()
{
    vector<vector<int>> retVal = {};
    ifstream inFile("./inputs/day-11.input");
    // ifstream inFile("./inputs/test.input");
    string strLine;
    while (getline(inFile,strLine))
    {
        retVal.push_back({});
        for (size_t i = 0; i < strLine.size(); i++)
        {
            retVal.back().push_back(strLine[i]-'0');
        }
    }
    return retVal;
}

bool isInBounds(const MathsUtil::Vec2 pos, const MathsUtil::Vec2 bounds)
{
    return  pos.get(0) >= 0 &&
            pos.get(0) < bounds.get(0) &&
            pos.get(1) >= 0 &&
            pos.get(1) < bounds.get(1);
}

StepResult processBoard(const vector<vector<int>> board)
{
    StepResult result = {};
    result.newBoard = board;
    MathsUtil::Vec2 bounds;
    bounds[0] = board.size();
    bounds[1] = board[0].size();

    vector<MathsUtil::Vec2> neighbours = {  {-1,-1}, {-1, 0}, {-1, 1},
                                            { 0,-1},          { 0, 1},
                                            { 1,-1}, { 1, 0}, { 1, 1} };

    for (size_t i = 0; i < result.newBoard.size(); i++)
    {
        for (size_t j = 0; j < result.newBoard[i].size(); j++)
        {
            result.newBoard[i][j]++;
        }
    }

    unordered_set<MathsUtil::Vec2> flashed;
    bool didFlash = false;
    do
    {
        didFlash = false;
        for (size_t i = 0; i < result.newBoard.size(); i++)
        {
            for (size_t j = 0; j < result.newBoard[i].size(); j++)
            {
                MathsUtil::Vec2 pos = {(int)i, (int)j};
                if (result.newBoard[i][j] > 9 && flashed.find(pos) == flashed.end())
                {
                    flashed.insert(pos);
                    didFlash = true;
                    for (size_t n = 0; n < neighbours.size(); n++)
                    {
                        MathsUtil::Vec2 nPos = pos + neighbours[n];
                        if (isInBounds(nPos,bounds))
                        {
                            result.newBoard[nPos[0]][nPos[1]]++;
                        }
                    }
                }
            }
        }
    } while (didFlash);
    


    for (size_t i = 0; i < result.newBoard.size(); i++)
    {
        for (size_t j = 0; j < result.newBoard[i].size(); j++)
        {
            if (result.newBoard[i][j]>9)
            {
                result.blinks++;
                result.newBoard[i][j] = 0;
            }
        }
    }

    return result;
}

ull part1(const vector<vector<int>> _board)
{
    ull blinks = 0;
    vector<vector<int>>board = _board;

    for (size_t i = 0; i < 100 ; i++)
    {
        auto res = processBoard(board);
        blinks+=res.blinks;
        board = res.newBoard;
    }

    return blinks;
}

bool flashedAll(const vector<vector<int>> board)
{
    return
    Utils::vReduceL<bool,bool>
    (
        Utils::vMap<vector<int>,bool>(
            board, 
            [](vector<int> v)
            { 
                return
                Utils::vReduceL<bool,bool>( 
                    Utils::vMap<int, bool>(
                        v,
                        [](int i)
                        { 
                            return i == 0;
                        }
                    ),
                    [](bool in, bool acc){ return in && acc;},
                    true
                );
            }),
        [](bool in, bool acc){ return in && acc;},
        true
    );
}

ull part2(const vector<vector<int>> _board)
{
    ull step;
    vector<vector<int>>board = _board;

    for (step = 0; !flashedAll(board); step++)
    {
        auto res = processBoard(board);
        board = res.newBoard;
    }
    
    return step;
}

int main()
{
    vector<vector<int>>lines = parseInput();
    cout<<part1(lines)<<endl;
    cout<<part2(lines)<<endl;
    cout<<"Hello World"<<endl;
    return 0;
}