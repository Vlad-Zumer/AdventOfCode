#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <array>
#include <map>
#include <algorithm>
#include <numeric>
#include "../../utils/cpp-utils.hpp"

using ull = unsigned long long;
using ll = long long;
using ld = long double;

enum class Move
{
    Left = 0,
    Right = 1
};

using Moves = std::vector<Move>;

struct Node
{
    std::string name;
    std::string leftName, rightName;
};

using NodeMap = std::map<std::string, Node>;

struct ParsedInput
{
    NodeMap nodeMap;
    Moves moves;
};

ParsedInput parseFile(const std::string &filePath)
{
    std::ifstream fileStream(filePath);
    std::string line;

    ParsedInput retVal;

    // parse moves
    std::getline(fileStream, line);
    const auto charToMove = [](const char c) -> Move
    {
        if (c == 'L')
        {
            return Move::Left;
        }
        if (c == 'R')
        {
            return Move::Right;
        }
        assert(false && "WTF? HOW? FU");
    };
    retVal.moves = Utils::vMap_f<char, Move>(std::vector<char>(line.begin(), line.end()), charToMove);

    std::getline(fileStream, line);
    while (std::getline(fileStream, line))
    {
        const auto nameAndNodes = Utils::str_split(line, "=");
        Node n{};
        n.name = Utils::str_trim(nameAndNodes[0]);

        const auto LRNodeNames = Utils::vMap(Utils::str_split(nameAndNodes[1], ","), Utils::str_trim);
        n.leftName = Utils::str_chopStart(LRNodeNames[0], "(");
        n.rightName = Utils::str_chopEnd(LRNodeNames[1], ")");

        retVal.nodeMap[n.name] = n;
    }

    return retVal;
}

std::string nextNodeName(const Node &node, const Move &move)
{
    if (move == Move::Left)
    {
        return node.leftName;
    }
    if (move == Move::Right)
    {
        return node.rightName;
    }
    assert(false && "U fucked up big time");
}

ull MoveRoute(const NodeMap &nodes, const Moves &moves, const std::string &from, std::function<bool(const Node &)> finished)
{
    Node curr = nodes.at(from);
    ull index = 0;
    while (!finished(curr))
    {
        const Move &move = moves.at(index % moves.size());
        curr = nodes.at(nextNodeName(curr, move));
        index++;
    }

    return index;
}

ull MoveRouteP1(const NodeMap &nodes, const Moves &moves, const std::string &from, const std::string &to)
{
    const auto finished = [&to](const Node &node) -> bool
    {
        return node.name == to;
    };

    return MoveRoute(nodes, moves, from, finished);
}

ull MoveRouteP2(const NodeMap &nodes, const Moves &moves, const std::string &fromEndsWith, const std::string &toEndsWith)
{

    const auto strNodePairToNode = [](const std::pair<std::string, Node> &p) -> Node
    {
        return p.second;
    };

    const auto nodeEndsWithStart = [&fromEndsWith](const Node &node) -> bool
    {
        return Utils::str_endsWith(node.name, fromEndsWith);
    };

    const auto finished = [&toEndsWith](const Node &node) -> bool
    {
        return Utils::str_endsWith(node.name, toEndsWith);
    };

    std::vector<Node> startNodes = Utils::vFilter_f<Node>(
        Utils::vMap_f<std::pair<std::string, Node>, Node>(Utils::mapToVec(nodes), strNodePairToNode),
        nodeEndsWithStart);

    std::vector<std::pair<Node, ull>> nodeAndCycleTimes = Utils::vMap_f<Node, std::pair<Node, ull>>(
        startNodes,
        [&nodes, &moves, finished](const Node &node) -> std::pair<Node, ull>
        {
            return {node, MoveRoute(nodes, moves, node.name, finished)};
        });

    ull retVal = Utils::vReduceL_f<std::pair<Node, ull>, ull>(
        nodeAndCycleTimes,
        [](const std::pair<Node, ull> &pair, ull acc) -> ull
        {
            return std::lcm(acc, pair.second);
        },
        1);

    // Not a general graph, just many cycles
    return retVal;
}

// g++ -std=c++17 -ggdb main.cpp -o main.exe -Wall -Wextra -Wpedantic -Werror && ./main
int main()
{
    ParsedInput input;
    // input = parseFile("./test.txt");
    input = parseFile("./input.txt");

    ull p1 = MoveRouteP1(input.nodeMap, input.moves, "AAA", "ZZZ");
    std::cout << "P1: " << p1 << "\n";

    // Not a general graph, just many cycles
    ull p2 = MoveRouteP2(input.nodeMap, input.moves, "A", "Z");
    std::cout << "P2: " << p2 << "\n";
    return 0;
}