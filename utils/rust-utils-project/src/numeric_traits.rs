use std::{
    fmt::{Debug, Display},
    iter::Sum,
    ops::{Add, AddAssign, Div, DivAssign, Mul, MulAssign, Neg, Rem, RemAssign, Sub, SubAssign},
};

pub trait Sqrtable<Out> {
    fn sqrt(self) -> Out;
}

pub trait NumericBase:
    Default
    + Copy
    + Debug
    + Display
    + Sized
    + PartialEq
    + PartialOrd
    + Add<Output = Self>
    + AddAssign
    + Sub<Output = Self>
    + SubAssign
    + Mul<Output = Self>
    + MulAssign
    + Div<Output = Self>
    + DivAssign
    + Rem<Output = Self>
    + RemAssign
    + Sqrtable<f32>
    + Sqrtable<f64>
    + Sum
{
}

pub trait UnsignedNumeric: NumericBase {}

pub trait SignedNumeric: NumericBase + Neg<Output = Self> {
    fn abs(&self) -> Self;
}

macro_rules! impl_empty_trait {
    ($trait:ident for $($t:ty),*) => ($(
        impl $trait for $t {
        }
    )*)
}

macro_rules! impl_signed_numeric {
    ($($t:ty),*) => ($(
        impl SignedNumeric for $t {
            fn abs(&self) -> Self
            {
                return <$t>::abs(*self);
            }
        }
    )*)
}

macro_rules! impl_sqrtable_trait {
    ($($t:ty),*) => ($(
        impl  Sqrtable<f32> for $t {
            fn sqrt(self) -> f32 {
                (self as f32).sqrt()
            }
        }
        impl  Sqrtable<f64> for $t {
            fn sqrt(self) -> f64 {
                (self as f64).sqrt()
            }
        }
    )*)
}

impl_sqrtable_trait!(i8, i16, i32, i64, i128, isize);
impl_sqrtable_trait!(u8, u16, u32, u64, u128, usize);
impl_sqrtable_trait!(f32, f64);

impl_empty_trait!(NumericBase for i8, i16, i32, i64, i128, isize);
impl_empty_trait!(NumericBase for u8, u16, u32, u64, u128, usize);
impl_empty_trait!(NumericBase for f32, f64);

impl_empty_trait!(UnsignedNumeric for i8, i16, i32, i64, i128, isize);
impl_empty_trait!(UnsignedNumeric for u8, u16, u32, u64, u128, usize);
impl_empty_trait!(UnsignedNumeric for f32, f64);

impl_signed_numeric!(i8, i16, i32, i64, i128, isize);
impl_signed_numeric!(f32, f64);
