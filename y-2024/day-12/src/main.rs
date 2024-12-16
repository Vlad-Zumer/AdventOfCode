#![allow(non_snake_case)]
// #![allow(dead_code)]
// #![allow(unused_imports)]

use std::{
    cmp::max,
    collections::HashSet,
    fs::File,
    io::{BufReader, Read},
    time::Instant,
};

use utils::{nd_vec::Vec2, vec_extensions::RemoveElem};

type Vec2D = Vec2<i32>;
type PlotMap = Vec<Vec<char>>;

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
enum Side {
    U,
    L,
    D,
    R,
}

impl Side {
    fn getAll() -> [Side; 4] {
        return [Side::U, Side::R, Side::D, Side::L];
    }

    fn getDirection(&self) -> Vec2D {
        return match self {
            Side::U => Vec2D::new([0, -1]),
            Side::L => Vec2D::new([-1, 0]),
            Side::D => Vec2D::new([0, 1]),
            Side::R => Vec2D::new([1, 0]),
        };
    }

    fn getDirections() -> [Vec2D; 4] {
        return Self::getAll().map(|side| side.getDirection());
    }
}

#[derive(Debug, Clone)]
struct Plot {
    _id: char,
    positions: HashSet<Vec2D>,
}

impl Plot {
    fn getArea(&self) -> u64 {
        return self.positions.len().try_into().unwrap();
    }

    fn getPerimeter(&self) -> u64 {
        return self
            .positions
            .iter()
            .map(|pos| {
                Side::getDirections()
                    .map(|dir| pos + dir)
                    .iter()
                    .filter(|nPos| !self.positions.contains(nPos))
                    .count()
            })
            .sum::<usize>()
            .try_into()
            .unwrap();
    }

    // sides == corners
    fn getSides(&self) -> u64 {
        let cornerPairs = [Side::U, Side::D]
            .into_iter()
            .flat_map(|vSide| {
                [Side::L, Side::R]
                    .into_iter()
                    .map(move |hSide| (vSide, hSide))
            })
            .collect::<Vec<_>>();

        // get missing and existing sides for each pos
        // ext-corner happens when missing corner pair
        // int-corner happens when corner pair exists but their diagonal (sum of dir) does not
        let corners = self
            .positions
            .iter()
            .map(|pos| {
                let (missingSides, sides): (Vec<Side>, Vec<Side>) =
                    Side::getAll().into_iter().partition(|side| {
                        let neighborPos = pos + side.getDirection();
                        return !self.positions.contains(&neighborPos);
                    });
                return (pos.clone(), sides, missingSides);
            })
            .map(|(pos, sides, missingSides)| {
                let extCornerCount = cornerPairs
                    .iter()
                    .filter(|(vs, hs)| missingSides.contains(vs) && missingSides.contains(hs))
                    .count();
                let intCornerCount = cornerPairs
                    .iter()
                    .filter(|(vs, hs)| sides.contains(vs) && sides.contains(hs))
                    .filter(|(vs, hs)| {
                        !self
                            .positions
                            .contains(&(&pos + &vs.getDirection() + &hs.getDirection()))
                    })
                    .count();
                return (pos, sides, missingSides, extCornerCount + intCornerCount);
            })
            .collect::<Vec<_>>();

        return corners
            .iter()
            .fold(0, |acc, (_, _, _, count)| acc + (*count as u64));
    }
}

#[derive(Debug, Clone)]
struct ParsedInput {
    _map: PlotMap,
    _plots: Vec<Plot>,
    _xySize: Vec2D,
}

fn parseFile(filePath: &str) -> ParsedInput {
    let file = File::open(filePath).expect("Cannot Open");
    let mut content = String::new();
    BufReader::new(file)
        .read_to_string(&mut content)
        .expect("Failed to read file");

    let mut xySize = Vec2D::zero();
    xySize[1] = max(xySize[1], content.lines().count().try_into().unwrap());

    let mut map = vec![];
    let mut plots = vec![];

    for l in content.lines() {
        map.push(l.chars().collect());
        xySize[0] = max(xySize[0], l.len().try_into().unwrap());
    }

    let mut toExplore = vec![];

    for x in 0..xySize[0] {
        for y in 0..xySize[1] {
            toExplore.push(Vec2D::new([x, y]));
        }
    }

    fn explorePosition(pos: &Vec2D, map: &PlotMap, xySize: &Vec2D) -> Plot {
        let ch = map[pos[1] as usize][pos[0] as usize];
        let mut positions = HashSet::new();
        let mut openSet = vec![pos.clone()];
        let mut seen = HashSet::new();
        while openSet.len() > 0 {
            let curPos = openSet.pop().unwrap();
            if ch != map[curPos[1] as usize][curPos[0] as usize] {
                continue;
            }
            positions.insert(curPos.clone());
            for dir in Side::getDirections() {
                let newPos = &curPos + dir;
                if isWithinLimits(&newPos, xySize)
                    && !openSet.contains(&newPos)
                    && !seen.contains(&newPos)
                {
                    openSet.push(newPos.clone());
                    seen.insert(newPos.clone());
                }
            }
        }

        return Plot { _id: ch, positions };
    }

    while toExplore.len() > 0 {
        let plot = explorePosition(&toExplore.first().unwrap(), &map, &xySize);
        plot.positions.iter().for_each(|pos| {
            toExplore.remove_elem(pos);
        });
        plots.push(plot);
    }

    return ParsedInput {
        _map: map,
        _plots: plots,
        _xySize: xySize,
    };
}

fn isWithinLimits(val: &Vec2D, xySize: &Vec2D) -> bool {
    return val[0] >= 0 && val[1] >= 0 && val[0] < xySize[0] && val[1] < xySize[1];
}

fn part1(input: &ParsedInput) -> u64 {
    return input
        ._plots
        .iter()
        .map(|plot| plot.getArea() * plot.getPerimeter())
        .sum();
}

fn part2(input: &ParsedInput) -> u64 {
    return input
        ._plots
        .iter()
        .map(|plot| plot.getArea() * plot.getSides())
        .sum();
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
