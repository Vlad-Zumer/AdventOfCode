#![allow(non_snake_case)]
// #![allow(dead_code)]
// #![allow(unused_imports)]
use std::{
    fs::File,
    io::BufRead,
    io::BufReader,
};

use utils::{
    nd_vec::numeric_traits::SignedNumeric,
    nd_vec::Vec2,
    vec_extensions::Transposable,
};

#[derive(Debug, PartialEq, Clone, Copy, Eq)]
enum MapElement {
    Empty,
    Galaxy,
}

#[derive(Debug, PartialEq, Eq, Clone)]
enum ExpandIndex {
    X(usize),
    Y(usize),
}

fn parseFile(filePath: &str) -> Vec<Vec<MapElement>> {
    let file = File::open(filePath).expect("Cannot Open");
    let lines = BufReader::new(file)
        .lines()
        .map(|l| {
            l.expect("Error Line Found")
                .chars()
                .map(|c| match c {
                    '.' => MapElement::Empty,
                    '#' => MapElement::Galaxy,
                    _ => panic!("Unrecognized input: '{}'", c),
                })
                .collect()
        })
        .collect();

    return lines;
}

fn getExpandIndices(map: &Vec<Vec<MapElement>>) -> Vec<ExpandIndex> {
    let mut ret_val = vec![];

    for (y, line) in map.iter().enumerate() {
        if line.iter().all(|el| el == &MapElement::Empty) {
            ret_val.push(ExpandIndex::Y(y));
        }
    }

    for (x, line) in map.transpose().iter().enumerate() {
        if line.iter().all(|el| el == &MapElement::Empty) {
            ret_val.push(ExpandIndex::X(x));
        }
    }

    return ret_val;
}

type VecSize = i64;

fn getGalaxies(
    map: &Vec<Vec<MapElement>>,
    expandedSpace: &Vec<ExpandIndex>,
    expandFactor: usize,
) -> Vec<Vec2<VecSize>> {
    let mut ret_val = vec![];

    for (y, line) in map.iter().enumerate() {
        for (x, el) in line.iter().enumerate() {
            if el == &MapElement::Galaxy {
                let expandX = expandedSpace
                    .iter()
                    .filter(|es| match es {
                        ExpandIndex::X(val) => x > *val,
                        ExpandIndex::Y(_) => false,
                    })
                    .count();

                let expandY = expandedSpace
                    .iter()
                    .filter(|es| match es {
                        ExpandIndex::X(_) => false,
                        ExpandIndex::Y(val) => y > *val,
                    })
                    .count();

                ret_val.push(Vec2::new([
                    (x + expandX * (expandFactor - 1)) as VecSize,
                    (y + expandY * (expandFactor - 1)) as VecSize,
                ]));

                // println!(
                //     "Galaxy@{}, X:{}, Y{}, EX:{}, EY:{}, FULL_EX:{}, FULL_EY:{}",
                //     ret_val.last().unwrap(),
                //     x,
                //     y,
                //     expandX,
                //     expandY,
                //     expandX * expandFactor,
                //     expandY * expandFactor
                // );
            }
        }
    }

    return ret_val;
}

fn manhattan_dist<T: SignedNumeric>(v1: &Vec2<T>, v2: &Vec2<T>) -> T {
    return (v1.clone() - v2.clone())
        .map(|a| a.abs())
        .fold(T::default(), |acc, it| acc + *it);
}

fn sumOfDistinctPairsLineDistance(galaxies: &Vec<Vec2<VecSize>>) -> u64 {
    let aux = galaxies
        .iter()
        .enumerate()
        .map(|(index, g)| (g, galaxies.iter().skip(index + 1)))
        .map(|(g, gs)| {
            gs.map(|ng| manhattan_dist(g, ng))
                .map(|v| v as u64)
                .sum::<u64>()
        })
        .sum();

    return aux;
}

fn main() {
    // let map = parseFile("./test.txt");
    let map = parseFile("./input.txt");
    let expandedSpace = getExpandIndices(&map);
    let galaxiesP1 = getGalaxies(&map, &expandedSpace, 2);
    let galaxiesP2 = getGalaxies(&map, &expandedSpace, 1_000_000);

    // for galaxy in galaxies.iter() {
    //     println!("{}", galaxy);
    // }

    // for exs in expandedSpace.iter() {
    //     println!("{:?}", exs);
    // }

    let p1 = sumOfDistinctPairsLineDistance(&galaxiesP1);
    let p2 = sumOfDistinctPairsLineDistance(&galaxiesP2);
    println!("P1: {}", p1);
    println!("P2: {}", p2);
}
