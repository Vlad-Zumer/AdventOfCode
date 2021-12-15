#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "../utils/cpp-utils.hpp"
using namespace std;
using ull = unsigned long long;

pair<string, map<string,string>> parseInput()
{
    ifstream inFile("./inputs/day-14.input");
    // ifstream inFile("./inputs/test.input");
    string currentCompound;
    map<string,string> rules;
    string strLine;
    getline(inFile,currentCompound);
    getline(inFile,strLine);

    while (getline(inFile,strLine))
    {
        vector<string> ruleStrs = Utils::str_split(strLine,"->");
        rules[Utils::str_trim(ruleStrs[0])] = Utils::str_trim(ruleStrs[1]);
    }

    return {currentCompound, rules};
}

string doReplace(const string original, const map<string,string> rules)
{
    string next = ""; 
    next.push_back(original[0]);
    for (size_t i = 0; i < original.size()-1; i++)
    {
        string toCheck = original.substr(i,2);
        if (rules.find(toCheck) != rules.end())
        {
            next += rules.at(toCheck);
        }
        next.push_back(original[i+1]);
    }
    return next;
}

map<char,ull> makeHistogram(const string str)
{
    map<char,ull> hist;
    for (size_t i = 0; i < str.size(); i++)
    {
        if (hist.find(str[i]) == hist.end())
        {
            hist[str[i]] = 0;
        }
        hist[str[i]]++;
    }
    return hist;
}

ull part1(const string str, const map<string,string> rules)
{
    string aux = str;
    for (size_t i = 0; i < 10; i++)
    {
        aux = doReplace(aux,rules);
    }

    ull min = -1, max=0;
    map<char,ull> hist = makeHistogram(aux);
    for (auto i : hist)
    {
        if(min > i.second)
        {
            min = i.second;
        }

        if(max < i.second)
        {
            max = i.second;
        }
    }
    return (max-min);
}

/******************************************************************
                          -PART2-
Idea stolen from Reddit
    -only store pairs and their number of appearances
    -each letter (except for first and last appear exactly 2x (one in a "left" pair and one in a "right" pair))
*******************************************************************/

map<char,ull> makeHistogram2(const map<string,ull> pairs, const char fst, const char lst)
{
    map<char,ull> hist;
    hist[fst]++;
    hist[lst]++;

    for (const auto p : pairs)
    {
        for (const auto ch : p.first)
        {
            hist[ch] += p.second;
        }
    }

    for (const auto h : hist)
    {
        hist[h.first] = h.second/2;
    }
    
    return hist;
}

map<string,ull> doReplace2(const map<string,ull> pairs, const map<string,string> rules)
{
    map<string,ull> newPairs;

    for (const auto kvp : pairs)
    {
        if (rules.find(kvp.first) != rules.end())
        {
            string fst="", snd="";
            fst.push_back(kvp.first.front());
            fst += rules.at(kvp.first);

            snd += rules.at(kvp.first);
            snd.push_back(kvp.first.back());

            newPairs[fst] += kvp.second;
            newPairs[snd] += kvp.second;
        }
        else
        {
            newPairs[kvp.first] = kvp.second;
        }
    }
    
    return newPairs;
}

ull part2(const string str, const map<string,string> rules)
{
    char fst = str.front(), lst = str.back();
    map<string,ull> pairs;
    for (size_t i = 0; i < str.size()-1; i++)
    {
        string pair = str.substr(i,2);
        pairs[pair]++;
    }
    
    for (size_t i = 0; i < 40; i++)
    {
        pairs = doReplace2(pairs,rules);
    }
    
    map<char,ull> hist = makeHistogram2(pairs,fst,lst);
    ull min = -1, max=0;
    for (auto i : hist)
    {
        if(min > i.second)
        {
            min = i.second;
        }

        if(max < i.second)
        {
            max = i.second;
        }
    }
    return (max-min);
}


int main()
{
    auto input = parseInput();
    string comp = input.first;
    map<string,string> rules = input.second;

    cout<<"Part 1:"<<part1(comp,rules)<<endl;
    cout<<"Part 2:"<<part2(comp,rules)<<endl;
    return 0;
}