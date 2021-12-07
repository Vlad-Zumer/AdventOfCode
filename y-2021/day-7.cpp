#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "../utils/cpp-utils.hpp"
using namespace std;
using ull = unsigned long long;

vector<long> parseLine(string line)
{
    vector<long> crabs = Utils::vMap<string, long>(Utils::vFilter(Utils::str_split(line, ","), Utils::isNotEmptyStr), [](string s) {return atol(s.c_str());});
    long maxCrabPos = Utils::vReduceL<long, long>(crabs,[](long n, long acc){ return acc>n?acc:n;},0);

    vector<long>aggrCrabs(maxCrabPos+1,0);

    for (size_t i = 0; i < crabs.size(); i++)
    {
        aggrCrabs[crabs[i]]++;
    }
    

    return aggrCrabs;
}

long getCostForPosition_part1(vector<long> aggrCrabs, long pos)
{
    long retVal = 0;
    long dif;
    for (long i = 0; i < aggrCrabs.size(); i++)
    {
        dif = i - pos;
        dif = dif<0?-dif:dif; 
        retVal += dif * aggrCrabs[i];
    }
    return retVal;
}

long getCostForPosition_part2(vector<long> aggrCrabs, long pos)
{
    long retVal = 0;
    long dif;
    for (long i = 0; i < aggrCrabs.size(); i++)
    {
        dif = i - pos;
        dif = dif<0?-dif:dif;
        dif = (dif*(dif+1))/2; 
        retVal += dif * aggrCrabs[i];
    }
    return retVal;
}

long getLowestCost(vector<long> aggrCrabs, long (*getCost)(vector<long>,long))
{
    long minPos = -1, minCost = LONG_MAX, newCost;
    for (long i = 0; i < aggrCrabs.size(); i++)
    {
        newCost = getCost(aggrCrabs,i);
        if(newCost < minCost)
        {
            minPos = i;
            minCost = newCost;
        }
    }
    return minCost;
}

int main()
{
    // ifstream inFile("./inputs/test.input");
    ifstream inFile("./inputs/day-7.input");

    string strLine;
    getline(inFile,strLine);
    auto aggrCrabs = parseLine(strLine);
    cout<<"Part1: "<<getLowestCost(aggrCrabs,getCostForPosition_part1)<<endl;
    cout<<"Part2: "<<getLowestCost(aggrCrabs,getCostForPosition_part2)<<endl;

    return 0;
}