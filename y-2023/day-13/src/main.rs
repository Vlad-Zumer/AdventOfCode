#![allow(non_snake_case)]
#![allow(dead_code)]
#![allow(unused_imports)]
use core::prelude::v1;
use std::{
    cmp::{max, min},
    fmt::Display,
    fs::File,
    io::BufRead,
    io::BufReader,
};

use utils::vec_extensions::Transposable;

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
enum TaggedIndex {
    X(u16),
    Y(u16),
}

#[derive(Debug, Clone, PartialEq)]
struct Mirror(Vec<Vec<char>>);

impl Mirror {
    fn render(&self) -> String {
        return self
            .0
            .iter()
            .map(|l| l.iter().collect::<String>())
            .collect::<Vec<_>>()
            .join("\n");
    }
}

impl Display for Mirror {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(f, "{}", self.render())
    }
}

fn parseFile(filePath: &str) -> Vec<Mirror> {
    let file = File::open(filePath).expect("Cannot Open");
    let lines = BufReader::new(file)
        .lines()
        .map(|l| l.expect("Error Line Found"));

    let mut ret = vec![Mirror(vec![])];
    for line in lines {
        if line.is_empty() {
            ret.push(Mirror(vec![]));
        } else {
            ret.last_mut().unwrap().0.push(line.chars().collect());
        }
    }

    return ret;
}

fn findLowestIndexOfRefl(mirror: &Mirror) -> TaggedIndex {
    fn findIndex(mirror: &Vec<Vec<char>>) -> Option<u16> {
        let mut index = 1;
        while index < mirror.len() {
            let mut eq = true;
            for dist in 0..(min(index, mirror.len() - index)) {
                let i1 = index - dist - 1;
                let i2 = index + dist;
                // println!("  INDEX:{} | I1:{} | I2:{}", index, i1, i2);
                // println!("    M1:'{}'", mirror[i1].iter().collect::<String>());
                // println!("    M2:'{}'", mirror[i2].iter().collect::<String>());
                eq &= mirror[i1] == mirror[i2];
            }
            if eq {
                return Some(TryInto::<u16>::try_into(index).unwrap() - 1);
            }
            index += 1;
        }
        return None;
    }

    let matrix = mirror.0.clone();
    // println!("Y:");
    if let Some(yRefl) = findIndex(&matrix) {
        return TaggedIndex::Y(yRefl);
    }

    // println!("X:");
    if let Some(xRefl) = findIndex(&matrix.transpose()) {
        return TaggedIndex::X(xRefl);
    }

    panic!("No reflection for mirror:\n {}", mirror);
}

fn numberOfDiff<T: PartialEq>(v1: &Vec<T>, v2: &Vec<T>) -> usize {
    let mut ret = max(v1.len(), v2.len()) - min(v1.len(), v2.len());
    for i in 0..(min(v1.len(), v2.len())) {
        if v1[i] != v2[i] {
            ret += 1;
        }
    }
    return ret;
}

fn findLowestIndexOfSmudgyRefl(mirror: &Mirror) -> TaggedIndex {
    fn findIndex(mirror: &Vec<Vec<char>>) -> Option<u16> {
        let mut index = 1;
        while index < mirror.len() {
            let mut eq = true;
            let mut seenSmudge = false;
            for dist in 0..(min(index, mirror.len() - index)) {
                let i1 = index - dist - 1;
                let i2 = index + dist;
                let numOfDiffEl = numberOfDiff(&mirror[i1], &mirror[i2]);
                // println!("  INDEX:{} | I1:{} | I2:{}", index, i1, i2);
                // println!("    M1:'{}'", mirror[i1].iter().collect::<String>());
                // println!("    M2:'{}'", mirror[i2].iter().collect::<String>());
                eq &= numOfDiffEl == 0 || (numOfDiffEl == 1 && !seenSmudge);
                seenSmudge |= numOfDiffEl == 1;
            }
            if eq && seenSmudge {
                return Some(TryInto::<u16>::try_into(index).unwrap() - 1);
            }
            index += 1;
        }
        return None;
    }

    let matrix = mirror.0.clone();
    // println!("Y:");
    if let Some(yRefl) = findIndex(&matrix) {
        return TaggedIndex::Y(yRefl);
    }

    // println!("X:");
    if let Some(xRefl) = findIndex(&matrix.transpose()) {
        return TaggedIndex::X(xRefl);
    }

    panic!("No smudgy reflection for mirror:\n {}", mirror);
}

fn main() {
    // let mirrors = parseFile("./test.txt");
    let mirrors = parseFile("./input.txt");

    let p1 = mirrors
        .iter()
        .map(|mirror| match findLowestIndexOfRefl(mirror) {
            TaggedIndex::X(val) => (val + 1) as u64,
            TaggedIndex::Y(val) => ((val + 1) * 100) as u64,
        })
        .sum::<u64>();

    let p2 = mirrors.iter().map(|mirror|{
        match findLowestIndexOfSmudgyRefl(mirror){
            TaggedIndex::X(val) => (val + 1) as u64,
            TaggedIndex::Y(val) => ((val + 1) * 100) as u64,
        }
    }).sum::<u64>();
    println!("P1: {}", p1);
    println!("P2: {}", p2);
}
