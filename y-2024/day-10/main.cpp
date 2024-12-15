#include <iostream>
#include <fstream>
#include <cstdint>
#include <string>
#include <vector>
#include <deque>
#include <unordered_set>
#include "../../utils/cpp-utils.hpp"
#include "../../utils/cpp-maths-utils.hpp"

using u8 = std::uint8_t;
using i8 = std::int8_t;
using ull = unsigned long long;
using ll = long long;

using Map = std::vector<std::vector<u8>>;
using Vec2D = MathsUtil::Vec2;

void parseFile(const std::string &filePath, Map &map)
{
    std::ifstream fileStream(filePath);
    std::string line;
    const auto toU8 = [](const char &c) -> u8
    {
        return (c - '0');
    };

    while (std::getline(fileStream, line))
    {
        map.push_back(Utils::vMap<char, u8>(Utils::strToVec(line), toU8));
    }
}

enum Direction
{
    N,
    S,
    E,
    W
};

Vec2D getVec(const Direction &dir)
{
    switch (dir)
    {
    case N:
        return Vec2D({0, -1});
    case E:
        return Vec2D({1, 0});
    case S:
        return Vec2D({0, 1});
    case W:
        return Vec2D({-1, 0});
    }
    assert(false && "unrecognized direction");
    return Vec2D();
}

bool isInsideMap(const Vec2D &pos, const Vec2D &xySize)
{
    return pos[0] >= 0 && pos[1] >= 0 && pos[0] < xySize[0] && pos[1] < xySize[1];
}

ull part1(const Map &map)
{
    Vec2D xySize = Vec2D({(int)map[0].size(), (int)map.size()});
    Direction dirs[] = {Direction::N, Direction::E, Direction::S, Direction::W};

    std::deque<std::vector<Vec2D>> openSet = {};
    for (size_t y = 0; y < map.size(); y++)
    {
        for (size_t x = 0; x < map[y].size(); x++)
        {
            if (map[y][x] == 0)
            {
                openSet.push_back({Vec2D{(int)x, (int)y}});
            }
        }
    }

    ull ret = 0;

    std::vector<std::pair<Vec2D, Vec2D>> seen = {};
    const auto contains = [](const std::vector<std::pair<Vec2D, Vec2D>> &v, const std::pair<Vec2D, Vec2D> &x) -> bool
    {
        return std::find_if(v.begin(), v.end(), [&x](const std::pair<Vec2D, Vec2D> &y) -> bool
                            { return x.first == y.first && x.second == y.second; }) != v.end();
    };

    while (!openSet.empty())
    {
        const std::vector<Vec2D> currPath = openSet.front();
        const Vec2D currPos = currPath.back();
        const u8 currVal = map[currPos[1]][currPos[0]];
        openSet.pop_front();

        if (currVal == 9)
        {
            const Vec2D originalPos = currPath.front();
            if (!contains(seen, {originalPos, currPos}))
            {
                seen.push_back({originalPos, currPos});
                ret++;
            }
            continue;
        }

        for (const Direction &dir : dirs)
        {
            Vec2D nextPos = currPos + getVec(dir);
            if (!isInsideMap(nextPos, xySize))
            {
                continue;
            }
            u8 nextVal = map[nextPos[1]][nextPos[0]];
            if (nextVal == currVal + 1)
            {
                std::vector<Vec2D> nextPath = currPath;
                nextPath.push_back(nextPos);
                openSet.push_back(nextPath);
            }
        }
    }

    return ret;
}

ull part2(const Map &map)
{
    Vec2D xySize = Vec2D({(int)map[0].size(), (int)map.size()});
    Direction dirs[] = {Direction::N, Direction::E, Direction::S, Direction::W};

    std::deque<std::vector<Vec2D>> openSet = {};
    for (size_t y = 0; y < map.size(); y++)
    {
        for (size_t x = 0; x < map[y].size(); x++)
        {
            if (map[y][x] == 0)
            {
                openSet.push_back({Vec2D{(int)x, (int)y}});
            }
        }
    }

    ull ret = 0;

    std::vector<std::vector<Vec2D>> seen = {};
    const auto contains = [](const std::vector<std::vector<Vec2D>> &v, const std::vector<Vec2D> &x) -> bool
    {
        return std::find_if(v.begin(), v.end(), [&x](const std::vector<Vec2D> &y) -> bool
                            { return Utils::vEq(x, y); }) != v.end();
    };

    while (!openSet.empty())
    {
        const std::vector<Vec2D> currPath = openSet.front();
        const Vec2D currPos = currPath.back();
        const u8 currVal = map[currPos[1]][currPos[0]];
        openSet.pop_front();

        if (currVal == 9)
        {
            if (!contains(seen, currPath))
            {
                seen.push_back(currPath);
                ret++;
            }
            continue;
        }

        for (const Direction &dir : dirs)
        {
            Vec2D nextPos = currPos + getVec(dir);
            if (!isInsideMap(nextPos, xySize))
            {
                continue;
            }
            u8 nextVal = map[nextPos[1]][nextPos[0]];
            if (nextVal == currVal + 1)
            {
                std::vector<Vec2D> nextPath = currPath;
                nextPath.push_back(nextPos);
                openSet.push_back(nextPath);
            }
        }
    }

    return ret;
}

// g++ -std=c++20 main.cpp -o main.exe -Wall -Wextra -Wpedantic -Werror
int main()
{
    Map map = {};
    // parseFile("./test.txt", map);
    parseFile("./input.txt", map);
    ull p1 = part1(map);
    ull p2 = part2(map);

    std::cout << "P1:" << p1 << "\n";
    std::cout << "P2:" << p2 << "\n";

    return 0;
}