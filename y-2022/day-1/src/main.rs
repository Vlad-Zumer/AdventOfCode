use std::fs::File;
use std::io::{prelude::*, BufReader};
use std::vec::Vec;

#[derive(Debug)]
struct Elf
{
    pub items : Vec<u64>
}

impl Elf
{
    fn total(&self) -> u64 
    {
        return self.items.iter().sum();
    }

    fn new() -> Self
    {
        return  Elf
        {
            items: Vec::new(),
        };
    }
}

fn readInput(filePath : &str) -> Option<Vec<Elf>>
{
    let f = File::open(filePath).ok()?;
    let reader = BufReader::new(f);
    let lines : Vec<_> = reader.lines().collect();

    
    if lines.iter().any(|line| line.is_err() )
    {
        return None;
    }
    
    let mut retVal = Vec::new();
    
    let mut index = 0;
    retVal.push(Elf::new());
    
    for line in lines.into_iter().map(|line| line.unwrap())
    {
        if line.is_empty()
        {
            index += 1;
            retVal.push(Elf::new())
        }
        else
        {
            let elf = retVal.get_mut(index)?;
            let number = line.parse().ok()?;
            elf.items.push(number);
        }
    }

    return Some(retVal);
}

fn solve1(elfs : &Vec<Elf>) -> u64
{
    return  elfs.iter().max_by_key(|elf| elf.total() ).unwrap().total();
}

fn solve2(elfs: &Vec<Elf>) -> u64
{
    const N : usize = 3;
    let mut selectedElfs : [Option<&Elf>; N] = [None; N];

    for elf in elfs.iter() 
    {
        for index in (0..N) {
            if selectedElfs[index].is_none()
            {
                selectedElfs[index] = Some(elf);

                if !selectedElfs.iter().any(|of| of.is_none())
                {
                    selectedElfs.sort_by_cached_key(|op| op.unwrap().total());
                }

                break;
            }
            else if elf.total() > selectedElfs[index].unwrap().total()
            {
                for rindex in (index+1..N).rev()
                {
                    selectedElfs[rindex] = selectedElfs[rindex-1];
                }
                selectedElfs[index] = Some(elf);
                break;
            }
        }
    }
    return  selectedElfs.iter().map(|of| of.unwrap().total()).sum();
}

fn main() {
    let Some(mut elfs) = readInput("input.txt") else
    {
        return;
    };
    println!("{}", solve1(&elfs));
    println!("{}", solve2(&elfs));
}
