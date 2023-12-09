#![allow(non_snake_case)]
use std::{collections::VecDeque, fs::File, io::BufRead, io::BufReader, num::NonZeroUsize};

pub struct MyWindows<I: Iterator> {
    iter: I,
    size: NonZeroUsize,
    buffer: VecDeque<I::Item>,
}

impl<I: Iterator> MyWindows<I> {
    pub fn new(iter: I, size: NonZeroUsize) -> Self {
        return MyWindows {
            iter,
            size,
            buffer: VecDeque::new(),
        };
    }
}

impl<I: Iterator> Iterator for MyWindows<I>
where
    I::Item: Clone,
{
    type Item = Vec<I::Item>;

    fn next(&mut self) -> Option<Self::Item> {
        if self.buffer.is_empty() {
            let bufCandidate = (0..self.size.get() - 1)
                .map(|_| self.iter.next())
                .collect::<Vec<_>>();
            if bufCandidate.iter().any(|el| el.is_none()) {
                return None;
            } else {
                self.buffer = bufCandidate.into_iter().map(|op| op.unwrap()).collect();
            }
        }

        let newVal = self.iter.next()?;

        self.buffer.push_back(newVal);
        let retVal = self.buffer.iter().map(|el| el.clone()).collect::<Vec<_>>();

        self.buffer.pop_front();

        return Some(retVal);
    }
}

trait Windowable: Iterator + Sized {
    fn windows(self, size: usize) -> MyWindows<Self> {
        let nzSize = NonZeroUsize::new(size).expect("window size must be non-zero");
        return MyWindows::new(self, nzSize);
    }
}

impl<T> Windowable for T where T: Iterator + Sized {}

fn parseFile(filePath: &str) -> Vec<Vec<i32>> {
    let file = File::open(filePath).expect("Cannot Open");
    let lines = BufReader::new(file)
        .lines()
        .map(|l| l.expect("Error Line Found"))
        .map(|l| {
            l.split(' ')
                .filter(|s| !s.is_empty())
                .map(|num| num.parse::<i32>().expect("Fucked up parsing"))
                .collect::<Vec<i32>>()
        })
        .collect::<Vec<_>>();

    return lines;
}

fn part1(lines: &Vec<Vec<i32>>) -> i128 {
    let mut sum = 0;
    for line in lines {
        let mut table: Vec<Vec<i32>> = vec![];
        let mut curr = line.clone();
        table.push(curr.clone());
        while curr.iter().any(|el| *el != 0) {
            curr = curr.iter().windows(2).map(|pair| pair[1] - pair[0]).collect();
            table.push(curr.clone());
        }
        let mut prevEl:i128 = 0;
        for line in table.iter().rev() {
            let last = line.last().unwrap();
            prevEl = prevEl + (*last as i128);
        }
        sum += prevEl;
    }

    return sum;
}

fn part2(lines: &Vec<Vec<i32>>) -> i128 {
    let mut sum = 0;
    for line in lines {
        let mut table: Vec<Vec<i32>> = vec![];
        let mut curr = line.clone();
        table.push(curr.clone());
        while curr.iter().any(|el| *el != 0) {
            curr = curr.iter().windows(2).map(|pair| pair[1] - pair[0]).collect();
            table.push(curr.clone());
        }
        let mut prevEl:i128 = 0;
        for line in table.iter().rev() {
            let first = line.first().unwrap();
            prevEl = (*first as i128) - prevEl;
        }
        sum += prevEl;
    }

    return sum;
}

fn main() {
    // let lines = parseFile("./test.txt");
    let lines = parseFile("./input.txt");
    
    let p1 = part1(&lines);
    println!("P1: {}", p1);

    let p2 = part2(&lines);
    println!("P2: {}", p2);
}
