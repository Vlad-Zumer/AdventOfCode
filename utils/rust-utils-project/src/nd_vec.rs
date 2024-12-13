#![allow(dead_code)]

#[path = "numeric_traits.rs"]
pub mod numeric_traits;

use std::{
    cmp::min,
    fmt::Display,
    ops::{Add, AddAssign, Div, DivAssign, Index, IndexMut, Mul, MulAssign, Neg, Sub, SubAssign},
};

use self::numeric_traits::{NumericBase, SignedNumeric};

#[derive(Debug, Clone, PartialEq, Eq, Hash)]
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

    pub fn dimensions() -> usize {
        return N;
    }

    pub fn size(&self) -> usize {
        return N;
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

    pub fn as_vec(&self) -> Vec<T> {
        return Vec::from(self.arr);
    }

    fn private_add(&self, rhs: &Self) -> Self {
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

    fn private_mul(&self, rhs: T) -> Self {
        let mul_arr = self
            .arr
            .iter()
            .map(|x| (*x) * rhs)
            .collect::<Vec<_>>()
            .try_into()
            .unwrap();

        return NDVec { arr: mul_arr };
    }

    fn private_div(&self, rhs: T) -> Self {
        let mul_arr = self
            .arr
            .iter()
            .map(|x| (*x) / rhs)
            .collect::<Vec<_>>()
            .try_into()
            .unwrap();

        return NDVec { arr: mul_arr };
    }

    fn private_add_assign(&mut self, rhs: &Self) {
        for index in 0..N {
            self[index] += rhs[index];
        }
    }

    fn private_mul_assign(&mut self, rhs: T) {
        for index in 0..N {
            self[index] *= rhs;
        }
    }

    fn private_div_assign(&mut self, rhs: T) {
        for index in 0..N {
            self[index] /= rhs;
        }
    }
}

impl<T: SignedNumeric, const N: usize> NDVec<T, N> {
    fn private_sub(&self, rhs: &Self) -> Self {
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

    fn private_neg(&self) -> Self {
        let neg_arr = self
            .arr
            .iter()
            .map(|x| -(*x))
            .collect::<Vec<_>>()
            .try_into()
            .unwrap();
        return NDVec { arr: neg_arr };
    }

    fn private_sub_assign(&mut self, rhs: &Self) {
        for index in 0..N {
            self[index] -= rhs[index];
        }
    }
}

////////////////////////////////////////////////
// Index
////////////////////////////////////////////////
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

////////////////////////////////////////////////
// Add
////////////////////////////////////////////////
impl<T: NumericBase, const N: usize> Add for NDVec<T, N> {
    type Output = NDVec<T, N>;

    fn add(self, rhs: Self) -> Self::Output {
        return self.private_add(&rhs);
    }
}

impl<T: NumericBase, const N: usize> Add<&NDVec<T, N>> for NDVec<T, N> {
    type Output = NDVec<T, N>;

    fn add(self, rhs: &Self) -> Self::Output {
        return self.private_add(rhs);
    }
}

impl<T: NumericBase, const N: usize> Add<NDVec<T, N>> for &NDVec<T, N> {
    type Output = NDVec<T, N>;

    fn add(self, rhs: NDVec<T, N>) -> Self::Output {
        return self.private_add(&rhs);
    }
}

impl<T: NumericBase, const N: usize> Add for &NDVec<T, N> {
    type Output = NDVec<T, N>;

    fn add(self, rhs: Self) -> Self::Output {
        return self.private_add(rhs);
    }
}

////////////////////////////////////////////////
// Sub
////////////////////////////////////////////////
impl<T: SignedNumeric, const N: usize> Sub for NDVec<T, N> {
    type Output = NDVec<T, N>;

    fn sub(self, rhs: Self) -> Self::Output {
        return self.private_sub(&rhs);
    }
}

impl<T: SignedNumeric, const N: usize> Sub<&NDVec<T, N>> for NDVec<T, N> {
    type Output = NDVec<T, N>;

    fn sub(self, rhs: &Self) -> Self::Output {
        return self.private_sub(rhs);
    }
}

impl<T: SignedNumeric, const N: usize> Sub<NDVec<T, N>> for &NDVec<T, N> {
    type Output = NDVec<T, N>;

    fn sub(self, rhs: NDVec<T, N>) -> Self::Output {
        return self.private_sub(&rhs);
    }
}

impl<T: SignedNumeric, const N: usize> Sub for &NDVec<T, N> {
    type Output = NDVec<T, N>;

    fn sub(self, rhs: Self) -> Self::Output {
        return self.private_sub(rhs);
    }
}

////////////////////////////////////////////////
// AddAssign
////////////////////////////////////////////////
impl<T: NumericBase, const N: usize> AddAssign for NDVec<T, N> {
    fn add_assign(&mut self, rhs: Self) {
        self.private_add_assign(&rhs);
    }
}

impl<T: NumericBase, const N: usize> AddAssign<&NDVec<T, N>> for NDVec<T, N> {
    fn add_assign(&mut self, rhs: &Self) {
        self.private_add_assign(rhs);
    }
}

impl<T: NumericBase, const N: usize> AddAssign<NDVec<T, N>> for &mut NDVec<T, N> {
    fn add_assign(&mut self, rhs: NDVec<T, N>) {
        self.private_add_assign(&rhs);
    }
}

impl<T: NumericBase, const N: usize> AddAssign<&NDVec<T, N>> for &mut NDVec<T, N> {
    fn add_assign(&mut self, rhs: &NDVec<T, N>) {
        self.private_add_assign(rhs);
    }
}

////////////////////////////////////////////////
// subAssign
////////////////////////////////////////////////
impl<T: SignedNumeric, const N: usize> SubAssign for NDVec<T, N> {
    fn sub_assign(&mut self, rhs: Self) {
        self.private_sub_assign(&rhs);
    }
}

impl<T: SignedNumeric, const N: usize> SubAssign<&NDVec<T, N>> for NDVec<T, N> {
    fn sub_assign(&mut self, rhs: &Self) {
        self.private_sub_assign(&rhs);
    }
}

impl<T: SignedNumeric, const N: usize> SubAssign<NDVec<T, N>> for &mut NDVec<T, N> {
    fn sub_assign(&mut self, rhs: NDVec<T, N>) {
        self.private_sub_assign(&rhs);
    }
}

impl<T: SignedNumeric, const N: usize> SubAssign<&NDVec<T, N>> for &mut NDVec<T, N> {
    fn sub_assign(&mut self, rhs: &NDVec<T, N>) {
        self.private_sub_assign(&rhs);
    }
}

////////////////////////////////////////////////
// Neg
////////////////////////////////////////////////
impl<T: SignedNumeric, const N: usize> Neg for NDVec<T, N> {
    type Output = NDVec<T, N>;

    fn neg(self) -> Self::Output {
        return self.private_neg();
    }
}

impl<T: SignedNumeric, const N: usize> Neg for &NDVec<T, N> {
    type Output = NDVec<T, N>;

    fn neg(self) -> Self::Output {
        return self.private_neg();
    }
}

////////////////////////////////////////////////
// Mul
////////////////////////////////////////////////
impl<T: NumericBase, const N: usize> Mul<T> for NDVec<T, N> {
    type Output = NDVec<T, N>;

    fn mul(self, rhs: T) -> Self::Output {
        return self.private_mul(rhs);
    }
}

impl<T: NumericBase, const N: usize> Mul<T> for &NDVec<T, N> {
    type Output = NDVec<T, N>;

    fn mul(self, rhs: T) -> Self::Output {
        return self.private_mul(rhs);
    }
}

////////////////////////////////////////////////
// Div
////////////////////////////////////////////////
impl<T: NumericBase, const N: usize> Div<T> for NDVec<T, N> {
    type Output = NDVec<T, N>;

    fn div(self, rhs: T) -> Self::Output {
        return self.private_div(rhs);
    }
}

impl<T: NumericBase, const N: usize> Div<T> for &NDVec<T, N> {
    type Output = NDVec<T, N>;

    fn div(self, rhs: T) -> Self::Output {
        return self.private_div(rhs);
    }
}

////////////////////////////////////////////////
// MulAssign
////////////////////////////////////////////////
impl<T: NumericBase, const N: usize> MulAssign<T> for NDVec<T, N> {
    fn mul_assign(&mut self, rhs: T) {
        self.private_mul_assign(rhs);
    }
}

impl<T: NumericBase, const N: usize> MulAssign<T> for &mut NDVec<T, N> {
    fn mul_assign(&mut self, rhs: T) {
        self.private_mul_assign(rhs);
    }
}

////////////////////////////////////////////////
// DivAssign
////////////////////////////////////////////////
impl<T: NumericBase, const N: usize> DivAssign<T> for NDVec<T, N> {
    fn div_assign(&mut self, rhs: T) {
        self.private_div_assign(rhs);
    }
}

impl<T: NumericBase, const N: usize> DivAssign<T> for &mut NDVec<T, N> {
    fn div_assign(&mut self, rhs: T) {
        self.private_div_assign(rhs);
    }
}

////////////////////////////////////////////////
// Display
////////////////////////////////////////////////
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
