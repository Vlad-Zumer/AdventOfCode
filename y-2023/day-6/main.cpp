#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <limits>
#include "../../utils/cpp-utils.hpp"

using ull = unsigned long long;
using ll = long long;
using ld = long double;

struct QuadraticSolution
{
    enum class Type
    {
        None,
        One,
        Two,
    };
    struct Empty
    {
    };
    struct DoubleSol
    {
        ld v1, v2;
    };

    Type type;
    union data
    {
        ld val;
        DoubleSol vals;
        Empty empty;
    } solution;
};

QuadraticSolution solveQuadratic(ld a, ld b, ld c)
{
    QuadraticSolution retVal{};

    ld delta = (b * b) - (4.0 * a * c);

    if (delta < 0.0)
    {
        retVal.type = QuadraticSolution::Type::None;
        return retVal;
    }

    if (std::abs(delta) <= std::numeric_limits<ld>::epsilon())
    {
        retVal.type = QuadraticSolution::Type::One;
        retVal.solution.val = -b / (2.0 * a);

        return retVal;
    }

    ld sqrtD = std::sqrt(delta);
    ld v2 = (-b + sqrtD) / (2.0 * a);
    ld v1 = (-b - sqrtD) / (2.0 * a);

    if (v1 > v2)
    {
        std::swap(v1, v2);
    }

    retVal.type = QuadraticSolution::Type::Two;
    retVal.solution.vals.v1 = v1;
    retVal.solution.vals.v2 = v2;

    return retVal;
}

std::vector<int> parseLine(const std::string &line)
{
    const auto strToInt = [](const std::string s) -> int
    {
        return std::stoi(s);
    };

    return Utils::vMap_f<std::string, int>(
        Utils::vFilter<std::string>(
            Utils::str_split(Utils::str_split(line, ":")[1], " "),
            Utils::isNotEmptyStr),
        strToInt);
}

void parseFile(const std::string &filePath, std::vector<int> &outTimes, std::vector<int> &outDistances)
{
    std::ifstream fileStream(filePath);
    std::string line;

    std::getline(fileStream, line);
    outTimes = parseLine(line);

    std::getline(fileStream, line);
    outDistances = parseLine(line);
}

ll calcWinningWays(ll availableTime, ll distanceToBeat)
{
    ld a = -1.0;
    ld b = availableTime;
    ld c = -distanceToBeat;
    QuadraticSolution sol = solveQuadratic(a, b, c);

    if (sol.type == QuadraticSolution::Type::None)
    {
        return 0;
    }

    if (sol.type == QuadraticSolution::Type::One)
    {
        return 1;
    }

    ll v1 = std::floor(sol.solution.vals.v1);
    ll v2 = std::ceil(sol.solution.vals.v2);

    return (v2 - 1) - (v1 + 1) + 1;
}

//   Record Distance = d
//              Time = t
//             Speed = s (parameter)
//       Charge Time = s
//         Move Time = t-s
// Traveled Distance = (t-s) * s = ts - s^2
// ----------------------------------------
// ts - s^2  = d <=> -s^2 + ts - d = 0 => solve for s
// to beat "Record Distance" s in range [s1+1, s2-1]

// g++ -std=c++17 main.cpp -o main.exe -Wall -Wextra -Wpedantic -Werror
int main()
{
    std::vector<int> times;
    std::vector<int> distances;
    // parseFile("./test.txt", times, distances);
    parseFile("./input.txt", times, distances);

    std::vector<int> winningWays = Utils::vMap_f<std::pair<int, int>, int>(
        Utils::vZip<int, int>(times, distances),
        [](const std::pair<int, int> &timeDistPair) -> int
        {
            return calcWinningWays(timeDistPair.first, timeDistPair.second);
        });

    ull p1 = Utils::vReduceL_f<int, ull>(
        /*vec*/ winningWays,
        /*func*/ [](const int &el, ull acc) -> ull
        { return el * acc; },
        /*init*/ 1);

    std::cout << "P1:" << p1 << "\n";

    const auto concatNum = [](const int i, std::string acc) -> std::string
    {
        return acc + std::to_string(i);
    };
    std::string timeStr = Utils::vReduceL_f<int, std::string>(times, concatNum, "");
    std::string distStr = Utils::vReduceL_f<int, std::string>(distances, concatNum, "");

    ull p2 = calcWinningWays(std::stoll(timeStr), std::stoll(distStr));

    std::cout << "P2:" << p2 << "\n";

    return 0;
}