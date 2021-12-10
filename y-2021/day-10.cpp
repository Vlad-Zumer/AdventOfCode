#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include "../utils/cpp-utils.hpp"
using namespace std;
using ull = unsigned long long;

vector<string> parseInput()
{
    vector<string> retVal = {};
    ifstream inFile("./inputs/day-10.input");
    // ifstream inFile("./inputs/test.input");
    string strLine;
    while (getline(inFile,strLine))
    {
        retVal.push_back(Utils::str_trim(strLine));
    }
    return retVal;
}

struct SyntaxError
{
    enum Type {CORRUPTED, INCOMPLETE, NONE};
    size_t position;
    char character;
    Type type;
    vector<char> left = {};
};

SyntaxError getLineError(const string line)
{
    vector<char>expected = {};
    for (size_t i = 0; i < line.size(); i++)
    {
        switch (line[i])
        {
        case '(':
        {
            expected.push_back(')');
            break;
        }
        
        case '[':
        {
            expected.push_back(']');
            break;
        }
        
        case '{':
        {
            expected.push_back('}');
            break;
        }
        
        case '<':
        {
            expected.push_back('>');
            break;
        }
        
        case ')':
        {
            char expectedCh = expected.back();
            expected.pop_back();
            if (expectedCh != ')')
            {
                SyntaxError s;
                s.type = SyntaxError::Type::CORRUPTED;
                s.character = ')';
                s.position = i;
                return s;
            }
            break;
        }
        
        case ']':
        {
            char expectedCh = expected.back();
            expected.pop_back();
            if (expectedCh != ']')
            {
                SyntaxError s;
                s.type = SyntaxError::Type::CORRUPTED;
                s.character = ']';
                s.position = i;
                return s;
            }
            break;
        }
        
        case '}':
        {
            char expectedCh = expected.back();
            expected.pop_back();
            if (expectedCh != '}')
            {
                SyntaxError s;
                s.type = SyntaxError::Type::CORRUPTED;
                s.character = '}';
                s.position = i;
                return s;
            }
            break;
        }
        
        case '>':
        {
            char expectedCh = expected.back();
            expected.pop_back();
            if (expectedCh != '>')
            {
                SyntaxError s;
                s.type = SyntaxError::Type::CORRUPTED;
                s.character = '>';
                s.position = i;
                return s;
            }
            break;
        }
        
        default:
            assert(false);
            break;
        }   
    }

    SyntaxError s;
    if (expected.size() == 0)
    {
        s.type = SyntaxError::Type::NONE;
    }
    else
    {
        s.type = SyntaxError::Type::INCOMPLETE;
        s.left = expected;
    }
    return s;
}


int getCorruptedScore(const string line)
{
    SyntaxError err = getLineError(line);
    if (err.type == SyntaxError::Type::CORRUPTED)
    {
        switch (err.character)
        {
        case ')':
            return 3;
            break;
        case ']':
            return 57;
            break;
        case '}':
            return 1197;
            break;
        case '>':
            return 25137;
            break;
        
        default:
            assert(false);
            break;
        }
    }
    return 0;   
}

ull part1(const vector<string> lines)
{
    ull sum = 0;
    for (size_t i = 0; i < lines.size(); i++)
    {
        sum += getCorruptedScore(lines[i]);
    }
    
    return sum;
}

ull getIncompleteScore(const string line)
{
    ull sum = 0;
    SyntaxError err = getLineError(line);
    if (err.type == SyntaxError::Type::INCOMPLETE)
    {   
        for (long i = err.left.size() -1; i >= 0; i--)
        {
            sum *=5;
            switch (err.left[i])
            {
            case ')':
                sum+=1;
                break;
            case ']':
                sum+=2;
                break;
            case '}':
                sum+=3;
                break;
            case '>':
                sum+=4;
                break;
            
            default:
                assert(false);
                break;
            }
        }
    }
    return sum;   
}

ull part2(const vector<string> lines)
{   
    vector<ull> incompleteScores = Utils::vFilter<ull>(Utils::vMap<string,ull>(lines,getIncompleteScore),[](ull num){ return num>0;});
    sort(incompleteScores.begin(),incompleteScores.end());
    return incompleteScores[incompleteScores.size()/2];
}

int main()
{
    vector<string>lines = parseInput();
    cout<<part1(lines)<<endl;
    cout<<part2(lines)<<endl;
    return 0;
}