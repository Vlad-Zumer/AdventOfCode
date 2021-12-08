#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include "../utils/cpp-utils.hpp"
using namespace std;
using ull = unsigned long long;

struct Signal
{
    string patterns[10];
    string outVal[4];
};

vector<Signal> ParseInput()
{
    vector<Signal> signals;
    // ifstream inFile("./inputs/test.input");
    ifstream inFile("./inputs/day-8.input");
    string strLine;
    while (getline(inFile,strLine))
    {
        Signal sig;
        vector<string> parts = Utils::str_split(strLine, "|");
        vector<string> part = Utils::vFilter(Utils::str_split(parts[0]," "), Utils::isNotEmptyStr);
        for (size_t i = 0; i < 10; i++)
        {
            sig.patterns[i] = part[i];
        }

        part = Utils::vFilter(Utils::str_split(parts[1], " "), Utils::isNotEmptyStr);
        for (size_t i = 0; i < 4; i++)
        {
            sig.outVal[i] = part[i];
        }

        signals.push_back(sig);        
    }

    return signals;
}

ull part1(const vector<Signal> sigs)
{
    ull sum = 0;
    //count how many times
    //  1 - len=2
    //  4 - len=4
    //  7 - len=3
    //  8 - len=7
    // appear
    for (size_t i = 0; i < sigs.size(); i++)
    {
        for (size_t j = 0; j < 4; j++)
        {
            string outSig = sigs[i].outVal[j];
            if(outSig.size() == 2 || outSig.size() == 4 || outSig.size() == 3 || outSig.size() == 7 )
            {
                sum++;
            }
        }
        
    }
    return sum;
}

vector<char> Intersect(const vector<char> v1, const vector<char> v2)
{
    function<bool(char)> isInV2 = [v2](char ch)
    {
        for (size_t i = 0; i < v2.size(); i++)
        {
            if (v2[i] == ch)
            {
                return true;
            }
        }
        return false;
        
    };
    vector<char> retVal = Utils::vFilter_f(v1,isInV2);
    return retVal;
}

string Convert_str(const string s, const map<char, char> m)
{
    string ns = s;
    for (size_t i = 0; i < s.size(); i++)
    {
        ns[i] = m.at(ns[i]);
    }
    return ns;
}

map<char, char> GetMapping(const map<char,vector<char>> partialMapping, const vector<string> mustMatch)
{
    
    auto isValidConf = [](const string s) -> bool
    {
        vector<string> validConf = {"abcefg","cf","acdeg","acdfg","bcdf","abdfg","abdefg","acf","abcdefg","abcdfg"};
        validConf = Utils::vMap<string,string>(validConf,[](const string conf){string aux = conf; sort(aux.begin(),aux.end()); return aux;});
        if(s.size()>7 || s.size()<2)
        {
            return false;
        }
        string convertedS = s;
        sort(convertedS.begin(),convertedS.end());
        for (string conf : validConf)
        {
            if(conf == convertedS)
            {
                return true;
            }
        }
        return false;
        
    };

    for (size_t a = 0; a < partialMapping.at('a').size(); a++)
    {
        for (size_t b = 0; b < partialMapping.at('b').size(); b++)
        {
            for (size_t c = 0; c < partialMapping.at('c').size(); c++)
            {
                for (size_t d = 0; d < partialMapping.at('d').size(); d++)
                {
                    for (size_t e = 0; e < partialMapping.at('e').size(); e++)
                    {
                        for (size_t f = 0; f < partialMapping.at('f').size(); f++)
                        {
                            for (size_t g = 0; g < partialMapping.at('g').size(); g++)
                            {
                                
                                map<char,char> mapping;
                                mapping['a'] = partialMapping.at('a')[a];
                                mapping['b'] = partialMapping.at('b')[b];
                                mapping['c'] = partialMapping.at('c')[c];
                                mapping['d'] = partialMapping.at('d')[d];
                                mapping['e'] = partialMapping.at('e')[e];
                                mapping['f'] = partialMapping.at('f')[f];
                                mapping['g'] = partialMapping.at('g')[g];

                                bool noDuplicates = true;

                                for (auto kvp1 : mapping)
                                {
                                    for (auto kvp2 : mapping)
                                    {
                                        if(kvp1.second == kvp2.second && kvp1.first != kvp2.first)
                                        {
                                            noDuplicates = false;
                                            break;
                                        }
                                        if(!noDuplicates)
                                        {
                                            break;
                                        }
                                    }
                                }
                                

                                if (noDuplicates)
                                {
                                    bool foundMatches = true;

                                    for (size_t i = 0; i < mustMatch.size(); i++)
                                    {
                                        string converted = Convert_str(mustMatch[i],mapping);
                                        if (!isValidConf(converted))
                                        {
                                            foundMatches = false;
                                            break;
                                        }
                                    }
                                    if (foundMatches)
                                    {
                                        return mapping;
                                    }
                                }
                                
                            }   
                        }   
                    }   
                }   
            }   
        }   
    }
    
    return {};
}

int Decode(const string s, const map<char, char> m)
{
    vector<string> validConf = {"abcefg","cf","acdeg","acdfg","bcdf","abdfg","abdefg","acf","abcdefg","abcdfg"};
    validConf = Utils::vMap<string,string>(validConf,[](const string conf){string aux = conf; sort(aux.begin(),aux.end()); return aux;});
    string convertedS = Convert_str(s,m);
    sort(convertedS.begin(),convertedS.end());

    for (size_t i = 0; i < validConf.size(); i++)
    {
        if(validConf[i] == convertedS)
        {
            return i;
        }
    }
    cout<<s<<" - "<<convertedS<<endl;
    for (auto kvp : m)
    {
        cout<<"\t"<<kvp.first<<": "<<kvp.second<<endl;
    }
    
    assert(false);
    return -1;
}

ull part2(vector<Signal> sigs)
{
    ull sum = 0;
    for (size_t n = 0; n < sigs.size(); n++)
    {
        Signal sig = sigs[n];
        map<char,vector<char>> mapping = {};
        vector<string> allSignals = {};
        mapping['a'] = {'a','b','c','d','e','f','g'};
        mapping['b'] = {'a','b','c','d','e','f','g'};
        mapping['c'] = {'a','b','c','d','e','f','g'};
        mapping['d'] = {'a','b','c','d','e','f','g'};
        mapping['e'] = {'a','b','c','d','e','f','g'};
        mapping['f'] = {'a','b','c','d','e','f','g'};
        mapping['g'] = {'a','b','c','d','e','f','g'};

        for (size_t i = 0; i < 10; i++)
        {
            vector<char> possibilities = {'a','b','c','d','e','f','g'};
            string pattern = sig.patterns[i];
            if(pattern.size() == 2) // 1
            {
                possibilities = {'c','f'};
            }
            if(pattern.size() == 3) // 7
            {
                possibilities = {'a','c','f'};
            }
            if(pattern.size() == 4) // 4
            {
                possibilities = {'b','c','d','f'};
            }

            for (size_t j = 0; j < pattern.size(); j++)
            {
                mapping[pattern[j]] = Intersect(possibilities, mapping[pattern[j]]);
            }
            allSignals.push_back(pattern);
        }

        for (size_t i = 0; i < 4; i++)
        {
            vector<char> possibilities = {'a','b','c','d','e','f','g'};
            string pattern = sig.outVal[i];
            if(pattern.size() == 2) // 1
            {
                possibilities = {'c','f'};
            }
            if(pattern.size() == 3) // 7
            {
                possibilities = {'a','c','f'};
            }
            if(pattern.size() == 4) // 4
            {
                possibilities = {'b','c','d','f'};
            }

            for (size_t j = 0; j < pattern.size(); j++)
            {
                mapping[pattern[j]] = Intersect(possibilities, mapping[pattern[j]]);
            }
            allSignals.push_back(pattern);
        }
        
        map<char,char> fullMapping = GetMapping(mapping,allSignals);

        ull num = 0;
        for (size_t i = 0; i < 4; i++)
        {
            num = num * 10 + Decode(sig.outVal[i],fullMapping);
        }

        sum += num;
    }
    return sum;
}

int main()
{
    vector<Signal> signals = ParseInput();
    cout<<"Part1: "<<part1(signals)<<endl;
    cout<<"Part2: "<<part2(signals)<<endl;
    return 0;
}