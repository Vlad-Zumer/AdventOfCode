#ifndef CPP_VZ_UTILS_PERF
#define CPP_VZ_UTILS_PERF

#include<string>
#include<functional>
#include<chrono>

namespace Utils
{
    using namespace std;

    template<typename T>
    struct TimedReturnedObject
    {
        chrono::nanoseconds Nanoseconds;
        T Value;
    };

    template<typename T>
    TimedReturnedObject<T> TimeFunction(T(*func)())
    {        
        auto start = chrono::high_resolution_clock::now();
        T val = func();
        auto end = chrono::high_resolution_clock::now();

        TimedReturnedObject<T> retObj;
        retObj.Nanoseconds = end - start;
        retObj.Value = val;

        return retObj;
    }

    template<typename T>
    TimedReturnedObject<T> TimeFunction_f(function<T()> func)
    {
        auto start = chrono::high_resolution_clock::now();
        T val = func();
        auto end = chrono::high_resolution_clock::now();

        TimedReturnedObject<T> retObj;
        retObj.Nanoseconds = end - start;
        retObj.Value = val;

        return retObj;
    }

}

#endif