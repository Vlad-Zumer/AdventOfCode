#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include "../utils/cpp-utils.hpp"
using namespace std;
using ull = unsigned long long;

const map<char,string> HexToBin = 
{
    {'0', "0000"},
    {'1', "0001"},
    {'2', "0010"},
    {'3', "0011"},
    {'4', "0100"},
    {'5', "0101"},
    {'6', "0110"},
    {'7', "0111"},
    {'8', "1000"},
    {'9', "1001"},
    {'A', "1010"},
    {'B', "1011"},
    {'C', "1100"},
    {'D', "1101"},
    {'E', "1110"},
    {'F', "1111"},
};

int BinToInt(const string binNum)
{
    int n = 0;
    for (size_t i = 0; i < binNum.size(); i++)    
    {
        n = (n << 1) + (binNum[i] - '0');
    }
    return n;
}

struct Packet
{
    int Version=-1;
    int ID=-1;
    ull LitVal = 0;
    vector<Packet> SubPackets = {};
};

string MakeBinMessage(const string message)
{
    string outVal="";
    for (size_t i = 0; i < message.size(); i++)
    {
        outVal += HexToBin.at(message[i]);
    }
    return outVal;
}

Packet RecParsePacket(const string binMessage, size_t &messageIndex)
{
    string packVer = binMessage.substr(messageIndex,3);
    messageIndex += 3;
    string packId = binMessage.substr(messageIndex,3);
    messageIndex += 3;
    
    Packet pack;
    pack.ID = BinToInt(packId);
    pack.Version = BinToInt(packVer);

    switch (pack.ID)
    {
        case 4:
        {
            ull val = 0;
            while (binMessage[messageIndex] == '1')
            {
                messageIndex++;
                val = (val << 4) + BinToInt(binMessage.substr(messageIndex,4));
                messageIndex += 4;
            }
            
            messageIndex++;
            val = (val << 4) + BinToInt(binMessage.substr(messageIndex,4));
            messageIndex += 4;
            
            pack.LitVal = val;

            break;
        }
        default:
        {
            char lengthTypeId = binMessage[messageIndex];
            messageIndex++;

            if (lengthTypeId == '0')
            {
                // total len encoding in next 15 bits
                int totalLen = BinToInt(binMessage.substr(messageIndex,15));
                messageIndex+=15;
                size_t subIndex = messageIndex;
                while (subIndex < messageIndex + totalLen)
                {
                    Packet child = RecParsePacket(binMessage,subIndex);
                    pack.SubPackets.push_back(child);
                }
                assert(subIndex == messageIndex + totalLen);
                messageIndex += totalLen;
            }
            else
            {
                // num of subpackets encoding in next 11 bits
                int numOfSubPacks = BinToInt(binMessage.substr(messageIndex,11));
                messageIndex+=11;
                for (size_t i = 0; i < numOfSubPacks; i++)
                {
                    Packet child = RecParsePacket(binMessage,messageIndex);
                    pack.SubPackets.push_back(child);
                }
            }

            break;
        }
    }

    return pack;
}

Packet ParseInput()
{
    ifstream inFile("./inputs/day-16.input");
    // ifstream inFile("./inputs/test.input");
    string message;
    getline(inFile,message);
    message = MakeBinMessage(message);
    size_t seek = 0;
    return RecParsePacket(message, seek);
}

ull part1 (const Packet pack)
{
    ull sum = 0;
    vector<Packet> toExplore = { pack };

    for (size_t i = 0; i < toExplore.size(); i++)
    {
        Packet tmp = toExplore[i];
        sum += tmp.Version;
        toExplore.insert(toExplore.end(),tmp.SubPackets.begin(), tmp.SubPackets.end());
    }
    

    return sum;
}

ull EvalPacket(const Packet p)
{
    switch (p.ID)
    {
        case 0:
        {
            ull sum = 0;
            for (size_t i = 0; i < p.SubPackets.size(); i++)
            {
                sum += EvalPacket(p.SubPackets[i]);
            }
            return sum;
            break;
        }
        case 1:
        {

            ull prod = 1;
            for (size_t i = 0; i < p.SubPackets.size(); i++)
            {
                prod *= EvalPacket(p.SubPackets[i]);
            }
            return prod;
            break;
        }
        case 2:
        {
            ull min = ULLONG_MAX;
            for (size_t i = 0; i < p.SubPackets.size(); i++)
            {
                ull tmp = EvalPacket(p.SubPackets[i]);
                if (tmp < min)
                {
                    min = tmp;
                }
            }
            return min;
            break;
        }
        case 3:
        {
            ull max = 0;
            for (size_t i = 0; i < p.SubPackets.size(); i++)
            {
                ull tmp = EvalPacket(p.SubPackets[i]);
                if (tmp > max)
                {
                    max = tmp;
                }
            }
            return max;
            break;
        }
        case 4:
        {
            return p.LitVal; 
            break;
        }
        case 5:
        {
            ull lhs, rhs;
            lhs = EvalPacket(p.SubPackets[0]);
            rhs = EvalPacket(p.SubPackets[1]);
            return lhs > rhs;
            break;
        }
        case 6:
        {
            ull lhs, rhs;
            lhs = EvalPacket(p.SubPackets[0]);
            rhs = EvalPacket(p.SubPackets[1]);
            return lhs < rhs;
            break;
        }
        case 7:
        {
            ull lhs, rhs;
            lhs = EvalPacket(p.SubPackets[0]);
            rhs = EvalPacket(p.SubPackets[1]);
            return lhs == rhs;
            break;
        }
    default:
        assert(false);
        break;
    }
    return 0;
}

ull part2(Packet start)
{
    return EvalPacket(start);
}

int main()
{
    Packet start = ParseInput();
    cout<<"Part 1:"<<part1(start)<<endl;
    cout<<"Part 2:"<<part2(start)<<endl;

    return 0;
}