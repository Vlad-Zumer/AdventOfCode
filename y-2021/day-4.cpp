#include<iostream>
#include<fstream>
#include<sstream>
#include<string>

#include "../utils/cpp-utils.hpp" 
using namespace std;

typedef struct Cell
{
    long val = -1;
    bool seen = false;
} Cell;

ostream& operator<<(ostream& os, const Cell cell)
{
    string seenBool = cell.seen ? "true" : "false";
    return os << "(" << cell.val<< ", " << seenBool << ")"; 
}

typedef struct WinStatus
{
    typedef enum {NONE, LINE, COL} Type;
    Type type = Type::NONE;
    long value = -1;
} WinStatus;

// assume square board
typedef struct Board
{
    vector<vector<Cell>> cells;
    vector<Cell> getBoardLine(const long index) const { return cells[index];}
    vector<Cell> getBoardColumn(const long index) const 
    {
        return Utils::vMap_f<vector<Cell>, Cell>(cells,[index](vector<Cell>cells){return cells[index];});
    }

    WinStatus checkWin() const
    {
        long boardSize = cells.size();
        bool won;
        vector<Cell> cellsToCheck;
        for (long i = 0; i < boardSize; i++)
        {
            won = true;
            cellsToCheck = getBoardLine(i);
            for (size_t j = 0; j < cellsToCheck.size(); j++)
            {
                won &= cellsToCheck[j].seen;
            }

            if(won)
            {
                return {WinStatus::Type::LINE,i} ;
            }

            won = true;
            cellsToCheck = getBoardColumn(i);
            // cout<<"COL "<<i<<" : ";
            for (size_t j = 0; j < cellsToCheck.size(); j++)
            {
                // cout<<cellsToCheck[j]<<", ";
                won &=  cellsToCheck[j].seen;
            }
            // cout<<endl;

            if(won)
            {
                return {WinStatus::Type::COL,i};
            }
        }

        return {};
        
    }

    bool didWin() const
    {
        return (checkWin().type != WinStatus::Type::NONE);
    }

    void markNumber(long number)
    {
        for (size_t i = 0; i < cells.size(); i++)
        {
            for (size_t j = 0; j < cells[i].size(); j++)
            {
                if (cells[i][j].val == number)
                {
                    cells[i][j].seen = true;
                }
            }
        }
    }

    void reset()
    {
        for (size_t i = 0; i < cells.size(); i++)
        {
            for (size_t j = 0; j < cells[i].size(); j++)
            {
                cells[i][j].seen = false;
            }
        }
    }

    vector<Cell> getNotMarked()
    {
        vector<Cell> ret = {};
        for (size_t i = 0; i < cells.size(); i++)
        {
            for (size_t j = 0; j < cells[i].size(); j++)
            {
                if (!cells[i][j].seen)
                {
                    ret.push_back(cells[i][j]);
                }
            }
        }

        return ret;
    }

    void dump() const
    {
        for (size_t i = 0; i < cells.size(); i++)
        {
            for (size_t j = 0; j < cells[i].size(); j++)
            {
                cout<<cells[i][j]<<", ";
            }
            cout<<endl;
        }
    }

} Board;


Board parseBoard(vector<string> lines)
{
    vector<vector<Cell>> parsedLines =
        Utils::vMap<string,vector<Cell>>(
            lines,
            [](string line){ 
                return Utils::vMap<string,Cell>(
                    Utils::vFilter(Utils::str_split(line, " "), Utils::isNotEmptyStr),
                    [](string num) {
                        Cell a = {};
                        a.val = atol(num.c_str());
                        return a;
                    }
                );
            }
        );

    Board b = {};
    b.cells = parsedLines;

    return b;
}


int main()
{
    ifstream inFile("./inputs/day-4.input");
    string numCalledStr, boardLine;
    getline(inFile,numCalledStr);
    
    vector<long> numCalled = Utils::vMap<string, long>(Utils::vFilter(Utils::str_split(numCalledStr,","), Utils::isNotEmptyStr), [](string s){ return atol(s.c_str());});

    getline(inFile,boardLine); // read empty line on file

    vector<vector<string>> boardsStrs = {{}};
    size_t lastBoardId = 0;
    while (getline(inFile,boardLine))
    {
        // cout<<"\""<<boardLine<< "\" - b: "<<lastBoardId<<endl;
        if(boardLine == "")
        {
            lastBoardId++;
            boardsStrs.push_back({});
            continue;
        }

        boardsStrs[lastBoardId].push_back(boardLine);
    }

    vector<Board> boards;
    for (size_t i = 0; i < boardsStrs.size(); i++)
    {
        boards.push_back(parseBoard(boardsStrs[i]));
    }

    long calledNum = -1;
    Board winningBoard = {};
    for (const long num : numCalled)
    {
        calledNum = num;
        for (Board& b : boards)
        {
            b.markNumber(calledNum);
            if(b.didWin())
            {
                winningBoard = b;
                break;
            }
        }

        if(winningBoard.cells.size() > 0)
        {
            break;
        }
    }
  
    cout<<"sol1: "<<calledNum * Utils::vReduceL<Cell,long>(winningBoard.getNotMarked(),[](Cell c, long acc){ return acc+c.val;}, 0)<<endl;

    calledNum = -1;
    winningBoard = {};
    vector<Board> tmp = Utils::vMap<Board,Board>(boards,[](Board a){a.reset(); return a;});
    for (const long num : numCalled)
    {
        for (size_t i = 0; i < tmp.size(); i++)
        {
            Board& b = tmp[i];
            b.markNumber(num);
            if(b.didWin())
            {
                winningBoard = b;
                calledNum = num;
                tmp.erase(tmp.begin() + i);
                i--;
            }
        }
        if(tmp.size() == 0)
        {
            break;
        }
    }

    cout<<"sol2: "<<calledNum * Utils::vReduceL<Cell,long>(winningBoard.getNotMarked(),[](Cell c, long acc){ return acc+c.val;}, 0)<<endl;

    return 0;
}