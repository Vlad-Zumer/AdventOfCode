#include<iostream>
#include<string>
#include<vector>
#include<unordered_set>

using namespace std;

#pragma region Input

vector<string> inputMap = { "...#..#.",
                            "..##.##.",
                            "..#.....",
                            "....#...",
                            "#.##...#",
                            "####..##",
                            "...##.#.",
                            "#.#.#...", };

#pragma endregion Input

#pragma region Vec3

typedef struct Vec3
{
    long x, y, z;

    const Vec3 operator+(const Vec3 &other) const
    {
        Vec3 newV3;
        newV3.x = x + other.x;
        newV3.y = y + other.y;
        newV3.z = z + other.z;
        return newV3;
    };
    
    const Vec3 operator-(const Vec3 other) const
    {
        return (*this)+Vec3{-other.x,-other.y,-other.y};
    };

    const Vec3 operator*(const long val) const
    {
        Vec3 newV3;
        newV3.x = x * val;
        newV3.y = y * val;
        newV3.z = z * val;
        return newV3;
    };
    
    const Vec3 operator/(const long val) const
    {
        Vec3 newV3;
        newV3.x = x / val;
        newV3.y = y / val;
        newV3.z = z / val;
        return newV3;
    };

    bool operator==(const Vec3 &other) const
    {
        return x == other.x && y == other.y && z == other.z;
    }

    bool operator!=(const Vec3 &other) const
    {
        return !((*this) == other);
    }

} Vec3l;

ostream& operator<<(ostream& os, const Vec3l vec)
{
    return os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")"; 
}

namespace std 
{
    template <> struct hash<Vec3l>
    {
        size_t operator()(const Vec3l& vec) const
        {
            hash<long> h_long;
            long x, y, z, salt;
            salt = 0x9e3779b9;
            x = (vec.x ^ salt) << vec.z;
            y = vec.y ^ (salt << x);
            z = ((vec.z << vec.y) ^ y);
            return h_long(x) ^ h_long(y) ^ h_long(z);
        }
    };
}

#pragma endregion Vec3

#pragma region Vec4

typedef struct Vec4
{
    long x, y, z, w;

    const Vec4 operator+(const Vec4 &other) const
    {
        Vec4 newV4;
        newV4.x = x + other.x;
        newV4.y = y + other.y;
        newV4.z = z + other.z;
        newV4.w = w + other.w;
        return newV4;
    };
    
    const Vec4 operator-(const Vec4 other) const
    {
        return (*this)+Vec4{-other.x,-other.y,-other.y,-other.w};
    };

    bool operator==(const Vec4 &other) const
    {
        return x == other.x && y == other.y && z == other.z && other.w == w;
    }

    bool operator!=(const Vec4 &other) const
    {
        return !((*this) == other);
    }

} Vec4l;

ostream& operator<<(ostream& os, const Vec4l vec)
{
    return os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << ")"; 
}

namespace std 
{
    template <> struct hash<Vec4l>
    {
        size_t operator()(const Vec4l& vec) const
        {
            hash<long> h_long;
            long x, y, z, w, salt;
            salt = 0x9e3779b9;
            x = (vec.x ^ salt) << vec.z;
            y = vec.y ^ (salt << x);
            z = ((vec.z << vec.y) ^ y);
            w = vec.w ^ x ^ salt;
            return h_long(x) ^ h_long(y) ^ h_long(z) ^ h_long(w);
        }
    };
}

#pragma endregion Vec4

namespace sol1
{
    using ActiveNode = Vec3l;

    const vector<Vec3l> neighbourDirs = 
    {
        //          TOP                                           MID                                   BOT
        {-1,-1,-1},  { 0,-1,-1},  { 1,-1,-1},    {-1,-1, 0},  { 0,-1, 0},  { 1,-1, 0},    {-1,-1, 1},  { 0,-1, 1},  { 1,-1, 1},
        {-1, 0,-1},  { 0, 0,-1},  { 1, 0,-1},    {-1, 0, 0}, /* 0, 0, 0*/  { 1, 0, 0},    {-1, 0, 1},  { 0 ,0, 1},  { 1, 0, 1},
        {-1, 1,-1},  { 0, 1,-1},  { 1, 1,-1},    {-1, 1, 0},  { 0, 1, 0},  { 1, 1, 0},    {-1, 1, 1},  { 0, 1, 1},  { 1, 1, 1},
    };

    vector<Vec3l> s2v (const unordered_set<Vec3l> in)
    {
        return vector<Vec3l>(in.begin(), in.end());
    }

    unordered_set<Vec3l> v2s (const vector<Vec3l> in)
    {

        return unordered_set<Vec3l>(in.begin(), in.end());
    }

    vector<ActiveNode> getActivesFromStrings(const vector<string> in)
    {
        vector<ActiveNode> actives;
        for (size_t y = 0; y < in.size(); y++)
        {
            for (size_t x = 0; x < in[y].size(); x++)
            {
                if (in[y][x] == '#')
                {
                    ActiveNode a = {(long)x, (long)y, 0};
                    actives.push_back(a);
                }
            }
        }

        return actives;
        
    }

    void dumpActives(const vector<ActiveNode> nodes)
    {
        cout<<"---ACTIVE NODES---"<<endl;
        for (size_t i = 0; i < nodes.size(); i++)
        {
            cout<<"\t"<<nodes[i]<<endl;
        }
        cout<<"--------END---------"<<endl;
    }


    vector<Vec3l> getNeighbouringPositions(const ActiveNode node)
    {
        vector<Vec3l> outVal;
        for (size_t i = 0; i < neighbourDirs.size(); i++)
        {
            outVal.push_back(node + neighbourDirs[i]);
        }
        
        return outVal;
    }

    int getActiveNeighbours(const Vec3l position, const unordered_set<ActiveNode> state)
    {
        vector<Vec3l> positionsToCheck = getNeighbouringPositions(position);
        int sum = 0;

        for (size_t i = 0; i < positionsToCheck.size(); i++)
        {
            if (state.find(positionsToCheck[i]) != state.end())
            {
                sum++;
            }
        }
        
        return sum;
    }

    unordered_set<ActiveNode> Step(const unordered_set<ActiveNode> prevState)
    {
        unordered_set<ActiveNode> newState = {};
        unordered_set<Vec3l> positionsToCheck = {};

        for (const ActiveNode& node : prevState)
        {
            vector<Vec3l> neighbours = getNeighbouringPositions(node);
            positionsToCheck.insert(neighbours.begin(),neighbours.end());
        }
        
        for (const Vec3l positionToCheck : positionsToCheck )
        {
            int activeNeighbours = getActiveNeighbours(positionToCheck, prevState);

            if (activeNeighbours == 3)
            {
                newState.insert(positionToCheck);
            }
            else if ( activeNeighbours == 2 && prevState.find(positionToCheck) != prevState.end())
            {
                newState.insert(positionToCheck);
            }
        }

        return newState;
    }
}

namespace sol2
{
    using ActiveNode = Vec4l;

    const vector<Vec4l> neighbourDirs = 
    {
        //          TOP                                           MID                                   BOT
        {-1,-1,-1,-1},  { 0,-1,-1,-1},  { 1,-1,-1,-1},    {-1,-1, 0,-1},  { 0,-1, 0,-1},  { 1,-1, 0,-1},    {-1,-1, 1,-1},  { 0,-1, 1,-1},  { 1,-1, 1,-1},
        {-1, 0,-1,-1},  { 0, 0,-1,-1},  { 1, 0,-1,-1},    {-1, 0, 0,-1},  { 0, 0, 0,-1},  { 1, 0, 0,-1},    {-1, 0, 1,-1},  { 0 ,0, 1,-1},  { 1, 0, 1,-1},  // w -1
        {-1, 1,-1,-1},  { 0, 1,-1,-1},  { 1, 1,-1,-1},    {-1, 1, 0,-1},  { 0, 1, 0,-1},  { 1, 1, 0,-1},    {-1, 1, 1,-1},  { 0, 1, 1,-1},  { 1, 1, 1,-1},

        //          TOP                                           MID                                   BOT
        {-1,-1,-1, 0},  { 0,-1,-1, 0},  { 1,-1,-1, 0},    {-1,-1, 0, 0},  { 0,-1, 0, 0},  { 1,-1, 0, 0},    {-1,-1, 1, 0},  { 0,-1, 1, 0},  { 1,-1, 1, 0},
        {-1, 0,-1, 0},  { 0, 0,-1, 0},  { 1, 0,-1, 0},    {-1, 0, 0, 0}, /* 0, 0, 0, 0*/  { 1, 0, 0, 0},    {-1, 0, 1, 0},  { 0 ,0, 1, 0},  { 1, 0, 1, 0},  // w 0
        {-1, 1,-1, 0},  { 0, 1,-1, 0},  { 1, 1,-1, 0},    {-1, 1, 0, 0},  { 0, 1, 0, 0},  { 1, 1, 0, 0},    {-1, 1, 1, 0},  { 0, 1, 1, 0},  { 1, 1, 1, 0},

        //          TOP                                           MID                                   BOT
        {-1,-1,-1, 1},  { 0,-1,-1, 1},  { 1,-1,-1, 1},    {-1,-1, 0, 1},  { 0,-1, 0, 1},  { 1,-1, 0, 1},    {-1,-1, 1, 1},  { 0,-1, 1, 1},  { 1,-1, 1, 1},
        {-1, 0,-1, 1},  { 0, 0,-1, 1},  { 1, 0,-1, 1},    {-1, 0, 0, 1},  { 0, 0, 0, 1},  { 1, 0, 0, 1},    {-1, 0, 1, 1},  { 0 ,0, 1, 1},  { 1, 0, 1, 1},  // w 1
        {-1, 1,-1, 1},  { 0, 1,-1, 1},  { 1, 1,-1, 1},    {-1, 1, 0, 1},  { 0, 1, 0, 1},  { 1, 1, 0, 1},    {-1, 1, 1, 1},  { 0, 1, 1, 1},  { 1, 1, 1, 1},
    };

    vector<Vec4l> s2v (const unordered_set<Vec4l> in)
    {
        return vector<Vec4l>(in.begin(), in.end());
    }

    unordered_set<Vec4l> v2s (const vector<Vec4l> in)
    {

        return unordered_set<Vec4l>(in.begin(), in.end());
    }

    vector<ActiveNode> getActivesFromStrings(const vector<string> in)
    {
        vector<ActiveNode> actives;
        for (size_t y = 0; y < in.size(); y++)
        {
            for (size_t x = 0; x < in[y].size(); x++)
            {
                if (in[y][x] == '#')
                {
                    ActiveNode a = {(long)x, (long)y, 0, 0};
                    actives.push_back(a);
                }
            }
        }

        return actives;
        
    }

    void dumpActives(const vector<ActiveNode> nodes)
    {
        cout<<"---ACTIVE NODES---"<<endl;
        for (size_t i = 0; i < nodes.size(); i++)
        {
            cout<<"\t"<<nodes[i]<<endl;
        }
        cout<<"--------END---------"<<endl;
    }


    vector<Vec4l> getNeighbouringPositions(const ActiveNode node)
    {
        vector<Vec4l> outVal;
        for (size_t i = 0; i < neighbourDirs.size(); i++)
        {
            outVal.push_back(node + neighbourDirs[i]);
        }
        
        return outVal;
    }

    int getActiveNeighbours(const Vec4l position, const unordered_set<ActiveNode> state)
    {
        vector<Vec4l> positionsToCheck = getNeighbouringPositions(position);
        int sum = 0;

        for (size_t i = 0; i < positionsToCheck.size(); i++)
        {
            if (state.find(positionsToCheck[i]) != state.end())
            {
                sum++;
            }
        }
        
        return sum;
    }

    unordered_set<ActiveNode> Step(const unordered_set<ActiveNode> prevState)
    {
        unordered_set<ActiveNode> newState = {};
        unordered_set<Vec4l> positionsToCheck = {};

        for (const ActiveNode& node : prevState)
        {
            vector<Vec4l> neighbours = getNeighbouringPositions(node);
            positionsToCheck.insert(neighbours.begin(),neighbours.end());
        }
        
        for (const Vec4l positionToCheck : positionsToCheck )
        {
            int activeNeighbours = getActiveNeighbours(positionToCheck, prevState);

            if (activeNeighbours == 3)
            {
                newState.insert(positionToCheck);
            }
            else if ( activeNeighbours == 2 && prevState.find(positionToCheck) != prevState.end())
            {
                newState.insert(positionToCheck);
            }
        }

        return newState;
    }
}

int main()
{
    unordered_set<sol1::ActiveNode> activeNodes1 = sol1::v2s(sol1::getActivesFromStrings(inputMap));
    unordered_set<sol2::ActiveNode> activeNodes2 = sol2::v2s(sol2::getActivesFromStrings(inputMap));
    for (size_t i = 0; i < 6; i++)
    {
        activeNodes1 = sol1::Step(activeNodes1);
        activeNodes2 = sol2::Step(activeNodes2);
    }
    cout<<activeNodes1.size()<<endl<<activeNodes2.size()<<endl;

    return 0;
}