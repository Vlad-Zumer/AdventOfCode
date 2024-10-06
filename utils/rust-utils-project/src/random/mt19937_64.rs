use super::RandomNumberGenerator;

pub type Word = u64;

/// Word size
const WORD_SIZE: u32 = 64;

/// State size
const STATE_SIZE: usize = 312 ;

/// Middle word (M)
const MIDDLE_WORD_OFFSET: usize = 156;

/// Lower mask bits (R)
const LOWER_MASK_BITS: u32 = 31;

/// Upper mask
const UPPER_MASK: Word = (0xffffffff_ffffffff as Word).wrapping_shl(LOWER_MASK_BITS);

const LOWER_MASK: Word = (0xffffffff_ffffffff as Word).wrapping_shr(WORD_SIZE - LOWER_MASK_BITS);

///
const A: Word = 0xB5026F5A_A96619E9;

/// Tempering bit masks
const B: Word = 0x71D67FFF_EDA60000;
const C: Word = 0xFFF7EEE0_00000000;

/// Tempering bit shifts
const S: u32 = 17;
const T: u32 = 37;

/// Additional tempering shifts
const U: u32 = 29;
const L: u32 = 43;

/// Additional tempering mask
const D: Word = 0x5555555555555555;

/// INITIALIZATION CONSTANT
const F: Word = 6364136223846793005;

#[derive(Debug)]
struct MersenneState {
    arr: [Word; STATE_SIZE],
    index: usize,
}

impl Default for MersenneState {
    fn default() -> Self {
        Self {
            arr: [(); STATE_SIZE].map(|_| Word::default()),
            index: Default::default(),
        }
    }
}

impl MersenneState {
    fn set_seed(&mut self, seed: Word) {
        let expand_seed = |seed: Word, i: Word| -> Word {
            return (F.wrapping_mul(seed ^ (seed.wrapping_shr(WORD_SIZE - 2)))).wrapping_add(i);
        };

        let mut new_seed = expand_seed(seed, 1);

        for i in 0..self.arr.len() {
            self.arr[i] = new_seed;
            new_seed = expand_seed(new_seed, (i + 1).try_into().unwrap());
        }

        self.twist_numbers();
    }

    fn twist_numbers(&mut self) {
        let arr_copy: [Word; STATE_SIZE] = self.arr;
        for i in 0..self.arr.len() {
            let x = (arr_copy[i] & UPPER_MASK) + (arr_copy[(i + 1) % STATE_SIZE] & LOWER_MASK);
            let mut xa = x.wrapping_shr(1);
            if xa % 2 != 0 {
                xa = xa ^ A;
            }
            self.arr[i] = arr_copy[(i + MIDDLE_WORD_OFFSET) % STATE_SIZE] ^ xa;
        }
    }

    fn reset(&mut self) {
        for i in 0..self.arr.len() {
            self.arr[i] = Word::default();
        }
        self.index = 0;
    }

    fn get_next_value(&mut self) -> Word {
        if self.index == STATE_SIZE {
            self.index = 0;
            self.twist_numbers();
        }
        let out_val = self.arr[self.index];
        self.index += 1;
        return out_val;
    }
}

#[cfg_attr(debug_assertions, derive(Debug))]
pub struct MersenneTwister64 {
    state: MersenneState,
}

impl MersenneTwister64 {
    fn temper_value(val: Word) -> Word {
        let mut y = val ^ (val.wrapping_shr(U) & D);
        y = y ^ (y.wrapping_shl(S) & B);
        y = y ^ (y.wrapping_shl(T) & C);
        let z = y ^ y.wrapping_shr(L);
        return z;
    }
}

impl RandomNumberGenerator for MersenneTwister64 {
    type Seed = Word;

    type Value = Word;

    fn new(seed: Self::Seed) -> Self {
        let mut val = Self {
            state: Default::default(),
        };
        val.reset_seed(seed);
        return val;
    }

    fn reset_seed(&mut self, seed: Self::Seed) {
        self.state.reset();
        self.state.set_seed(seed);
    }

    fn next_value(&mut self) -> Self::Value {
        let state_value = self.state.get_next_value();
        return Self::temper_value(state_value);
    }

    fn next_f32(&mut self) -> f32 {
        let val = self.next_value();
        return (val as f32) / (Self::Value::MAX as f32);
    }

    fn next_f64(&mut self) -> f64 {
        let val = self.next_value();
        return (val as f64) / (Self::Value::MAX as f64);
    }
}