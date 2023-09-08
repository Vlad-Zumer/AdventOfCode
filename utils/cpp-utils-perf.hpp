#ifndef CPP_VZ_UTILS_PERF
#define CPP_VZ_UTILS_PERF

#include<functional>
#include<chrono>
#include<utility>

//////////////////////////////////////////////
//          Wrapper Examples
//////////////////////////////////////////////
//      template<class TOut, class... FuncTsIn, class... WrapperTsIn>
//      TOut wrap(TOut(*func)(FuncTsIn...), WrapperTsIn... args)
//      {
//          std::cout<<"Wrapped function MFER!\n";
//          return func(std::forward<WrapperTsIn>(args)...);
//      }
//
//      template<class CL, class TOut, class... FuncTsIn, class... WrapperTsIn>
//      TOut wrap(CL* self, TOut(CL::*func)(FuncTsIn...), WrapperTsIn... args)
//      {
//          std::cout<<"Wrapped method MFER!\n";
//          return (self->*func)(std::forward<WrapperTsIn>(args)...);
//      }
//
//      template<class CL, class TOut, class... FuncTsIn, class... WrapperTsIn>
//      TOut wrap(const CL* self, TOut(CL::*func)(FuncTsIn...) const, WrapperTsIn... args)
//      {
//          std::cout<<"Wrapped const method MFER!\n";
//          return (self->*func)(std::forward<WrapperTsIn>(args)...);
//      }
//
//      template<class TOut, class... FuncTsIn, class... WrapperTsIn>
//      TOut wrap(std::function<TOut(FuncTsIn...)> func, WrapperTsIn... args)
//      {
//          std::cout<<"Wrapped with std::function MFER!\n";
//          return func(std::forward<WrapperTsIn>(args)...);
//      }
//////////////////////////////////////////////

namespace Utils
{
    using namespace std;

    template<typename T>
    struct TimedReturnedObject
    {
        chrono::nanoseconds Nanoseconds;
        T Value;
    };

    template<class TOut, class... FuncTsIn, class... WrapperTsIn>
    TimedReturnedObject<TOut> TimeFunction(TOut(*func)(FuncTsIn...), WrapperTsIn... args)
    {
        TimedReturnedObject<TOut> retObj;

        auto start = chrono::high_resolution_clock::now();
        retObj.Value = func(std::forward<WrapperTsIn>(args)...);
        auto end = chrono::high_resolution_clock::now();

        retObj.Nanoseconds = end - start;
        return retObj;
    }

    template<class TOut, class... FuncTsIn, class... WrapperTsIn>
    TimedReturnedObject<TOut> TimeFunction(std::function<TOut(FuncTsIn...)> func, WrapperTsIn... args)
    {
        TimedReturnedObject<TOut> retObj;

        auto start = chrono::high_resolution_clock::now();
        retObj.Value = func(std::forward<WrapperTsIn>(args)...);
        auto end = chrono::high_resolution_clock::now();

        retObj.Nanoseconds = end - start;
        return retObj;
    }

    template<class CL, class TOut, class... FuncTsIn, class... WrapperTsIn>
    TimedReturnedObject<TOut> TimeFunction(CL* self, TOut(CL::*func)(FuncTsIn...), WrapperTsIn... args)
    {
        TimedReturnedObject<TOut> retObj;

        auto start = chrono::high_resolution_clock::now();
        retObj.Value = (self->*func)(std::forward<WrapperTsIn>(args)...);
        auto end = chrono::high_resolution_clock::now();

        retObj.Nanoseconds = end - start;
        return retObj;
    }
    
    template<class CL, class TOut, class... FuncTsIn, class... WrapperTsIn>
    TimedReturnedObject<TOut> TimeFunction(const CL* self, TOut(CL::*func)(FuncTsIn...) const, WrapperTsIn... args)
    {
        TimedReturnedObject<TOut> retObj;

        auto start = chrono::high_resolution_clock::now();
        retObj.Value = (self->*func)(std::forward<WrapperTsIn>(args)...);
        auto end = chrono::high_resolution_clock::now();

        retObj.Nanoseconds = end - start;
        return retObj;
    }

    ////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////
    //         DEAL WITH "void" NOT BEING A PROPER RETURN TYPE
    //              TEMPLATE SPECIALIZATION FOR "void"
    ////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////

    template<>
    struct TimedReturnedObject<void>
    {
        chrono::nanoseconds Nanoseconds;
    };

    template<class... FuncTsIn, class... WrapperTsIn>
    TimedReturnedObject<void> TimeFunction(void(*func)(FuncTsIn...), WrapperTsIn... args)
    {
        TimedReturnedObject<void> retObj;

        auto start = chrono::high_resolution_clock::now();
        func(std::forward<WrapperTsIn>(args)...);
        auto end = chrono::high_resolution_clock::now();

        retObj.Nanoseconds = end - start;
        return retObj;
    }

    template<class... FuncTsIn, class... WrapperTsIn>
    TimedReturnedObject<void> TimeFunction(std::function<void(FuncTsIn...)> func, WrapperTsIn... args)
    {
        TimedReturnedObject<void> retObj;

        auto start = chrono::high_resolution_clock::now();
        func(std::forward<WrapperTsIn>(args)...);
        auto end = chrono::high_resolution_clock::now();

        retObj.Nanoseconds = end - start;
        return retObj;
    }

    template<class CL, class... FuncTsIn, class... WrapperTsIn>
    TimedReturnedObject<void> TimeFunction(CL* self, void(CL::*func)(FuncTsIn...), WrapperTsIn... args)
    {
        TimedReturnedObject<void> retObj;

        auto start = chrono::high_resolution_clock::now();
        (self->*func)(std::forward<WrapperTsIn>(args)...);
        auto end = chrono::high_resolution_clock::now();

        retObj.Nanoseconds = end - start;
        return retObj;
    }
    
    template<class CL, class... FuncTsIn, class... WrapperTsIn>
    TimedReturnedObject<void> TimeFunction(const CL* self, void(CL::*func)(FuncTsIn...) const, WrapperTsIn... args)
    {
        TimedReturnedObject<void> retObj;

        auto start = chrono::high_resolution_clock::now();
        (self->*func)(std::forward<WrapperTsIn>(args)...);
        auto end = chrono::high_resolution_clock::now();

        retObj.Nanoseconds = end - start;
        return retObj;
    }

}

#endif