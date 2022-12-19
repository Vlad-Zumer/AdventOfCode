#![allow(non_snake_case)]

use std::{fs::File, io::{BufReader, BufRead}, str::FromStr, cmp::{max, min}};

#[derive(Debug, PartialEq, Eq)]
enum Range
{
    EmptyRange,
    InclRange { start: i32, end: i32 },
}

impl Range
{
    
    fn newIncl(start: i32, end: i32) -> Self
    {
        assert!(start <= end, "Start > End");
        return Range::InclRange
        {
            start,
            end
        };
    }

    fn isEmpty(&self) -> bool
    {
        return  self == &Range::EmptyRange;
    }

    fn getIntersection(&self, other: &Range) -> Range
    {
        let Range::InclRange { start, end } = self else
        {
            return Range::EmptyRange;
        };
        let myStart = start;
        let myEnd = end;
        
        let Range::InclRange { start, end } = other else
        {
            return  Range::EmptyRange;
        };
        
        let intersectionStart = max(myStart, start).clone();
        let intersectionEnd = min(myEnd, end).clone();

        if intersectionStart > intersectionEnd
        {
            return Range::EmptyRange;
        }

        return Range::newIncl(intersectionStart, intersectionEnd);
    }

    fn fullyContains(&self, other: &Range) -> bool
    {
        let Range::InclRange { start, end } = self else
        {
            return false;
        };
        let myStart = start;
        let myEnd = end;
        
        let Range::InclRange { start, end } = other else
        {
            return  true;
        };

        return myStart <= start && myEnd >= end;
    }
}

impl FromStr for Range
{
    type Err = String;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        let vec : Vec<_> = s.split('-').map(|s| s.trim()).collect();
        assert!(vec.len() == 2, "Range: Bad format string '{}'", s);

        let Ok(start) = vec[0].parse::<i32>() else
        {
            panic!("Could not parse '{}' to i32", vec[0]);
        };

        let Ok(end) = vec[1].parse::<i32>() else
        {
            panic!("Could not parse '{}' to i32", vec[1]);
        };

        return Ok(Range::newIncl(start, end));
    }
}

#[derive(Debug)]
struct ElfPair
{
    elf1: Range,
    elf2: Range
}

impl ElfPair
{
    fn new(elf1: Range, elf2: Range) -> Self
    {
        return  ElfPair
        {
            elf1,
            elf2
        };
    }

    fn workFullyOverlaps(&self) -> bool
    {
        return self.elf1.fullyContains(&self.elf2) || self.elf2.fullyContains(&self.elf1);
    }

    fn workOverlapsAtAll(&self) -> bool
    {
        return !self.elf1.getIntersection(&self.elf2).isEmpty();
    }
}

impl FromStr for ElfPair
{
    type Err=String;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        let vec : Vec<_> = s.split(',').map(|s| s.trim()).collect();
        assert!(vec.len() == 2, "ElfPair: Bad format string '{}'", s);

        let Ok(e1) = vec[0].parse::<Range>() else
        {
            panic!("Could not parse '{}' to Range", vec[0]);
        };

        let Ok(e2) = vec[1].parse::<Range>() else
        {
            panic!("Could not parse '{}' to Range", vec[1]);
        };

        return Ok(ElfPair::new(e1, e2))
    }
}

fn parseInput(filePath : &str) -> Option<Vec<ElfPair>>
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
        retVal.push(line.parse().unwrap());
    }

    return Some(retVal);
}

fn part1 (elfPairs : &Vec<ElfPair>) -> i32
{
    return elfPairs.iter().filter(|p| p.workFullyOverlaps()).count() as i32;
}

fn part2 (elfPairs : &Vec<ElfPair>) -> i32
{
    return elfPairs.iter().filter(|p| p.workOverlapsAtAll()).count() as i32;
}

fn main() {
    let Some(elfPairs) = parseInput("input.txt") else
    {
        return;
    };

    println!("{}", part1(&elfPairs));
    println!("{}", part2(&elfPairs));

}