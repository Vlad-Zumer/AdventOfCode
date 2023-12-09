#ifndef CPP_VZ_UTILS
#define CPP_VZ_UTILS

#include <string>
#include <vector>
#include <assert.h>
#include <map>
#include <array>
#include <unordered_map>
#include <functional>
#include <ctype.h>

namespace Utils
{

    using namespace std;

    string str_reverse(const string &str)
    {
        string out;
        for (auto it = str.rbegin(); it != str.rend(); it++)
        {
            out += *it;
        }
        return out;
    }

    vector<string> str_split(const string &str, const string &tok)
    {
        assert(tok.size() > 0);
        vector<string> retVal = {};
        string tmp = str;
        int tokSize = tok.size();
        size_t findPos = tmp.find(tok);
        while (findPos < string::npos)
        {
            retVal.push_back(tmp.substr(0, findPos));
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

    bool str_contains(const string &str, const char c)
    {
        return str.find(c) != std::string::npos;
    }

    bool str_contains(const string &str, const string &needle)
    {
        return str.find(needle) != std::string::npos;
    }

    bool str_startsWith(const string &str, const string &pattern)
    {
        if (str.size() < pattern.size())
        {
            return false;
        }

        for (size_t i = 0; i < pattern.size(); i++)
        {
            if (str.at(i) != pattern.at(i))
            {
                return false;
            }
        }

        return true;
    }

    bool str_endsWith(const string &str, const string &pattern)
    {
        //   STR : ****ABC
        //   PAT : ABC

        // R_STR : CBA****
        // R_PAT : CBA

        const string rStr = str_reverse(str);
        const string rPattern = str_reverse(pattern);

        return str_startsWith(rStr, rPattern);
    }

    string str_chopStart(const string &str, const string &setCharsToChop)
    {
        size_t startIndex = 0;

        while (startIndex < str.size())
        {
            if (!str_contains(setCharsToChop, str.at(startIndex)))
            {
                break;
            }
            startIndex++;
        }

        if (startIndex == str.size())
        {
            return "";
        }

        return str.substr(startIndex);
    }

    string str_chopEnd(const string &str, const string &setCharsToChop)
    {
        size_t startIndex = 0;
        const string rStr = str_reverse(str);
        while (startIndex < rStr.size())
        {
            if (!str_contains(setCharsToChop, rStr.at(startIndex)))
            {
                break;
            }
            startIndex++;
        }

        if (startIndex == rStr.size())
        {
            return "";
        }

        return str_reverse(rStr.substr(startIndex));
    }

    string str_trim(const string &str)
    {
        size_t begin = 0, end = str.size() - 1;

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

        return str.substr(begin, end - begin + 1);
    }

    template <typename A, typename B>
    vector<pair<A, B>> mapToVec(const map<A, B> &map)
    {
        vector<pair<A, B>> retVal;
        for (const auto &kvp : map)
        {
            retVal.push_back(kvp);
        }
        return retVal;
    }

    template <typename A, typename B>
    vector<pair<A, B>> umapToVec(const unordered_map<A, B> &map)
    {
        vector<pair<A, B>> retVal;
        for (const auto &kvp : map)
        {
            retVal.push_back(kvp);
        }
        return retVal;
    }

    template <typename A, size_t N>
    vector<A> arrToVec(const array<A, N> &arr)
    {
        return vector<A>(arr.begin(), arr.end());
    }

    template <typename A, size_t N>
    array<A, N> vecToArr(const vector<A> &vec)
    {
        array<A, N> arr;
        for (size_t i = 0; i < N; i++)
        {
            arr[i] = vec[i];
        }
        return arr;
    }

    template <typename A, size_t N>
    bool arrEq(const array<A, N> &a, const array<A, N> b)
    {
        for (size_t i = 0; i < N; i++)
        {
            if (a[i] != b[i])
            {
                return false;
            }
        }

        return true;
    }

    template <typename A>
    bool vEq(const vector<A> &a, const vector<A> &b)
    {
        if (a.size() != b.size())
        {
            return false;
        }

        for (size_t i = 0; i < a.size(); i++)
        {
            if (a[i] != b[i])
            {
                return false;
            }
        }

        return true;
    }

    template <typename A>
    vector<A> vFilter(const vector<A> &in, bool (*pred)(const A &))
    {
        vector<A> retVal = {};

        for (const A &s : in)
        {
            if (pred(s))
            {
                retVal.push_back(s);
            }
        }

        return retVal;
    }

    template <typename A>
    vector<A> vFilter_f(const vector<A> &in, function<bool(const A &)> pred)
    {
        vector<A> retVal = {};

        for (const A &s : in)
        {
            if (pred(s))
            {
                retVal.push_back(s);
            }
        }

        return retVal;
    }

    template <typename A, typename B>
    map<A, B> mFilter(const map<A, B> &in, bool (*pred)(const A &, const B &))
    {
        map<A, B> retVal = {};

        for (const auto &s : in)
        {
            if (pred(s.first, s.second))
            {
                retVal.insert(s);
            }
        }

        return retVal;
    }

    template <typename A, typename B>
    map<A, B> mFilter_f(const map<A, B> &in, function<bool(const A &, const B &)> pred)
    {
        map<A, B> retVal = {};

        for (const auto &s : in)
        {
            if (pred(s.first, s.second))
            {
                retVal.insert(s);
            }
        }

        return retVal;
    }

    template <typename A, typename B>
    unordered_map<A, B> umFilter(const unordered_map<A, B> &in, bool (*pred)(const A &, const B &))
    {
        unordered_map<A, B> retVal = {};

        for (const auto &s : in)
        {
            if (pred(s.first, s.second))
            {
                retVal.insert(s);
            }
        }

        return retVal;
    }

    template <typename A, typename B>
    unordered_map<A, B> umFilter_f(const unordered_map<A, B> &in, function<bool(const A &, const B &)> pred)
    {
        unordered_map<A, B> retVal = {};

        for (const auto &s : in)
        {
            if (pred(s.first, s.second))
            {
                retVal.insert(s);
            }
        }

        return retVal;
    }

    template <typename A, typename B>
    vector<B> vMap(const vector<A> &in, B (*func)(const A &))
    {
        vector<B> retVal = {};

        for (const auto &x : in)
        {
            retVal.push_back(func(x));
        }

        return retVal;
    }

    template <typename A, typename B>
    vector<B> vMap_f(const vector<A> &in, function<B(const A &)> func)
    {
        vector<B> retVal = {};

        for (const auto &x : in)
        {
            retVal.push_back(func(x));
        }

        return retVal;
    }

    template <typename A, typename B>
    B vReduceL(const vector<A> &in, B (*func)(const A &, const B), const B initVal)
    {
        B retVal = initVal;

        for (const auto &x : in)
        {
            retVal = func(x, retVal);
        }

        return retVal;
    }

    template <typename A, typename B>
    B vReduceL_f(const vector<A> &in, function<B(const A &, const B)> func, const B initVal)
    {
        B retVal = initVal;

        for (const auto &x : in)
        {
            retVal = func(x, retVal);
        }

        return retVal;
    }

    /// Good idea for multi threading/GPU systems
    template <typename A>
    A vFastReduceAssociative(vector<A> in, A (*func)(const A, const A))
    {
        static_assert(std::bool_constant<false>::value, "Use Utils::vReduceL or vReduceL_f as they get better performance when optimized (on a single thread).");
        /*
        in: 0 1 2 3 4 5 6 7 8 9 10
        process:
            in[0] = f(0,1)
            in[2] = f(2,3)
            in[4] = f(4,5)
            in[6] = f(6,7)
            in[8] = f(8,9)
            in[10] = 10
        --------------
        in: f0 x f2 x f4 x f6 x f8 x 10
        process:
            in[0] = f(0,2)
            in[4] = f(4,6)
            in[8] = f(8,10)
        --------------
        in: ff0 x x x ff4 x x x ff8 x x
        process:
            in[0] = f(0,4)
            in[8] = in[8]
        --------------
        in: fff0 x x x x x x x ff8 x x
        process:
            in[0] = f(0,8)
        --------------
        in: ffff0 x x x x x x x x x x x
        */
        const size_t inSize = in.size();
        size_t step = 2;
        while (step / 2 <= inSize)
        {
            for (size_t i = 0; i + step / 2 < inSize; i += step)
            {
                in[i] = func(in[i], in[i + step / 2]);
            }
            step *= 2;
        }
        return in[0];
    }

    template <typename A, typename B>
    vector<pair<A, B>> vZip(const vector<A> &as, const vector<B> &bs)
    {
        vector<pair<A, B>> ret;
        for (size_t i = 0; i < as.size() && i < bs.size(); i++)
        {
            ret.push_back({as[i], bs[i]});
        }

        return ret;
    }

    template <typename T>
    vector<T> vRangeIncl(const T start, const T end)
    {
        static_assert(is_arithmetic<T>::value, "Cannot make vector of non-arithmetic type");
        assert(start < end);

        vector<T> ret;

        for (T i = start; i <= end; i++)
        {
            ret.push_back(i);
        }

        return ret;
    }

    bool isEmptyStr(const string &s)
    {
        return s.size() == 0;
    }

    bool isNotEmptyStr(const string &s)
    {
        return !isEmptyStr(s);
    }
}

#define IMPL_COMP_OPS(Type)                                             \
    bool operator!=(const Type &a, const Type &b) { return !(a == b); } \
    bool operator<=(const Type &a, const Type &b) { return !(b < a); }  \
    bool operator>(const Type &a, const Type &b) { return (b < a); }    \
    bool operator>=(const Type &a, const Type &b) { return !(a < b); }

#endif
