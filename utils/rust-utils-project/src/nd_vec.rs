#![allow(dead_code)]

#[path = "numeric_traits.rs"]
pub mod numeric_traits;

use std::{
    cmp::min,
    fmt::Display,
    ops::{Add, AddAssign, Div, DivAssign, Index, IndexMut, Mul, MulAssign, Neg, Sub, SubAssign},
};

use self::numeric_traits::{NumericBase, SignedNumeric};

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct NDVec<T, const N: usize>
where
    T: NumericBase,
{
    arr: [T; N],
}

impl<T: NumericBase, const N: usize> NDVec<T, N> {
    pub fn zero() -> Self {
        NDVec {
            arr: [T::default(); N],
        }
    }

    pub fn new(arr: [T; N]) -> Self {
        return NDVec { arr };
    }

    pub fn to_dimensions<const ND: usize>(&self) -> NDVec<T, ND> {
        let mut ret_val = NDVec::<T, ND>::zero();
        for index in 0..(min(N, ND)) {
            ret_val[index] = self[index].clone();
        }
        return ret_val;
    }

    pub fn map<F, T2>(&self, f: F) -> NDVec<T2, N>
    where
        T2: NumericBase,
        F: FnMut(&T) -> T2,
    {
        let ret_arr = self
            .arr
            .iter()
            .map(f)
            .collect::<Vec<_>>()
            .try_into()
            .expect("Something went wrong");

        return NDVec::new(ret_arr);
    }

    pub fn fold<F, T2>(&self, init: T2, mut f: F) -> T2
    where
        F: FnMut(T2, &T) -> T2,
    {
        return self.arr.iter().fold(init, |acc, it| f(acc, it));
    }

    pub fn len_squared(&self) -> T {
        self.arr.iter().map(|x| ((*x) * (*x))).sum()
    }

    pub fn lenf32(&self) -> f32 {
        self.len_squared().sqrt()
    }

    pub fn lenf64(&self) -> f64 {
        self.len_squared().sqrt()
    }
}

impl<T: NumericBase, const N: usize> Index<usize> for NDVec<T, N> {
    type Output = T;

    fn index(&self, index: usize) -> &Self::Output {
        return &self.arr[index];
    }
}

impl<T: NumericBase, const N: usize> IndexMut<usize> for NDVec<T, N> {
    fn index_mut(&mut self, index: usize) -> &mut Self::Output {
        return &mut self.arr[index];
    }
}

impl<T: NumericBase, const N: usize> Add for NDVec<T, N> {
    type Output = NDVec<T, N>;

    fn add(self, rhs: Self) -> Self::Output {
        let it1 = (&self).arr.iter();
        let it2 = (&rhs).arr.iter();
        let sum_arr: [T; N] = it1
            .zip(it2)
            .map(|(a, b)| *a + *b)
            .collect::<Vec<_>>()
            .try_into()
            .expect("Error adding 2 vectors of different lengths");
        return NDVec { arr: sum_arr };
    }
}

impl<T: SignedNumeric, const N: usize> Sub for NDVec<T, N> {
    type Output = NDVec<T, N>;

    fn sub(self, rhs: Self) -> Self::Output {
        let it1 = (&self).arr.iter();
        let it2 = (&rhs).arr.iter();
        let dif_arr: [T; N] = it1
            .zip(it2)
            .map(|(a, b)| *a - *b)
            .collect::<Vec<_>>()
            .try_into()
            .expect("Error adding 2 vectors of different lengths");
        return NDVec { arr: dif_arr };
    }
}

impl<T: NumericBase, const N: usize> AddAssign for NDVec<T, N> {
    fn add_assign(&mut self, rhs: Self) {
        for index in 0..N {
            self[index] += rhs[index];
        }
    }
}

impl<T: SignedNumeric, const N: usize> SubAssign for NDVec<T, N> {
    fn sub_assign(&mut self, rhs: Self) {
        for index in 0..N {
            self[index] -= rhs[index];
        }
    }
}

impl<T: SignedNumeric, const N: usize> Neg for NDVec<T, N> {
    type Output = NDVec<T, N>;

    fn neg(self) -> Self::Output {
        let neg_arr = self
            .arr
            .iter()
            .map(|x| -(*x))
            .collect::<Vec<_>>()
            .try_into()
            .unwrap();
        return NDVec { arr: neg_arr };
    }
}

impl<T: NumericBase, const N: usize> Mul<T> for NDVec<T, N> {
    type Output = NDVec<T, N>;

    fn mul(self, rhs: T) -> Self::Output {
        let mul_arr = self
            .arr
            .iter()
            .map(|x| (*x) * rhs)
            .collect::<Vec<_>>()
            .try_into()
            .unwrap();

        return NDVec { arr: mul_arr };
    }
}

impl<T: NumericBase, const N: usize> Div<T> for NDVec<T, N> {
    type Output = NDVec<T, N>;

    fn div(self, rhs: T) -> Self::Output {
        let mul_arr = self
            .arr
            .iter()
            .map(|x| (*x) / rhs)
            .collect::<Vec<_>>()
            .try_into()
            .unwrap();

        return NDVec { arr: mul_arr };
    }
}

impl<T: NumericBase, const N: usize> MulAssign<T> for NDVec<T, N> {
    fn mul_assign(&mut self, rhs: T) {
        for index in 0..N {
            self[index] *= rhs;
        }
    }
}

impl<T: NumericBase, const N: usize> DivAssign<T> for NDVec<T, N> {
    fn div_assign(&mut self, rhs: T) {
        for index in 0..N {
            self[index] /= rhs;
        }
    }
}

impl<T: NumericBase, const N: usize> Display for NDVec<T, N> {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(
            f,
            "Vec{}:({})",
            N,
            self.arr
                .iter()
                .map(|x| x.to_string())
                .collect::<Vec<_>>()
                .join(", ")
        )
    }
}

pub type Vec2<T> = NDVec<T, 2>;
pub type Vec3<T> = NDVec<T, 3>;
pub type Vec4<T> = NDVec<T, 4>;
