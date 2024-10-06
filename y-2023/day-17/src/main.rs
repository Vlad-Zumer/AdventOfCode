#![allow(non_snake_case)]
#![allow(dead_code)]
#![allow(unused_imports)]

use std::{
    collections::HashMap,
    fs::File,
    io::{BufRead, BufReader},
    ops::Sub,
};

use utils::nd_vec::NDVec;

fn parseFile(filePath: &str) -> Vec<Vec<u8>> {
    let file = File::open(filePath).expect("Cannot Open");
    let lines = BufReader::new(file)
        .lines()
        .map(|l| l.expect("Error Line Found"));

    return lines
        .map(|line| {
            line.chars()
                .map(|ch| {
                    ch.to_string()
                        .parse()
                        .expect(&format!("char {} is not an parsable as int", ch))
                })
                .collect()
        })
        .collect();
}

type Position2D = NDVec<u16, 2>;

fn manhattanDistance(pos1: &Position2D, pos2: &Position2D) -> u64 {
    let p1 = pos1.map(|x| *x as i32);
    let p2 = pos2.map(|x| *x as i32);
    return (p1 - p2)
        .map(|x| x.abs() as u64)
        .fold(0, |acc, val| acc + val);
}

#[derive(Debug, Hash, Clone, PartialEq, Eq)]
enum Direction {
    N,
    E,
    S,
    W,
}

impl Direction {
    fn getOpposite(&self) -> Direction {
        return match self {
            Direction::N => Direction::S,
            Direction::E => Direction::W,
            Direction::S => Direction::N,
            Direction::W => Direction::E,
        };
    }

    fn get2DVec(&self) -> NDVec<i8, 2> {
        match self {
            Direction::N => NDVec::new([0, -1]),
            Direction::E => NDVec::new([1, 0]),
            Direction::S => NDVec::new([0, 1]),
            Direction::W => NDVec::new([-1, 0]),
        }
    }
}

const DIRECTIONS: &'static [Direction; 4] =
    &[Direction::N, Direction::E, Direction::S, Direction::W];

#[derive(Debug, Clone, Hash, PartialEq, Eq)]
struct NodeIndex {
    pos: Position2D,
    dir: Direction,
    times: u8,
}
impl NodeIndex {
    fn new(pos: Position2D, dir: Direction, times: u8) -> Self {
        return NodeIndex { pos, dir, times };
    }
}

#[derive(Debug, Clone)]
struct MapGraph {
    data: Vec<Vec<u8>>,
    startPos: Position2D,
    endPos: Position2D,
}

impl MapGraph {
    fn numLines(&self) -> usize {
        return self.data.len();
    }

    fn numCols(&self) -> usize {
        return self.data[0].len();
    }

    fn getCost(&self, pos: &Position2D) -> u8 {
        return self.data[pos[1] as usize][pos[0] as usize];
    }
}

// using A* : https://en.wikipedia.org/wiki/A*_search_algorithm#Pseudocode
fn aStar(graph: &MapGraph, part: &Part) -> Option<(Vec<NodeIndex>, u64)> {
    let heuristic = manhattanDistance;

    let mut openSet: Vec<NodeIndex> = vec![];
    let mut gScore: HashMap<NodeIndex, u64> = HashMap::new();
    let mut fScore: HashMap<NodeIndex, u64> = HashMap::new();
    let mut cameFrom: HashMap<NodeIndex, NodeIndex> = HashMap::new();

    let mut neighbors: Vec<NodeIndex> = vec![];

    {
        let startNodeIndex = NodeIndex::new(graph.startPos.clone(), Direction::S, 0);
        openSet.push(startNodeIndex.clone());
        gScore.insert(startNodeIndex.clone(), 0);
        fScore.insert(
            startNodeIndex.clone(),
            heuristic(&startNodeIndex.pos, &graph.endPos),
        );
    }

    while !openSet.is_empty() {
        let curIndex = openSet.pop().unwrap();

        let aux = part != &Part::Part2 || curIndex.times >= 4; 

        if curIndex.pos == graph.endPos && aux {
            let mut path = vec![];
            let mut curPathNode = Some(&curIndex);
            while curPathNode.is_some() {
                let val = curPathNode.unwrap();
                path.push(val.clone());
                curPathNode = cameFrom.get(&val);
            }
            assert!(path[0].pos == graph.endPos);
            path.reverse();
            assert!(path[0].pos == graph.startPos);
            return Some((path, *gScore.get(&curIndex).unwrap()));
        }

        let validDirs = DIRECTIONS
            .iter()
            .filter(|dir| 
                // don't go back
                // times == 0 to distinguish starting pos, allowing all moves
                curIndex.times == 0 || &&curIndex.dir.getOpposite() != dir
            )
            .filter(|dir| 
                // times == 0 to distinguish starting pos, allowing all moves
                curIndex.times == 0 || match part {
                    // don't go more than 3 spaces in a line
                    Part::Part1 => curIndex.times < 3 || &&curIndex.dir != dir,
                    // go at least 4 spaces in a line, but not more than 10
                    Part::Part2 => (&&curIndex.dir == dir && curIndex.times < 10) || (&&curIndex.dir != dir && curIndex.times >= 4),
                }
            );

        neighbors.clear();

        for dir in validDirs {
            // create new pos
            let newPos = curIndex.pos.map(|x| *x as i32) + dir.get2DVec().map(|x| *x as i32);
            if  newPos[0] < 0 || 
                newPos[1] < 0 || 
                newPos[0] as usize >= graph.numCols() || 
                newPos[1] as usize >= graph.numLines() {
                continue;
            }

            // create new index
            let times = if dir == &curIndex.dir {curIndex.times+1} else {1};
            let newIndex = NodeIndex::new(newPos.map(|x| *x as u16), dir.clone(), times);
            neighbors.push(newIndex);
        }

        for neighbor in neighbors.iter() {
            let newGScore = gScore.get(&curIndex).unwrap() + graph.getCost(&neighbor.pos) as u64;
            let newFScore = newGScore + heuristic(&neighbor.pos, &graph.endPos);
            if gScore.get(&neighbor).unwrap_or(&u64::MAX) > &newGScore{
                cameFrom.insert(neighbor.clone(), curIndex.clone());
                gScore.insert(neighbor.clone(), newGScore);
                fScore.insert(neighbor.clone(), newFScore);

                if !openSet.contains(&neighbor)
                {
                    // insert so that open set is sorted desc by fScore
                    let index = openSet.partition_point(|index| fScore.get(index).unwrap() >= &newFScore );
                    openSet.insert(index, neighbor.clone());
                }
            }
        }
    }

    return None;
}

#[derive(Debug, PartialEq, Eq)]
enum Part {
    Part1,
    Part2
}

fn printSol(input: &Vec<Vec<u8>>, path: Vec<NodeIndex>, _cost: u64)
{
    let mut strInput = input.iter().map(|line| line.iter().map(|x| x.to_string()).collect::<Vec<_>>()).collect::<Vec<_>>();

    for node in path.iter().skip(1) {
        strInput[node.pos[1] as usize][node.pos[0] as usize] = match node.dir {
            Direction::N => "^".to_owned(),
            Direction::E => ">".to_owned(),
            Direction::S => "v".to_owned(),
            Direction::W => "<".to_owned(),
        }
    }

    for line in strInput {
        for s in line{
            print!("{}", s);
        }
        print!("\n");
    }
}

fn main() {
    let input = parseFile("./test.txt");
    // let input = parseFile("./input.txt");

    let numLines = input.len();
    let numCols = input[0].len();

    assert!(numLines < u16::MAX.into());
    assert!(numCols < u16::MAX.into());

    let graph = MapGraph {
        data: input.clone(),
        startPos: Position2D::new([0, 0]),
        endPos: Position2D::new([numCols.sub(1) as u16, numLines.sub(1) as u16]),
    };


    let p1 = aStar(&graph, &Part::Part1).expect("No path found for P1, how?");
    let p2 = aStar(&graph, &Part::Part2).expect("No path found for P2, how?");
    println!("P1: {}", p1.1);
    println!("P2: {}", p2.1);
    // println!();
    // println!("Sol 1:");
    // printSol(&input, p1.0, p1.1);
    // println!();
    // println!("Sol 2:");
    // printSol(&input, p2.0, p2.1);
}
