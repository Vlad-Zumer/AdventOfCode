#![allow(non_snake_case)]

use std::{fs::File, io::{BufReader, BufRead}, str::FromStr};


#[derive(Debug, PartialEq, Eq, Clone)]
enum Move {
    Rock,
    Paper,
    Scissors,
}

impl Move
{
    fn score(&self) -> i32
    {
        return match &self {
            Move::Rock => 1,
            Move::Paper => 2,
            Move::Scissors => 3,
        }
    }

    fn getMoveForMyResult(&self, res: MatchResult) -> Move
    {
        let moveToWin = match self {
            Move::Rock => Move::Scissors,
            Move::Paper => Move::Rock,
            Move::Scissors => Move::Paper,
        };

        let moveToLose = match self {
            Move::Rock => Move::Paper,
            Move::Paper => Move::Scissors,
            Move::Scissors => Move::Rock,
        };

        return match res {
            MatchResult::Draw => self.clone(),
            MatchResult::Win => moveToWin,
            MatchResult::Loss => moveToLose,
        };
    }
}

impl FromStr for Move
{
    type Err = String;

    fn from_str(s: &str) -> Result<Self, Self::Err> 
    {
        let Some(char) = s.trim().chars().nth(0) else
        {
            return  Err(format!("No char in '{}'", s));
        };

        return match char {
            'a' | 'A' | 'x' | 'X' => Ok( Move::Rock ),
            'b' | 'B' | 'y' | 'Y' => Ok( Move::Paper ),
            'c' | 'C' | 'z' | 'Z' => Ok( Move::Scissors),
            _ => Err(format!("Char '{}' is invalid", char)),
        };
    }
}

#[derive(Debug, PartialEq, Eq)]
enum MatchResult {
    Win,
    Draw,
    Loss,
}

impl MatchResult
{
    fn part2(m : &Move) -> Self
    {
        return match m {
            Move::Rock => MatchResult::Loss,
            Move::Paper => MatchResult::Draw,
            Move::Scissors => MatchResult::Win,
        };
    }

    fn negate(&self) -> Self
    {
        return match self {
            MatchResult::Win => MatchResult::Loss,
            MatchResult::Draw => MatchResult::Draw,
            MatchResult::Loss => MatchResult::Win,
        };
    }
}

#[derive(Debug)]
struct Match
{
    me : Move,
    opponent: Move,
}

impl Match
{
    fn new(me: Move, opponent: Move) -> Self
    {
        return Match
        {
            me,
            opponent
        };
    }

    fn result(&self) -> MatchResult
    {
        if self.me == self.opponent
        {
            return  MatchResult::Draw;
        }
        else if (self.me == Move::Rock      && self.opponent == Move::Scissors) ||
                (self.me == Move::Paper     && self.opponent == Move::Rock) ||
                (self.me == Move::Scissors  && self.opponent == Move::Paper)
        {
            return  MatchResult::Win;   
        }
        else
        {
            return  MatchResult::Loss;    
        }
    }

    fn score(&self) -> i32
    {
        let matchScore = match self.result() {
            MatchResult::Win => 6,
            MatchResult::Draw => 3,
            MatchResult::Loss => 0,
        };

        return self.me.score() + matchScore;
    }

    fn convertToPart2(mut self) -> Self
    {
        self.me = self.opponent.getMoveForMyResult(MatchResult::part2(&self.me).negate());
        return  self;
    }
}

impl FromStr for Match
{
    type Err = String;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        let chars : Vec<_> = s.trim().split(' ').collect();
        
        if chars.len() != 2
        {
            return Err(format!("String '{}' is invalid match", s));
        }

        let op = chars[0].parse::<Move>()?;
        let me = chars[1].parse::<Move>()?;

        return Ok(Match::new(me, op));
    }
}

fn readInput(filePath : &str) -> Option<Vec<Match>>
{
    let f = File::open(filePath).ok()?;
    let reader = BufReader::new(f);
    let lines : Vec<_> = reader.lines().collect();

    
    if lines.iter().any(|line| line.is_err() )
    {
        return None;
    }
    
    let mut retVal = Vec::new();
    
    for line in lines.into_iter().map(|line| line.unwrap())
    {
        retVal.push(line.parse::<Match>().unwrap());
    }

    return Some(retVal);
}

fn sol1(allMatches : &Vec<Match>) -> i32
{
    return allMatches.iter().map(|m| m.score()).sum();
}

fn main() {
    let Some(mut allMatches) = readInput("input.txt") else
    {
        return;
    };

    println!("{}", sol1(&allMatches));
    
    allMatches = allMatches.into_iter().map(|m| m.convertToPart2()).collect();
    println!("{}", sol1(&allMatches));

}
