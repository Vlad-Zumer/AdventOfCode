#![allow(non_snake_case)]

use std::{fs::File, io::{BufReader, BufRead}, collections::HashSet, hash::Hash};


trait Dups<T> where T : Eq + Hash
{
    fn hasDuplicates(self) -> bool;
    fn getDuplicates(self) -> Vec<T>;
}

impl<T: Eq + Hash, const N: usize> Dups<T> for [T; N]
{
    fn hasDuplicates(self) -> bool {
        let mut set = HashSet::new();
        for el in self
        {
            if set.contains(&el)
            {
                return  true;
            }
            set.insert(el);
        }

        return  false;
    }

    fn getDuplicates(self) -> Vec<T> {
        let mut vec = Vec::new();
        let mut set = HashSet::new();
        for el in self
        {
            if set.contains(&el)
            {
                vec.push(el);
                continue;
            }
            set.insert(el);
        }

        return  vec;
    }
}

fn parseInput(filePath : &str) -> Option<String>
{
   let f = File::open(filePath).ok()?;
   let lines : Vec<_> = BufReader::new(f).lines().collect();

    if lines.iter().any(|l| l.is_err())
    {
        return None;
    }

    let mut retVal = String::new();

    for line in lines.into_iter().map(|lr| lr.unwrap())
    {
        retVal += &line;
    }

    return Some(retVal);
}

fn solve<const N: usize>(signal: &str) -> usize
{
    let mut index: usize = 0;
    let mut seen : [char; N] = [' '; N];

    for ch in signal.chars()
    {
        index += 1;
        if seen.iter().any(|ch| ch == &' ')
        {
            seen.rotate_left(1);
            seen[N-1] = ch;
            continue;
        }

        seen.rotate_left(1);
        seen[N-1] = ch;

        if !seen.clone().hasDuplicates()
        {
            return index;
        }
    }
    return  0;
}

fn main() {

    let Some(signal) = parseInput("input.txt") else
    {
        return;
    };

    println!("{}", solve::<4>(&signal));
    println!("{}", solve::<14>(&signal));

}