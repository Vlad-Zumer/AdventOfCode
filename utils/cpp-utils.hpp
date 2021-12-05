#ifndef CPP_VZ_UTILS
#define CPP_VZ_UTILS

#include<string>
#include<vector>
#include<assert.h>
#include<map>
#include<unordered_map>
#include<functional>
#include<ctype.h>

namespace Utils
{

    using namespace std;

    vector<string> str_split(const string str, const string tok)
    {
        assert(tok.size() > 0);
        vector<string> retVal = {};
        string tmp = str;
        int tokSize = tok.size();
        size_t findPos = tmp.find(tok);
        while(findPos < string::npos)
        {
            retVal.push_back(tmp.substr(0,findPos));
            if (findPos + tokSize < tmp.size())
            {
                tmp = tmp.substr(findPos + tokSize);
            }
            else
            {
                tmp = "";
                break;
            }
            findPos = tmp.find(tok);
        }

        retVal.push_back(tmp);

        return retVal;
    }

    string str_trim(const string str)
    {
        size_t begin = 0, end = str.size()-1;

        while (begin < str.size() && isspace(str[begin]))
        {
            begin++;
        }

        if (begin == str.size())
        {
            return "";
        }

        while (end > 0 && end > begin && isspace(str[end]))
        {
            end--;
        }

        if (end == 0)
        {
            return "";
        }

        return str.substr(begin,end-begin+1);
    }

    template<typename A>
    vector<A> vFilter(const vector<A> in, bool(*pred)(const A))
    {
        vector<A> retVal = {};

        for (const A s : in)
        {
            if (pred(s))
            {
                retVal.push_back(s);
            }
        }

        return retVal;
    }

    template<typename A>
    vector<A> vFilter_f(const vector<A> in, function<bool(const A)> pred)
    {
        vector<A> retVal = {};

        for (const A s : in)
        {
            if (pred(s))
            {
                retVal.push_back(s);
            }
        }

        return retVal;
    }

    template<typename A, typename B>
    map<A,B> mFilter(const map<A,B> in, bool(*pred)(const A, const B))
    {
        map<A,B> retVal = {};

        for (auto const s : in)
        {
            if (pred(s.first,s.second))
            {
                retVal.insert(s);
            }
        }

        return retVal;
    }

    template<typename A, typename B>
    map<A,B> mFilter_f(const map<A,B> in, function<bool(const A, const B)> pred)
    {
        map<A,B> retVal = {};

        for (auto const s : in)
        {
            if (pred(s.first,s.second))
            {
                retVal.insert(s);
            }
        }

        return retVal;
    }

    template<typename A, typename B>
    unordered_map<A,B> umFilter(const unordered_map<A,B> in, bool(*pred)(const A, const B))
    {
        unordered_map<A,B> retVal = {};

        for (auto const s : in)
        {
            if (pred(s.first,s.second))
            {
                retVal.insert(s);
            }
        }

        return retVal;
    }

    template<typename A, typename B>
    unordered_map<A,B> umFilter_f(const unordered_map<A,B> in, function<bool(const A, const B)> pred)
    {
        unordered_map<A,B> retVal = {};

        for (auto const s : in)
        {
            if (pred(s.first,s.second))
            {
                retVal.insert(s);
            }
        }

        return retVal;
    }

    template<typename A, typename B>
    vector<B> vMap(const vector<A> in, B(*func)(const A))
    {
        vector<B> retVal = {};

        for (auto const x : in)
        {
            retVal.push_back(func(x));
        }
        
        return retVal;
    }

    template<typename A, typename B>
    vector<B> vMap_f(const vector<A> in, function<B(const A)> func)
    {
        vector<B> retVal = {};

        for (auto const x : in)
        {
            retVal.push_back(func(x));
        }
        
        return retVal;
    }


    template<typename A, typename B>
    B vReduceL(const vector<A> in, B(*func)(const A, const B), const B initVal)
    {
        B retVal = initVal;

        for (auto const x : in)
        {
            retVal = func(x,retVal);
        }
        
        return retVal;
    }

    template<typename A, typename B>
    B vReduceL_f(const vector<A> in, function<B(const A, const B)> func, const B initVal)
    {
        B retVal = initVal;

        for (auto const x : in)
        {
            retVal = func(x,retVal);
        }
        
        return retVal;
    }

    template<typename A, typename B>
    vector<pair<A,B>> vZip(const vector<A> as, const vector<B> bs)
    {
        vector<pair<A,B>> ret;
        for (size_t i = 0; i < as.size() && i < bs.size(); i++)
        {
            ret.push_back({as[i],bs[i]});
        }
    }

    vector<long long> vRangeIncl(long long start, long long end)
    {
        assert(start < end);
        
        vector<long long> ret;
        for (long long i = start; i <= end ; i++)
        {
            ret.push_back(i);
        }
        return ret;
    }

    bool isEmptyStr(const string s)
    {
        return s.size() == 0;
    }

    bool isNotEmptyStr(const string s)
    {
        return !isEmptyStr(s);
    }






}
#endif