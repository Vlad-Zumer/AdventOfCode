#![allow(non_snake_case)]
// #![allow(dead_code)]
// #![allow(unused_imports)]

use std::{
    collections::{HashMap, HashSet},
    fs::File,
    io::{BufRead, BufReader},
    time::Instant,
};

use utils::{algorithms::gcd, nd_vec::Vec2};

type Vec2D = Vec2<i16>;

#[derive(Debug, Clone)]
struct ParsedInput {
    antennas: HashMap<char, Vec<Vec2D>>,
    xySize: Vec2D,
}

fn parseFile(filePath: &str) -> ParsedInput {
    let file = File::open(filePath).expect("Cannot Open");
    let lines = BufReader::new(file).lines();

    let mut antennas: HashMap<char, Vec<Vec2D>> = HashMap::new();
    let mut xySize = Vec2D::zero();

    for (lIndex, l) in lines.enumerate() {
        for (cIndex, c) in l.expect("Failed to get line").chars().enumerate() {
            match c {
                '.' => (),
                x if x.is_alphanumeric() => {
                    let pos = Vec2D::new([cIndex.try_into().unwrap(), lIndex.try_into().unwrap()]);
                    if antennas.contains_key(&x) {
                        antennas.get_mut(&x).unwrap().push(pos);
                    } else {
                        antennas.insert(x, vec![pos]);
                    }
                }
                x => panic!(
                    "Unrecognized char '{x}' at line={}, col={}",
                    lIndex + 1,
                    cIndex + 1
                ),
            }
            if cIndex + 1 > xySize[0].try_into().unwrap() {
                xySize[0] = (cIndex + 1).try_into().unwrap();
            }
        }
        if lIndex + 1 > xySize[1].try_into().unwrap() {
            xySize[1] = (lIndex + 1).try_into().unwrap();
        }
    }

    return ParsedInput { antennas, xySize };
}

fn getAllPairs<T: Clone>(things: &Vec<T>) -> Vec<(T, T)> {
    let mut ret = vec![];
    for (index, t1) in things.iter().enumerate() {
        for t2 in things.iter().skip(index + 1) {
            ret.push((t1.clone(), t2.clone()));
        }
    }
    return ret;
}

fn isWithinLimits(val: &Vec2D, xySize: &Vec2D) -> bool {
    return val[0] >= 0 && val[1] >= 0 && val[0] < xySize[0] && val[1] < xySize[1];
}

fn part1(input: &ParsedInput) -> u64 {
    let mut allAntinodes: HashSet<Vec2D> = HashSet::new();

    for (_ch, antennas) in input.antennas.iter() {
        let antinodes = getAllPairs(antennas)
            .iter()
            .flat_map(|(x, y)| vec![(x - y) * 2 + y, (y - x) * 2 + x])
            .filter(|x| isWithinLimits(x, &input.xySize))
            .collect::<Vec<_>>();

        for antinode in antinodes {
            allAntinodes.insert(antinode);
        }
    }

    return allAntinodes.len().try_into().unwrap();
}

fn genAllAntinodes(p1: &Vec2D, p2: &Vec2D, xySize: &Vec2D) -> Vec<Vec2D> {
    let mut ret = vec![];

    let diff = p1 - p2;
    let dir = &diff / gcd(diff[0].abs(), diff[1].abs());

    let mut i = 0;
    while isWithinLimits(&(p2 + (&dir * i)), xySize) {
        ret.push(p2 + (&dir * i));
        i += 1;
    }
    i = -1;
    while isWithinLimits(&(p2 + (&dir * i)), xySize) {
        ret.push(p2 + (&dir * i));
        i -= 1;
    }

    return ret;
}

fn part2(input: &ParsedInput) -> u64 {
    let mut allAntinodes: HashSet<Vec2D> = HashSet::new();

    for (_ch, antennas) in input.antennas.iter() {
        let antinodes = getAllPairs(antennas)
            .iter()
            .flat_map(|(x, y)| genAllAntinodes(x, y, &input.xySize))
            .filter(|x| isWithinLimits(x, &input.xySize))
            .collect::<Vec<_>>();

        for antinode in antinodes {
            allAntinodes.insert(antinode);
        }
    }

    return allAntinodes.len().try_into().unwrap();
}

fn main() {
    // let input = parseFile("./test.txt");
    let input = parseFile("./input.txt");

    {
        let now = Instant::now();
        let p1: u64 = part1(&input);
        println!("[{:.3?}] P1: {}", now.elapsed(), p1);
    }
    {
        let now = Instant::now();
        let p2: u64 = part2(&input);
        println!("[{:.3?}] P2: {}", now.elapsed(), p2);
    }
}
