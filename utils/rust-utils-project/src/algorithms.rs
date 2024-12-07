#![allow(dead_code)]

#[path = "numeric_traits.rs"]
pub mod numeric_traits;

use self::numeric_traits::NumericBase;

pub fn gcd<T>(a: T, b: T) -> T
where
    T: NumericBase,
{
    let mut a1 = a.clone();
    let mut b1 = b.clone();

    while b1 != T::default() {
        let t = b1;
        b1 = a1 % b1;
        a1 = t;
    }
    return a1;
}
