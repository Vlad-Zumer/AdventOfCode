#![allow(non_snake_case)]
// #![allow(dead_code)]
// #![allow(unused_imports)]

use std::{
    collections::HashSet,
    fs::File,
    io::{BufRead, BufReader},
    thread,
    time::Instant,
};

use utils::nd_vec::Vec2;
type Vec2D = Vec2<i32>;

#[derive(Debug, Clone, Hash, PartialEq, Eq)]
enum Direction {
    N,
    E,
    S,
    W,
}

impl Direction {
    fn nextDGuardDir(&self) -> Direction {
        match self {
            Direction::N => Direction::E,
            Direction::E => Direction::S,
            Direction::S => Direction::W,
            Direction::W => Direction::N,
        }
    }

    fn asVec(&self) -> Vec2D {
        match self {
            Direction::N => Vec2D::new([0, -1]),
            Direction::E => Vec2D::new([1, 0]),
            Direction::S => Vec2D::new([0, 1]),
            Direction::W => Vec2D::new([-1, 0]),
        }
    }
}

#[derive(Debug, Clone)]
struct ParsedInput {
    guardPos: Vec2D,
    blockers: HashSet<Vec2D>,
    xySize: Vec2D,
}

fn parseFile(filePath: &str) -> ParsedInput {
    let file = File::open(filePath).expect("Cannot Open");
    let mut guardPos: Option<Vec2D> = None;
    let mut xySize = Vec2D::zero();
    let lines = BufReader::new(file).lines();
    let blockers = lines
        .enumerate()
        .map(|(lIndex, line)| {
            if xySize[1] < lIndex.try_into().unwrap() {
                xySize[1] = (lIndex + 1).try_into().unwrap();
            }
            line.expect("Cannot read line")
                .chars()
                .enumerate()
                .map(|(cIndex, ch)| {
                    if xySize[0] < cIndex.try_into().unwrap() {
                        xySize[0] = (cIndex + 1).try_into().unwrap();
                    }

                    match ch {
                        '#' => Some(Vec2D::new([
                            cIndex.try_into().unwrap(),
                            lIndex.try_into().unwrap(),
                        ])),
                        '^' => {
                            guardPos = Some(Vec2D::new([
                                cIndex.try_into().unwrap(),
                                lIndex.try_into().unwrap(),
                            ]));
                            None
                        }
                        _ => None,
                    }
                })
                .filter(|op| op.is_some())
                .map(|someOp| someOp.unwrap())
                .collect::<Vec<_>>()
        })
        .flatten()
        .collect::<HashSet<_>>();

    return ParsedInput {
        guardPos: guardPos.expect("Did not find guard position"),
        blockers: blockers,
        xySize,
    };
}

fn isOutsideMap(pos: &Vec2D, mapSize: &Vec2D) -> bool {
    return pos[0] < 0 || pos[1] < 0 || pos[0] >= mapSize[0] || pos[1] >= mapSize[1];
}

#[allow(dead_code)]
fn dumpMap(mapSize: &Vec2D, blockers: &HashSet<Vec2D>, seen: &HashSet<Vec2D>) {
    println!("");
    for y in 0..mapSize[1] {
        for x in 0..mapSize[0] {
            let xyPos = Vec2D::new([x, y]);
            if blockers.contains(&xyPos) {
                print!("#")
            } else if seen.contains(&xyPos) {
                print!("X")
            } else {
                print!(".")
            }
        }
        print!("\n");
    }
    print!("\n");
}

fn getNextState(
    currPos: &Vec2D,
    currDir: &Direction,
    blockers: &HashSet<Vec2D>,
) -> (Vec2D, Direction) {
    let mut nextDir = currDir.clone();
    let mut nextPos = currPos.clone() + nextDir.asVec();
    let mut dirsTried = 0;
    while blockers.contains(&nextPos) {
        nextDir = nextDir.nextDGuardDir();
        nextPos = currPos.clone() + nextDir.asVec();
        dirsTried += 1;
        if dirsTried >= 4 {
            panic!("Tried all possible directions, guard is trapped");
        }
    }

    return (nextPos, nextDir);
}

fn part1(input: &ParsedInput) -> u64 {
    let mut currDir = Direction::N;
    let mut currPos = input.guardPos.clone();
    let mut seen: HashSet<Vec2D> = HashSet::new();

    while !isOutsideMap(&currPos, &input.xySize) {
        seen.insert(currPos.clone());
        let (nextPos, nextDir) = getNextState(&currPos, &currDir, &input.blockers);
        currPos = nextPos;
        currDir = nextDir;
    }

    return seen.len().try_into().unwrap();
}

fn hasCycle(initPos: &Vec2D, mapSize: &Vec2D, blockers: &HashSet<Vec2D>) -> bool {
    let mut currDir = Direction::N;
    let mut currPos = initPos.clone();
    let mut seen: HashSet<(Vec2D, Direction)> = HashSet::new();

    while !isOutsideMap(&currPos, mapSize) {
        if seen.contains(&(currPos.clone(), currDir.clone())) {
            return true;
        }
        seen.insert((currPos.clone(), currDir.clone()));
        let (nextPos, nextDir) = getNextState(&currPos, &currDir, blockers);
        currPos = nextPos;
        currDir = nextDir;
    }

    return false;
}

fn solvePart2(blockersToAdd: &[Vec2D], input: ParsedInput) -> u64 {
    return blockersToAdd
        .iter()
        .map(|blockerToAdd| {
            let mut blockers = HashSet::from(input.blockers.clone());
            blockers.insert(blockerToAdd.clone());
            hasCycle(&input.guardPos, &input.xySize, &blockers)
        })
        .filter(|val| *val)
        .count()
        .try_into()
        .unwrap();
}

#[allow(dead_code)]
fn part2SingleThread(input: &ParsedInput) -> u64 {
    let mut toAdd = vec![];
    for y in 0..input.xySize[1] {
        for x in 0..input.xySize[0] {
            let xyPos = Vec2D::new([x, y]);
            if !(input.blockers.contains(&xyPos) || input.guardPos == xyPos) {
                toAdd.push(xyPos.clone());
            }
        }
    }

    return solvePart2(&toAdd, input.clone());
}

fn getChunkSize(numOfThreads: usize, vecSize: usize) -> usize {
    let mut chunkSize = vecSize / numOfThreads;
    while (vecSize / chunkSize >= numOfThreads) && (vecSize % chunkSize > 0) {
        chunkSize += 1;
    }
    return chunkSize;
}

fn part2Parallel(input: &ParsedInput, numOfThreads: usize) -> u64 {
    let mut toAdd = vec![];
    for y in 0..input.xySize[1] {
        for x in 0..input.xySize[0] {
            let xyPos = Vec2D::new([x, y]);
            if !(input.blockers.contains(&xyPos) || input.guardPos == xyPos) {
                toAdd.push(xyPos.clone());
            }
        }
    }

    let chunkSize = getChunkSize(numOfThreads, toAdd.len());

    let mut chunksIt = toAdd.chunks(chunkSize);
    let mut thHandles = vec![];

    for _ in 0..numOfThreads {
        let ownedChunk = chunksIt.next().unwrap().to_owned();
        let thInputClone = input.clone();
        let thHandle = thread::spawn(move || solvePart2(&ownedChunk, thInputClone));
        thHandles.push(thHandle);
    }

    let mut sum = 0;
    for t in thHandles {
        let result = t.join().unwrap();
        sum += result;
    }

    return sum;
}

fn main() {
    // let input = parseFile("./test.txt");
    let input = parseFile("./input.txt");

    {
        let now = Instant::now();
        let p1: u64 = part1(&input);
        println!("[{:.3?}] P1: {}", now.elapsed(), p1);
    }
    // {
    //     let now = Instant::now();
    //     let p2s: u64 = part2SingleThread(&input);
    //     println!("[{:.3?}] P2 (single thread): {}", now.elapsed(), p2s);
    // }
    {
        let now = Instant::now();
        let p2p: u64 = part2Parallel(&input, 16);
        println!("[{:.3?}] P2 (parallel): {}", now.elapsed(), p2p);
    }
}
