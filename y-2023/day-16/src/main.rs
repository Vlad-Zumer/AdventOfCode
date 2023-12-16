#![allow(non_snake_case)]
// #![allow(dead_code)]
// #![allow(unused_imports)]
use std::{
    collections::{HashSet, VecDeque},
    fs::File,
    io::{BufRead, BufReader},
};

#[derive(Debug, PartialEq, Eq, Clone, Copy, Hash)]
enum Side {
    Top,
    Bottom,
    Left,
    Right,
}

impl Side {
    fn getOppositeSide(&self) -> Side {
        match self {
            Side::Top => Side::Bottom,
            Side::Bottom => Side::Top,
            Side::Left => Side::Right,
            Side::Right => Side::Left,
        }
    }

    fn getOrthogonalSides(&self) -> [Side; 2] {
        match self {
            Side::Top | Side::Bottom => [Side::Left, Side::Right],
            Side::Left | Side::Right => [Side::Top, Side::Bottom],
        }
    }

    fn getOffset(&self) -> (i8, i8) {
        match self {
            Side::Top => (0, -1),
            Side::Bottom => (0, 1),
            Side::Left => (-1, 0),
            Side::Right => (1, 0),
        }
    }
}

#[derive(Debug, PartialEq, Eq, Clone, Copy)]
enum CellType {
    Empty,
    VSplitter,  // Splits vertical    on   the horizontal
    HSplitter,  // Splits horizontal  on   the vertical
    MirrorLBRT, // Left to Bottom   &   Right to Top    (\)
    MirrorLTRB, // Left to Top      &   Right to Bottom (/)
}

impl CellType {
    fn getExitSides(&self, entry: Side) -> HashSet<Side> {
        match (self, entry) {
            (CellType::HSplitter, Side::Left) => HashSet::from(Side::Left.getOrthogonalSides()),
            (CellType::HSplitter, Side::Right) => HashSet::from(Side::Right.getOrthogonalSides()),
            (CellType::VSplitter, Side::Top) => HashSet::from(Side::Top.getOrthogonalSides()),
            (CellType::VSplitter, Side::Bottom) => HashSet::from(Side::Bottom.getOrthogonalSides()),
            (CellType::MirrorLBRT, Side::Top) => HashSet::from([Side::Right]),
            (CellType::MirrorLBRT, Side::Bottom) => HashSet::from([Side::Left]),
            (CellType::MirrorLBRT, Side::Left) => HashSet::from([Side::Bottom]),
            (CellType::MirrorLBRT, Side::Right) => HashSet::from([Side::Top]),
            (CellType::MirrorLTRB, Side::Top) => HashSet::from([Side::Left]),
            (CellType::MirrorLTRB, Side::Bottom) => HashSet::from([Side::Right]),
            (CellType::MirrorLTRB, Side::Left) => HashSet::from([Side::Top]),
            (CellType::MirrorLTRB, Side::Right) => HashSet::from([Side::Bottom]),
            (CellType::Empty, entry) => HashSet::from([entry.getOppositeSide()]),
            (_, entry) => HashSet::from([entry.getOppositeSide()]),
        }
    }
}

#[derive(Debug, Clone)]
struct Cell {
    laserEntries: HashSet<Side>,
    laserExits: HashSet<Side>,
    cellType: CellType,
}

impl Cell {
    fn new(cellType: CellType) -> Cell {
        Cell {
            laserEntries: HashSet::new(),
            laserExits: HashSet::new(),
            cellType,
        }
    }

    fn hasLaserPathBeenTaken(&self, entry: Side) -> bool {
        return self.laserEntries.contains(&entry);
    }

    fn laserEnteredCell(&mut self, entry: Side) -> &mut Self {
        self.laserEntries.insert(entry);
        for exitSide in self.cellType.getExitSides(entry) {
            self.laserExits.insert(exitSide);
        }
        self
    }
}

fn parseFile(filePath: &str) -> Vec<Vec<Cell>> {
    let file = File::open(filePath).expect("Cannot Open");
    let lines = BufReader::new(file)
        .lines()
        .map(|line| {
            line.expect("Cannot read line")
                .chars()
                .map(|ch| match ch {
                    '.' => Cell::new(CellType::Empty),
                    '-' => Cell::new(CellType::VSplitter),
                    '|' => Cell::new(CellType::HSplitter),
                    '/' => Cell::new(CellType::MirrorLTRB),
                    '\\' => Cell::new(CellType::MirrorLBRT),
                    _ => panic!("Unknown char"),
                })
                .collect::<Vec<_>>()
        })
        .collect::<Vec<_>>();

    return lines;
}

fn ImAFiringMahLazor(
    map: &mut Vec<Vec<Cell>>,
    entrySide: Side,
    initCellXY: (usize, usize),
) -> &mut Vec<Vec<Cell>> {
    let mut toProcessQ = VecDeque::from([(initCellXY.0, initCellXY.1, entrySide)]);

    while toProcessQ.len() > 0 {
        let (x, y, entrySide) = toProcessQ.pop_front().unwrap();

        if y < map.len() && x < map[y].len() && !map[y][x].hasLaserPathBeenTaken(entrySide) {
            let exitSides = map[y][x].cellType.getExitSides(entrySide);
            map[y][x].laserEnteredCell(entrySide);
            for side in exitSides {
                let (dx, dy) = side.getOffset();
                toProcessQ.push_back((
                    x.wrapping_add_signed(dx as isize),
                    y.wrapping_add_signed(dy as isize),
                    side.getOppositeSide(),
                ));
            }
        }
    }

    return map;
}

fn ImAFiringAllMahLazorz(map: &Vec<Vec<Cell>>) -> Vec<Vec<Vec<Cell>>> {
    let mut ret = vec![];
    for y in [0, map.len() - 1] {
        for x in 0..map[y].len() {
            if y == 0 {
                let mut clone = map.clone();
                ImAFiringMahLazor(&mut clone, Side::Top, (x, y));
                ret.push(clone);
            }

            if y == (map.len() - 1) {
                let mut clone = map.clone();
                ImAFiringMahLazor(&mut clone, Side::Bottom, (x, y));
                ret.push(clone);
            }
        }
    }

    for y in 0..map.len() - 1 {
        for x in [0, map[y].len()] {
            if x == 0 {
                let mut clone = map.clone();
                ImAFiringMahLazor(&mut clone, Side::Left, (x, y));
                ret.push(clone);
            }

            if x == (map[y].len() - 1) {
                let mut clone = map.clone();
                ImAFiringMahLazor(&mut clone, Side::Right, (x, y));
                ret.push(clone);
            }
        }
    }

    return ret;
}

fn CalcNumberOfEnergizedCells(map: &Vec<Vec<Cell>>) -> u64 {
    map.iter()
        .map(|line| {
            line.iter()
                .map(|cell| {
                    if cell.laserEntries.len() > 0 {
                        1u8
                    } else {
                        0u8
                    }
                })
                .fold(0u64, |acc, val| acc + (val as u64))
        })
        .sum::<u64>()
}

fn part1(map: &Vec<Vec<Cell>>) -> u64 {
    CalcNumberOfEnergizedCells(ImAFiringMahLazor(&mut map.clone(), Side::Left, (0, 0)))
}

fn part2(map: &Vec<Vec<Cell>>) -> u64 {
    ImAFiringAllMahLazorz(map)
        .iter()
        .map(|map| CalcNumberOfEnergizedCells(map))
        .max()
        .unwrap()
}

fn main() {
    // let input = parseFile("./test.txt");
    let input = parseFile("./input.txt");

    let p1: u64 = part1(&input);

    // this takes a while
    // maybe not a good idea to create that much memory
    let p2: u64 = part2(&input);

    println!("P1: {}", p1);
    println!("P2: {}", p2);
}
