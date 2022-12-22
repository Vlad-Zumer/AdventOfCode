#![allow(non_snake_case)]

use std::{fs::File, io::{BufReader, BufRead}};

#[derive(Debug, Clone)]
struct CargoCrate
{
    crateName: String
}

impl CargoCrate {
    fn new(crateName: String) -> Self { Self { crateName } }
}

type CargoStack = Vec<CargoCrate>;

#[derive(Debug, Clone)]
struct Move
{
    fromStackNum: usize,
    toStackNum: usize,
    cratesToMove: usize,
}

impl Move {
    fn new(fromStackNum: usize, toStackNum: usize, cratesToMove: usize) -> Self { Self { fromStackNum, toStackNum, cratesToMove } }
}

#[derive(Debug, Clone)]
struct ProblemSpace
{
    cargoStacks: Vec<CargoStack>
}

impl ProblemSpace
{
    fn new(cargoStacks: Vec<CargoStack>) -> Self { Self { cargoStacks } }

    fn doMove(&mut self, theMove : &Move) -> &mut Self
    {
        for _ in 0..theMove.cratesToMove {
            let el = self.cargoStacks[theMove.fromStackNum].pop().expect(&format!("From '{}' was empty", theMove.fromStackNum));
            self.cargoStacks[theMove.toStackNum].push(el);
        }
        return self;
    }

    fn doMoves(&mut self, theMoves : &Vec<Move>) -> &mut Self
    {
        return theMoves.iter().fold(self, |acc, elem| acc.doMove(elem));
    }

    fn doMove2(&mut self, theMove : &Move) -> &mut Self
    {
        let indexToSplitAt = self.cargoStacks[theMove.fromStackNum].len() - theMove.cratesToMove;
        let mut elems = self.cargoStacks[theMove.fromStackNum].split_off(indexToSplitAt);
        self.cargoStacks[theMove.toStackNum].append(&mut elems);
        return self;
    }

    fn doMoves2(&mut self, theMoves : &Vec<Move>) -> &mut Self
    {
        return theMoves.iter().fold(self, |acc, elem| acc.doMove2(elem));
    }
}

fn parseInput(filePath : &str) -> Option<(ProblemSpace, Vec<Move>)>
{
   let f = File::open(filePath).ok()?;
   let lines : Vec<_> = BufReader::new(f).lines().collect();

    if lines.iter().any(|l| l.is_err())
    {
        return None;
    }

    let mut cargoMoves = Vec::new();
    cargoMoves.push(Move::new(0,0,0));

    let mut startReadingMoves = false;

    let mut cargoStacksIn : Vec<Vec<_>> = Vec::new();

    for line in lines.into_iter().map(|lr| lr.unwrap())
    {
        if line.trim().is_empty()
        {
            startReadingMoves = true;
            continue;
        }

        if !startReadingMoves
        {
            let chars = line.chars().collect::<Vec<char>>();
            let chunks = chars.chunks(4).map(|chs| String::from_iter(chs).trim().to_owned()).collect();
            cargoStacksIn.push(chunks);
        }
        else
        {
            let words: Vec<_> = line.split(" ").collect();
            assert!(words[0] == "move");
            assert!(words[2] == "from");
            assert!(words[4] == "to");

            let num = words[1].parse::<usize>().unwrap();
            let from = words[3].parse::<usize>().unwrap();
            let to = words[5].parse::<usize>().unwrap();

            assert!(from > 0);
            assert!(to > 0);

            cargoMoves.push(Move::new(from-1, to-1, num));
        }
    }

    // remove numbers row
    cargoStacksIn.pop();

    // process cargo stacks
    let mut cargoStacks: Vec<Vec<CargoCrate>> = Vec::new();

    for line in cargoStacksIn.into_iter().rev()
    {
        for (index, cargoCrate) in line.into_iter().enumerate()
        {
            if cargoCrate.is_empty()
            {
                continue;
            }

            while cargoStacks.get(index).is_none() {
                cargoStacks.push(Vec::new());
            }

            cargoStacks[index].push(CargoCrate::new(cargoCrate));
        }
    }

    let space = ProblemSpace::new(cargoStacks);

    return Some((space, cargoMoves));
}

fn part1(mut space: ProblemSpace, moves: &Vec<Move>) -> String
{
    let crateTops : Vec<_>  = space.doMoves(moves).cargoStacks.iter().map(|stack| stack.last().and_then(|cargoCrate| Some(cargoCrate.clone()))).collect();

    return crateTops.into_iter().map(|op| op.and_then(|cargoCrate| Some(cargoCrate.crateName)).unwrap_or("_".to_owned())).fold(String::new(), |mut acc, elem| { acc.extend(elem.chars()); return  acc; });
}

fn part2(mut space: ProblemSpace, moves: &Vec<Move>) -> String
{
    let crateTops : Vec<_>  = space.doMoves2(moves).cargoStacks.iter().map(|stack| stack.last().and_then(|cargoCrate| Some(cargoCrate.clone()))).collect();

    return crateTops.into_iter().map(|op| op.and_then(|cargoCrate| Some(cargoCrate.crateName)).unwrap_or("_".to_owned())).fold(String::new(), |mut acc, elem| { acc.extend(elem.chars()); return  acc; });
}

fn main() {
    let Some((problemSpace, cargoMoves)) = parseInput("input.txt") else
    {
        return;
    };
    
    println!("{}", part1(problemSpace.clone(), &cargoMoves));
    println!("{}", part2(problemSpace.clone(), &cargoMoves));

}