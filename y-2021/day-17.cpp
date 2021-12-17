#include <iostream>
#include <utility>
using namespace std;
using ull = unsigned long long;

const pair<int,int> XTarget = {217,240};
const pair<int,int> YTarget = {-126,-69};

ull CalculateBallisticMotion(const int X0, const int V0, const int a, const int t, const bool stopsAt0)
{
    if (t == 0)
    {
        return X0;
    }
    if(stopsAt0)
    {
        int TV0 = V0 / a;
        TV0 = TV0 < 0 ? -TV0 : TV0;
        int T = TV0 < t ? TV0 : t;
        return X0 + (V0*T) + ((a*T*(T-1))/2);
    }
    return X0 + (V0 * t) + (a*t*(t-1))/2;
}

ull part1()
{
    int maxYVel = 0;
    for (int yVel = 0; yVel < 10000; yVel++)
    {
        for (int t = 0; t < 10000; t++)
        {
            int yPos = CalculateBallisticMotion(0,yVel,-1,t,false);
            if (yPos >= YTarget.first && yPos <= YTarget.second)
            {
                maxYVel = yVel;
            }
        }
        
    }
    
    return (maxYVel * (maxYVel+1))/2;
}


ull part2()
{ 
    ull count = 0;
    for (int xVel = 0; xVel < XTarget.second+1; xVel++)
    {
        for (int yVel = YTarget.first; yVel < 1000; yVel++)
        {
            for (int t = 0; t < 1000; t++)
            {
                int xPos = CalculateBallisticMotion(0,xVel,-1,t,true);
                int yPos = CalculateBallisticMotion(0,yVel,-1,t,false);
                if (yPos >= YTarget.first && yPos <= YTarget.second &&
                    xPos >= XTarget.first && xPos <= XTarget.second)
                {
                    count++;
                    break;
                }
            }
        }
    }
    return count;
}

int main()
{
    cout<<"Part 1:"<<part1()<<endl;
    cout<<"Part 2:"<<part2()<<endl;    

    return 0;
}