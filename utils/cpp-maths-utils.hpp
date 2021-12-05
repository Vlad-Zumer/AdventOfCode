#ifndef CPP_VZ_MATHS_UTILS
#define CPP_VZ_MATHS_UTILS

#include <iostream>
#include <type_traits>
#include <vector>
#include <math.h>

namespace MathsUtil
{
    using namespace std;

    // N dim vector class
    template<class T, size_t size>
    class VecN
    {
    protected:

        static_assert(is_arithmetic<T>::value,"Cannot make vector of non-arithmetic type");
        T arr[size];

    public:

        size_t numCoord = size;
        
        T& operator[](size_t index) { return arr[index];}
        
        VecN<T,size> operator+(const VecN<T, size> &other) const
        {
            VecN<T,size> newV;
            for (size_t i = 0; i < size; i++)
            {
                newV.arr[i] = (arr[i] + other.arr[i]);
            }
            return newV;
        }

        VecN<T,size> operator-(const VecN<T, size> &other) const
        {
            VecN<T,size> newV;
            for (size_t i = 0; i < size; i++)
            {
                newV.arr[i] = (arr[i] - other.arr[i]);
            }
            return newV;
        } 
        
        VecN<T,size> operator* (const T mul) const
        {
            VecN<T,size> newV;
            for (size_t i = 0; i < size; i++)
            {
                newV.arr[i] = (arr[i] * mul);
            }
            return newV;
        }
        
        VecN<T,size> operator/ (const T div) const 
        {
            VecN<T,size> newV;
            for (size_t i = 0; i < size; i++)
            {
                newV.arr[i] = (arr[i]/div);
            }
            return newV;
        }

        bool operator==(const VecN<T,size> &other) const
        {
            bool equal = true;
            for (size_t i = 0; i < size; i++)
            {
                equal &= (arr[i] == other.arr[i]);
            }
            return equal;
        }

        bool operator!=(const VecN<T,size> &other) const
        {
            return !((*this) == other);
        }
        
        friend ostream& operator<< (ostream& os, const VecN<T,size> vec)
        {
            os<<"Vec:"<<" { ";
            for (size_t i = 0; i < size; i++)
            {
                if (i + 1 == size)
                {
                    os<<vec.arr[i];
                }
                else
                {
                    os<<vec.arr[i]<<", ";
                }
            }
            os<<" }"<<endl;

            return os;
        }
        
        VecN()
        {
            for (size_t i = 0; i < size; i++)
            {
                arr[i] = (T)0;
            }
        }

        T get(const size_t index) const {return arr[index];}

        vector<T> toStdVector()
        {
            vector<T> ret(size);
            for (size_t i = 0; i < size; i++)
            {
                ret[i] = arr[i];
            }
            return ret;
        }

        template<class D, typename std::enable_if<std::is_arithmetic<D>::value>::type* = nullptr>
        D lengthSq()
        {
            D len = 0;
            for (size_t i = 0; i < size; i++)
            {
                len += arr[i] * arr[i];
            }
            return len;
        }
        
        float lengthf()
        {
            return sqrtf(lengthSq<float>());
        }

        double length()
        {
            return sqrt(lengthSq<double>());
        }

    };

    template<class Old, class New, size_t size>
    VecN<New,size> VecNConvert (VecN<Old, size> vec)
    {
        VecN<New,size> ret;
        for (size_t i = 0; i < size; i++)
        {
            ret[i] = (New)vec[i];
        }
        return ret;
    }


    using Vec2 = VecN<int,2>;
    using Vec2l = VecN<long,2>;
    using Vec2ll = VecN<long long,2>;
    using Vec2f = VecN<float,2>;
    
    using Vec3 = VecN<int,3>;
    using Vec3l = VecN<long,3>;
    using Vec3ll = VecN<long long,3>;
    using Vec3f = VecN<float,3>;
}

#endif