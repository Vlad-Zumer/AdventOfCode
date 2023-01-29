#![allow(non_snake_case)]

use std::{fs::File, io::{BufReader, BufRead}};

#[derive(Debug,Clone)]
enum Command
{
    NoOp,
    AddX(i64),
    Begin,
}

#[derive(Debug,Clone)]
struct CpuState
{
    x: i64,
    nextCycle: u32,
    command: Command
}

type ParsedType = Vec<CpuState>;
fn parseInput(filePath : &str) -> Option<ParsedType>
{
   let f = File::open(filePath).ok()?;
   let lines : Vec<_> = BufReader::new(f).lines().collect();

    if lines.iter().any(|l| l.is_err())
    {
        return None;
    }

    let mut retVal = Vec::new();
    let mut state = CpuState { x: 1, nextCycle: 1, command: Command::Begin };
    retVal.push(state.clone());

    for line in lines.into_iter().map(|lr| lr.unwrap())
    {
        if &line == "noop"
        {
            state.command = Command::NoOp;
            state.nextCycle += 1;
            retVal.push(state.clone());
        }
        else if line.starts_with("addx")
        {
            let valStr = line.trim_start_matches("addx ");
            let val = valStr.parse().unwrap();
            state.command = Command::AddX(val);
            state.nextCycle += 2;
            state.x += val;
            retVal.push(state.clone());
        }
        else
        {
            return None;    
        }
    }

    return Some(retVal);
}

fn part1(states: &Vec<CpuState>) -> i64
{
    let cycles = vec![20,60,100,140,180,220];

    let foundStates = cycles.into_iter().map(|cycle|
        {
            let state = states.iter().skip_while(move |state| state.nextCycle <= cycle)
                .next()
                .expect(&format!("Could not find state for cycle: {}", cycle));

            return (cycle, state);
        });

    return foundStates.map(|(cycle, state)| 
        {
           let x = match state.command {
                Command::NoOp => state.x,
                Command::Begin => state.x,
                Command::AddX(val) => state.x - val,
            };
            return i64::from(cycle) * x;
        }).sum();
}

fn part2(states: &Vec<CpuState>) -> String
{
    let mut pixelsAsLine = String::new();

    for [prev, curr] in states.windows(2).flat_map(<&[_; 2]>::try_from)
    {
        let cycles = prev.nextCycle..curr.nextCycle;
        let pixelPos= cycles.map(|c| (c - 1) % 40);
        let spritePos = [prev.x-1, prev.x, prev.x+1].map(|i| i as u32);
        let pixels: String = pixelPos.map(|pi| if spritePos.contains(&pi) {'\u{2588}'} else {' '}).collect();

        pixelsAsLine.push_str(&pixels);
    }

    let ret = pixelsAsLine.chars().enumerate().fold(String::new(), |mut acc, (indx, ch)|
    {
        if indx != 0 && indx % 40 == 0 {
            acc.push('\n');
        }
        acc.push(ch);

        return acc;
    });

    return ret;
}

fn main() {
    let Some(states) = parseInput("input.txt") else
    {
        return;
    };

    println!("part 1: {}", part1(&states));
    println!("part 2:\n{}", part2(&states));
}