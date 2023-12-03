#![allow(non_snake_case)]

use std::{fs::File, io::{BufReader, BufRead}, ops::Add, iter::Sum};

#[derive(Debug, Clone)]
enum Number
{
    Snafu(String),
    Dec(i64)
}

impl Number
{
    fn getDec(self) -> Self
    {
        return match self {
            Number::Dec(_) => self,
            Number::Snafu(str) => 
            {
                let mut pow = 1;
                let mut val = 0;
                for ch in str.chars().rev()
                {
                    let mult: i64 = match ch {
                        '0' | '1' | '2' => ch.to_string().parse().unwrap(),
                        '-' => -1,
                        '=' => -2,
                        _ => unreachable!()
                    };
                    
                    val += mult * pow;
                    pow *= 5;
                }

                Number::Dec(val)
            },
        };
    }

    fn getSnafu(self) -> Self
    {
        return match self {
            Number::Snafu(_) => self,
            Number::Dec(int) => 
            {
                let mut base5RevNum = String::new();
                let mut base10Num = int;
                while base10Num > 0 {
                    let remainder = base10Num % 5;
                    base10Num /= 5;
                    base5RevNum.push_str(&remainder.to_string());
                }

                let mut carryOver = 0;
                let mut snafu = String::new();
                let overSums = ["=", "-", "0"];
                for digit in base5RevNum.chars().map(|ch| ch.to_digit(10).unwrap())
                {
                    let fullSum = carryOver + digit;
                    if fullSum < 3
                    {
                        carryOver = 0;
                        snafu.push_str(&fullSum.to_string());
                    }
                    else
                    {
                        carryOver = 1;
                        snafu.push_str(overSums[(fullSum - 3) as usize]);
                    }
                }
                if carryOver > 0
                {
                    snafu.push_str(&carryOver.to_string());
                }
                
                Number::Snafu(snafu.chars().rev().collect())
            },
        };
    }

    fn convert(self) -> Self
    {
        return match self {
            Number::Snafu(_) => self.getDec(),
            Number::Dec(_) => self.getSnafu(),
        };
    }
}

impl Add for Number
{
    type Output = Number;

    fn add(self, rhs: Self) -> Self::Output {
        let Number::Dec(num1) = self.getDec() else { unreachable!() };
        let Number::Dec(num2) = rhs.getDec() else { unreachable!() };

        return Number::Dec(num1 + num2);
    }
}

impl Sum for Number
{
    fn sum<I: Iterator<Item = Self>>(iter: I) -> Self {
        return iter.fold(Number::Dec(0), |acc, new| acc + new);
    }
}

type ParsedType = Vec<Number>;
fn parseInput(filePath : &str) -> Option<ParsedType>
{
   let f = File::open(filePath).ok()?;
   let lines : Vec<_> = BufReader::new(f).lines().collect();

    if lines.iter().any(|l| l.is_err())
    {
        return None;
    }

    let mut retVal = Vec::new();

    for line in lines.into_iter().map(|lr| lr.unwrap())
    {
        retVal.push(Number::Snafu(line.trim().to_string()));
    }

    return Some(retVal);
}

fn part1(nums: &Vec<Number>) -> Number
{
    return nums.iter().map(|num| num.clone()).sum::<Number>().getSnafu();
}

fn main() {
    let numbers = parseInput("input.txt").expect("Reading Failed");

    println!("part 1: {:?}", part1(&numbers));
    
}