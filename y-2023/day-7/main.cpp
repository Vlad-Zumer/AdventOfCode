#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <array>
#include <map>
#include <algorithm>
#include "../../utils/cpp-utils.hpp"

using ull = unsigned long long;
using ll = long long;
using ld = long double;

const std::map<char, int> s_cardToNumberMap = {
    {'*', 0},
    {'2', 2},
    {'3', 3},
    {'4', 4},
    {'5', 5},
    {'6', 6},
    {'7', 7},
    {'8', 8},
    {'9', 9},
    {'T', 10},
    {'J', 11},
    {'Q', 12},
    {'K', 13},
    {'A', 14},
};

struct Hand
{
    enum Type
    {
        HighCard = 0,
        OnePair = 5,
        TwoPair = 15,
        Three = 20,
        FullHouse = 25,
        Four = 30,
        Five = 35
    } type;

    std::array<int, 5> cards;

    friend bool operator==(const Hand &a, const Hand &b);
    friend bool operator!=(const Hand &a, const Hand &b);
    friend bool operator<(const Hand &a, const Hand &b);
    friend bool operator<=(const Hand &a, const Hand &b);
    friend bool operator>(const Hand &a, const Hand &b);
    friend bool operator>=(const Hand &a, const Hand &b);
};

bool operator==(const Hand &a, const Hand &b)
{
    return (a.type == b.type) && Utils::arrEq(a.cards, b.cards);
}

bool operator<(const Hand &a, const Hand &b)
{
    if (a.type < b.type)
    {
        return true;
    }

    if (a.type > b.type)
    {
        return false;
    }

    for (size_t i = 0; i < 5; i++)
    {
        if (a.cards[i] < b.cards[i])
        {
            return true;
        }

        if (b.cards[i] < a.cards[i])
        {
            return false;
        }
    }

    return false;
}

IMPL_COMP_OPS(Hand);

struct Play
{
    Hand hand;
    int bid;
};

struct ParsedInput
{
    std::string handStr;
    int bid;
};

std::string typeToString(const Hand::Type type)
{
    switch (type)
    {
    case Hand::Type::HighCard:
        return "HighCard";
    case Hand::Type::OnePair:
        return "OnePair";
    case Hand::Type::TwoPair:
        return "TwoPair";
    case Hand::Type::Three:
        return "Three";
    case Hand::Type::FullHouse:
        return "FullHouse";
    case Hand::Type::Four:
        return "Four";
    case Hand::Type::Five:
        return "Five";
    default:
        break;
    }

    return "ERROR";
}

std::vector<ParsedInput> parseFile(const std::string &filePath)
{
    std::ifstream fileStream(filePath);
    std::string line;

    std::vector<ParsedInput> retVal;
    while (std::getline(fileStream, line))
    {
        const auto splits = Utils::str_split(line, " ");
        retVal.push_back(ParsedInput{/*handStr*/ splits[0], /*bid*/ std::stoi(splits[1])});
    }

    return retVal;
}

Hand::Type calculateHandType(const Hand &hand)
{
    std::map<int, int> countMap{};

    for (const auto &cardVal : hand.cards)
    {
        countMap[cardVal]++;
    }

    Hand::Type handType = Hand::Type::HighCard;

    for (const auto &valCountPair : countMap)
    {
        if (valCountPair.first == s_cardToNumberMap.at('*'))
        {
            continue;
        }

        switch (valCountPair.second)
        {
        case 2:
            if (handType == Hand::Type::OnePair)
            {
                return Hand::Type::TwoPair;
            }
            if (handType == Hand::Type::Three)
            {
                return Hand::Type::FullHouse;
            }
            handType = Hand::Type::OnePair;
            break;
        case 3:
            if (handType == Hand::Type::OnePair)
            {
                return Hand::Type::FullHouse;
            }
            handType = Hand::Type::Three;
            break;
        case 4:
            return Hand::Type::Four;
        case 5:
            return Hand::Type::Five;
        default:
            break;
        }
    }

    return handType;
}

Hand inputToHand(const ParsedInput &input)
{
    Hand retVal{};

    std::map<char, int> countMap{};

    const auto countAndMap = [&countMap](const char c) -> int
    {
        countMap[c]++;
        return s_cardToNumberMap.at(c);
    };
    {
        const std::vector<int> cardsVec = Utils::vMap_f<char, int>(std::vector(input.handStr.begin(), input.handStr.end()), countAndMap);
        retVal.cards = Utils::vecToArr<int, 5>(cardsVec);

        retVal.type = calculateHandType(retVal);
    }
    return retVal;
}

Play inputToPlay(const ParsedInput &input)
{
    return Play{inputToHand(input), input.bid};
}

Play applyNewJokerRule(const Play &play)
{
    Play retVal = play;
    retVal.hand.cards = Utils::vecToArr<int, 5>(
        Utils::vMap_f<int, int>(
            Utils::arrToVec<int>(play.hand.cards),
            [](const int val) -> int
            {
                if (val == s_cardToNumberMap.at('J'))
                {
                    return s_cardToNumberMap.at('*');
                }
                return val;
            }));

    // get the best hand type we can without J
    retVal.hand.type = calculateHandType(retVal.hand);

    return retVal;
}

Hand::Type calcBestHandWithJoker(const Hand &hand)
{
    const auto isJoker = [](const int card) -> bool
    {
        return card == s_cardToNumberMap.at('*');
    };

    const Hand::Type currentBest = hand.type;
    const int numOfJokers = Utils::vFilter_f<int>(Utils::arrToVec(hand.cards), isJoker).size();

    switch (numOfJokers)
    {
    case 5:
        // JJJJJ
    case 4:
        // *JJJJ
        return Hand::Type::Five;
    case 3:
        // **JJJ
        if (currentBest == Hand::Type::OnePair)
        {
            return Hand::Type::Five;
        }
        return Hand::Type::Four;
    case 2:
        // ***JJ
        if (currentBest == Hand::Type::Three)
        {
            return Hand::Type::Five;
        }
        if (currentBest == Hand::Type::OnePair)
        {
            return Hand::Type::Four;
        }
        return Hand::Type::Three;
    case 1:
        // ****J
        if (currentBest == Hand::Type::Four)
        {
            return Hand::Type::Five;
        }
        if (currentBest == Hand::Type::Three)
        {
            return Hand::Type::Four;
        }
        if (currentBest == Hand::Type::TwoPair)
        {
            return Hand::Type::FullHouse;
        }
        if (currentBest == Hand::Type::OnePair)
        {
            return Hand::Type::Three;
        }
        return Hand::Type::OnePair;
    case 0:
        return currentBest;
    default:
        assert(false && "HOW?");
        break;
    }
}

Play updateHandType(const Play &play){
    Play retVal = play;
    retVal.hand.type = calcBestHandWithJoker(play.hand);
    return retVal;
}

// g++ -std=c++17 -ggdb main.cpp -o main.exe -Wall -Wextra -Wpedantic -Werror
int main()
{
    std::vector<ParsedInput> input;
    // input = parseFile("./test.txt");
    input = parseFile("./input.txt");

    const auto playLtComp = [](const Play &a, const Play &b) -> bool
    {
        return a.hand < b.hand;
    };

    std::vector<Play> plays = Utils::vMap<ParsedInput, Play>(input, inputToPlay);
    std::sort(plays.begin(), plays.end(), playLtComp);

    ull p1 = 0;
    for (size_t i = 0; i < plays.size(); i++)
    {
        p1 += (i + 1) * plays[i].bid;
    }
    std::cout << "P1: " << p1 << "\n";

    plays = Utils::vMap<Play, Play>(plays, applyNewJokerRule);
    plays = Utils::vMap_f<Play, Play>(plays, updateHandType);

    std::sort(plays.begin(), plays.end(), playLtComp);

    ull p2 = 0;
    for (size_t i = 0; i < plays.size(); i++)
    {
        p2 += (i + 1) * plays[i].bid;
    }
    std::cout << "P2: " << p2 << "\n";
}