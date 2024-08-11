#![allow(unused_imports)]
#![allow(dead_code)]


pub mod mt19937_32;
pub mod mt19937_64;

pub use mt19937_32::MersenneTwister32;
pub use mt19937_64::MersenneTwister64;

/// Actual interface/API for a random number generator
pub trait RandomNumberGenerator {
    type Seed;
    type Value;

    fn new(seed: Self::Seed) -> Self;

    fn reset_seed(&mut self, seed: Self::Seed);

    fn next_value(&mut self) -> Self::Value;

    fn next_f32(&mut self) -> f32;

    fn next_f64(&mut self) -> f64;
}