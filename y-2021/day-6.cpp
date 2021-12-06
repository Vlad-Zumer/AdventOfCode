#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "../utils/cpp-utils.hpp"
using namespace std;
using ull = unsigned long long;

void parseLine(string line, ull (&arr)[9])
{
    for (size_t i = 0; i < 9; i++)
    {
        arr[i] = 0;
    }
    
    vector<int> fishes = Utils::vMap<string,int>(Utils::vFilter(Utils::str_split(line,","),Utils::isNotEmptyStr), [](string s){return atoi(s.c_str());});
    
    for (size_t i = 0; i < fishes.size(); i++)
    {
        arr[fishes[i]]++;
    }
}

void passDay(ull (&arr)[9])
{
    ull at0 = arr[0];

    for (size_t i = 1; i < 9; i++)
    {
        arr[i-1] = arr[i];
    }
    arr[8] = 0;
    arr[8] += at0;
    arr[6] += at0;
}


int main()
{
    // ifstream inFile("./inputs/test.input");
    ifstream inFile("./inputs/day-6.input");

    string strLine;
    getline(inFile,strLine);
    ull fishCooldown[9];
    parseLine(strLine, fishCooldown);

    for (size_t i = 0; i < 80; i++)
    {
        passDay(fishCooldown);
    }

    ull sum = 0;
    for (size_t i = 0; i < 9; i++)
    {
        sum += fishCooldown[i];
    }
    cout<<"Sol 1:"<<sum<<endl;

    for (size_t i = 0; i < 256-80; i++)
    {
        passDay(fishCooldown);
    }

    sum = 0;
    for (size_t i = 0; i < 9; i++)
    {
        sum += fishCooldown[i];
    }
    cout<<"Sol 2:"<<sum<<endl;

    return 0;
}