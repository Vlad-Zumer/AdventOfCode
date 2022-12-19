#![allow(non_snake_case)]

use std::{fs::File, io::BufReader, io::BufRead, collections::HashSet};

#[derive(Debug)]
struct Backpack
{
    comp1 : String,
    comp2 : String,
}

impl Backpack
{
    fn new(comp1: String, comp2: String) -> Self
    {
        return Backpack
        {
            comp1,
            comp2
        };
    }

    fn calcPriority(ch : char) -> i32
    {
        assert!(('A' <= ch && 'Z' >= ch) || ('a' <= ch && 'z' >= ch), "invalid char {}", ch);

        if 'A' <= ch && 'Z' >= ch
        {
            return 27 + (ch as i32) - ('A' as i32);
        }
        else if 'a' <= ch && 'z' >= ch
        {
            return  1 + (ch as i32) - ('a' as i32);
        }

        return  0;
    }

    fn findCommon(&self) -> Vec<char>
    {
        let mut retV : Vec<char> = self.comp1.chars().filter(|ch| self.comp2.contains(*ch)).collect();
        retV.dedup();
        return  retV;
    }

    fn getInventory(&self) -> HashSet<char>
    {
        let fullInv = self.comp1.clone() + &self.comp2;
        return fullInv.chars().collect();
    } 
}

fn parseInput(filePath : &str) -> Option<Vec<Backpack>>
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
        let len = line.len();
        assert!(len%2==0, "len is odd");
        let s1 = &line[..len/2];
        let s2 = &line[len/2..];

        retVal.push(Backpack::new(s1.to_owned(), s2.to_owned()));
    }

    return Some(retVal);
}

fn part1(backpacks: &Vec<Backpack>) -> i32
{
    return backpacks.iter().map(|b| b.findCommon().iter().map(|ch| Backpack::calcPriority(*ch)).sum::<i32>()).sum();
}


fn part2(backpacks: &Vec<Backpack>) -> i32
{
    assert!(backpacks.len()%3 == 0, "Too few backpacks for part 2");
    let groups = backpacks.chunks(3);
    let badges : Vec<_> = groups.map(|g| g.iter().map(|b| b.getInventory())
                                                              .reduce(|acc, inv| acc.intersection(&inv).map(|ch| *ch).collect()))
                                .collect();

    if badges.iter().any(|b| b.is_none())
    {
        assert!(false, "Something went terribly wrong");
    }

    return  badges.iter().map(|b| b.as_ref().unwrap().iter().map(|ch| Backpack::calcPriority(*ch)).sum::<i32>()).sum();
}

fn main() {
    let Some(backpacks) = parseInput("input.txt") else
    {
        return;
    };

    println!("{}", part1(&backpacks));
    println!("{}", part2(&backpacks));
}
