#![allow(non_snake_case)]
use std::{
    fmt::{Display, Write},
    fs::File,
    io::BufRead,
    io::BufReader,
    ops::{Add, AddAssign, Neg, Not, Sub, SubAssign},
};

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
enum Side {
    Left,
    Right,
    Top,
    Bottom,
}

#[allow(dead_code)]
fn canSidesConnect(s1: &Side, s2: &Side) -> bool {
    let retVal = match (s1, s2) {
        (Side::Left, Side::Right) => true,
        (Side::Right, Side::Left) => true,
        (Side::Top, Side::Bottom) => true,
        (Side::Bottom, Side::Top) => true,
        _ => false,
    };
    return retVal;
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
enum MazePiece {
    Empty,
    Start,
    Vertical,
    Horizontal,
    CornerTopLeft,
    CornerTopRight,
    CornerBottomLeft,
    CornerBottomRight,
}

#[allow(dead_code)]
impl MazePiece {
    pub fn values() -> Vec<MazePiece> {
        vec![
            MazePiece::Empty,
            MazePiece::Start,
            MazePiece::Vertical,
            MazePiece::Horizontal,
            MazePiece::CornerTopLeft,
            MazePiece::CornerTopRight,
            MazePiece::CornerBottomLeft,
            MazePiece::CornerBottomRight,
        ]
    }

    pub fn tryNew(c: &char) -> Option<Self> {
        return match c {
            '|' | '\u{2551}' => Some(MazePiece::Vertical),
            '-' | '\u{2550}' => Some(MazePiece::Horizontal),
            'L' | '\u{255A}' => Some(MazePiece::CornerBottomLeft),
            'J' | '\u{255D}' => Some(MazePiece::CornerBottomRight),
            '7' | '\u{2557}' => Some(MazePiece::CornerTopRight),
            'F' | '\u{2554}' => Some(MazePiece::CornerTopLeft),
            '.' | '\u{2591}' | ' ' => Some(MazePiece::Empty),
            'S' | '*' => Some(MazePiece::Start),
            _ => None,
        };
    }

    pub fn render(&self) -> char {
        match self {
            MazePiece::Start => '*',
            MazePiece::Empty => ' ', //'\u{2591}',
            MazePiece::Vertical => '\u{2551}',
            MazePiece::Horizontal => '\u{2550}',
            MazePiece::CornerTopLeft => '\u{2554}',
            MazePiece::CornerTopRight => '\u{2557}',
            MazePiece::CornerBottomLeft => '\u{255A}',
            MazePiece::CornerBottomRight => '\u{255D}',
        }
    }

    pub fn renderOriginal(&self) -> char {
        match self {
            MazePiece::Empty => '.',
            MazePiece::Start => 'S',
            MazePiece::Vertical => '|',
            MazePiece::Horizontal => '-',
            MazePiece::CornerTopLeft => 'F',
            MazePiece::CornerTopRight => '7',
            MazePiece::CornerBottomLeft => 'L',
            MazePiece::CornerBottomRight => 'J',
        }
    }

    pub fn getConnectionSides(&self) -> Option<[Side; 2]> {
        match self {
            MazePiece::Empty => None,
            MazePiece::Start => None,
            MazePiece::Vertical => Some([Side::Top, Side::Bottom]),
            MazePiece::Horizontal => Some([Side::Right, Side::Left]),
            MazePiece::CornerTopLeft => Some([Side::Bottom, Side::Right]),
            MazePiece::CornerTopRight => Some([Side::Left, Side::Bottom]),
            MazePiece::CornerBottomLeft => Some([Side::Right, Side::Top]),
            MazePiece::CornerBottomRight => Some([Side::Left, Side::Top]),
        }
    }

    pub fn getPiecesThatCanConnect(&self, side: &Side) -> Vec<MazePiece> {
        let myConnSides = self.getConnectionSides();
        if !myConnSides.map_or(false, |arr| arr.contains(side)) {
            return vec![];
        }

        let retVal = MazePiece::values()
            .into_iter()
            .filter(|piece| {
                !piece
                    .getConnectionSides()
                    .map_or(vec![], |x| x.to_vec())
                    .iter()
                    .filter(|sideIt| canSidesConnect(side, sideIt))
                    .collect::<Vec<_>>()
                    .is_empty()
            })
            .collect::<Vec<_>>();

        return retVal;
    }
}

#[allow(dead_code)]
impl Display for MazePiece {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        f.write_char(self.render())
    }
}

#[allow(dead_code)]
struct MazeLine<'a>(&'a Vec<MazePiece>);

#[allow(dead_code)]
impl<'a> Display for MazeLine<'a> {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        f.write_fmt(format_args!(
            "{}",
            self.0.iter().map(|maze| maze.render()).collect::<String>()
        ))
    }
}

#[allow(dead_code)]
#[derive(Debug, Clone, PartialEq, Eq)]
struct Position {
    x: i32,
    y: i32,
}

impl Add for Position {
    type Output = Self;

    fn add(self, rhs: Self) -> Self::Output {
        return Position {
            x: self.x + rhs.x,
            y: self.y + rhs.y,
        };
    }
}

impl AddAssign for Position {
    fn add_assign(&mut self, rhs: Self) {
        self.x += rhs.x;
        self.y += rhs.y;
    }
}

impl Neg for Position {
    type Output = Self;

    fn neg(self) -> Self::Output {
        return Position {
            x: -self.x,
            y: -self.y,
        };
    }
}

impl Sub for Position {
    type Output = Self;

    fn sub(self, rhs: Self) -> Self::Output {
        return Position {
            x: self.x - rhs.x,
            y: self.y - rhs.y,
        };
    }
}

impl SubAssign for Position {
    fn sub_assign(&mut self, rhs: Self) {
        self.x -= rhs.x;
        self.y -= rhs.y;
    }
}

#[allow(dead_code)]
fn directionToPosDelta(direction: &Side) -> Position {
    match direction {
        Side::Left => Position { x: -1, y: 0 },
        Side::Right => Position { x: 1, y: 0 },
        Side::Top => Position { x: 0, y: -1 },
        Side::Bottom => Position { x: 0, y: 1 },
    }
}

#[allow(dead_code)]
fn complement(s: &Side) -> Side {
    match s {
        Side::Left => Side::Right,
        Side::Right => Side::Left,
        Side::Top => Side::Bottom,
        Side::Bottom => Side::Top,
    }
}

#[allow(dead_code)]
#[derive(Debug, Clone, PartialEq, Eq)]
struct TrackingPos {
    curr: Position,
    entrySide: Side,
    dist: u64,
}

#[allow(dead_code)]
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
enum Fill {
    Filled,
    Empty,
}

#[allow(dead_code)]
impl Not for Fill {
    type Output = Self;

    fn not(self) -> Self::Output {
        match self {
            Fill::Filled => Fill::Empty,
            Fill::Empty => Fill::Filled,
        }
    }
}

#[allow(dead_code)]
#[derive(Debug, Clone, PartialEq)]
struct FillMazePiece {
    mazePiece: MazePiece,
    fill: Option<Fill>,
}

impl FillMazePiece {
    pub fn render(&self) -> char {
        match self.fill {
            Some(Fill::Empty) => '`',
            Some(Fill::Filled) => '░',
            None => self.mazePiece.render(),
        }
    }
}

impl Display for FillMazePiece {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        f.write_char(self.render())
    }
}

#[allow(dead_code)]
#[derive(Debug, Clone, PartialEq)]
struct FillMazeLine<'a>(&'a Vec<FillMazePiece>);

impl<'a> Display for FillMazeLine<'a> {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        f.write_fmt(format_args!(
            "{}",
            self.0.iter().map(|maze| maze.render()).collect::<String>()
        ))
    }
}

fn parseFile(filePath: &str) -> Vec<Vec<MazePiece>> {
    let file = File::open(filePath).expect("Cannot Open");
    let lines = BufReader::new(file)
        .lines()
        .map(|l| l.expect("Error Line Found"))
        .map(|l| {
            l.chars()
                .map(|c| MazePiece::tryNew(&c).expect("Fucked up parsing"))
                .collect::<Vec<_>>()
        })
        .collect::<Vec<_>>();

    return lines;
}

fn findStartPos(maze: &Vec<Vec<MazePiece>>) -> Option<Position> {
    for (y, mazeLine) in maze.iter().enumerate() {
        for (x, mazeTile) in mazeLine.iter().enumerate() {
            if mazeTile == &MazePiece::Start {
                return Some(Position {
                    x: (x as i32),
                    y: (y as i32),
                });
            }
        }
    }

    return None;
}

fn getAdvance(pos: &TrackingPos, maze: &Vec<Vec<MazePiece>>) -> TrackingPos {
    let tile = &maze[pos.curr.y as usize][pos.curr.x as usize];
    let exitSide = tile
        .getConnectionSides()
        .map_or(vec![], |x| x.to_vec())
        .iter()
        .filter(|side| side != &&pos.entrySide)
        .next()
        .expect(&format!(
            "There should be at least one {:?} + '{}'",
            pos, tile
        ))
        .clone();
    let delta = directionToPosDelta(&exitSide);

    return TrackingPos {
        curr: pos.curr.clone() + delta,
        entrySide: complement(&exitSide),
        dist: pos.dist + 1,
    };
}

fn getStart(maze: &Vec<Vec<MazePiece>>) -> (Position, MazePiece) {
    // find start pos
    let startPos = findStartPos(maze).expect("No start found");
    // println!("{:?}", startPos);
    // find start maze type
    let moveAndSides = vec![
        (-1, 0, Side::Right),
        (1, 0, Side::Left),
        (0, -1, Side::Bottom),
        (0, 1, Side::Top),
    ];
    let possibleStarts = moveAndSides
        .iter()
        .map(|(dx, dy, side)| {
            let (newPosX, newPosY) = (startPos.x + dx, startPos.y + dy);
            let (minX, minY) = (0, 0);
            let (maxX, maxY) = (maze[0].len(), maze.len());
            if newPosX < minX
                || newPosX > maxX.try_into().unwrap()
                || newPosY < minY
                || newPosY > maxY.try_into().unwrap()
            {
                return vec![];
            }
            let tileAtPos = &maze[newPosY as usize][newPosX as usize];
            let possiblePieces = tileAtPos.getPiecesThatCanConnect(side);
            // println!(
            //     "Maze{:?}:'{}' Side:{:?} , Pos:{:?}",
            //     (newPosX, newPosY),
            //     tileAtPos,
            //     side,
            //     possiblePieces
            // );

            return possiblePieces;
        })
        .filter(|pieceVec| !pieceVec.is_empty())
        .collect::<Vec<_>>();

    assert!(
        possibleStarts.len() == 2,
        "Piece can only connect to exactly 2 others"
    );

    let startTile = possibleStarts[0]
        .iter()
        .filter(|p1| possibleStarts[1].contains(p1))
        .next()
        .expect("There should be something here")
        .clone();

    return (startPos, startTile);
}

fn part1(maze: &Vec<Vec<MazePiece>>) -> u64 {
    let (startPos, startTile) = getStart(maze);

    // println!("Start Tile:{}", startTile);
    let connections = startTile
        .getConnectionSides()
        .expect("This should be able to connect to something now");

    // p1 = start pos + T/L
    let mut p1 = TrackingPos {
        curr: startPos.clone() + directionToPosDelta(&connections[0]),
        entrySide: complement(&connections[0]),
        dist: 1,
    };

    // p2 = start pos + B/R
    let mut p2 = TrackingPos {
        curr: startPos.clone() + directionToPosDelta(&connections[1]),
        entrySide: complement(&connections[1]),
        dist: 1,
    };

    // for line in maze.iter() {
    //     println!("{}", MazeLine(line));
    // }

    // while p1 != p2 advance p1 and p2 and record distance
    while p1.curr != p2.curr {
        // println!("P1:{:?} - {}",p1, &maze[p1.curr.x as usize][p1.curr.y as usize]);
        // println!("P2:{:?} - {}",p2, &maze[p2.curr.x as usize][p2.curr.y as usize]);
        p1 = getAdvance(&p1, maze);
        p2 = getAdvance(&p2, maze);
    }
    // print dist
    // println!("{:?}", p1);
    return p1.dist;
}

fn part2(maze: &Vec<Vec<MazePiece>>) -> u64 {
    let (startPos, startTile) = getStart(maze);

    // println!("Start Tile:{}", startTile);
    let connections = startTile
        .getConnectionSides()
        .expect("This should be able to connect to something now");

    // p1 = start pos + T/L
    let mut p1 = TrackingPos {
        curr: startPos.clone() + directionToPosDelta(&connections[0]),
        entrySide: complement(&connections[0]),
        dist: 1,
    };

    // p2 = start pos + B/R
    let mut p2 = TrackingPos {
        curr: startPos.clone() + directionToPosDelta(&connections[1]),
        entrySide: complement(&connections[1]),
        dist: 1,
    };

    // for line in maze.iter() {
    //     println!("{}", MazeLine(line));
    // }

    let mut polygonPos = vec![startPos];

    // while p1 != p2 advance p1 and p2 and record location
    while p1.curr != p2.curr {
        polygonPos.push(p1.curr.clone());
        polygonPos.push(p2.curr.clone());
        // println!("P1:{:?} - {}",p1, &maze[p1.curr.x as usize][p1.curr.y as usize]);
        // println!("P2:{:?} - {}",p2, &maze[p2.curr.x as usize][p2.curr.y as usize]);
        p1 = getAdvance(&p1, maze);
        p2 = getAdvance(&p2, maze);
    }
    polygonPos.push(p1.curr.clone());
    polygonPos.sort_by_key(|el| el.x);
    polygonPos.sort_by_key(|el| el.y);

    // H Scan Fill Rules
    // A ╔════╗ A
    //
    // A ╔════╝ B
    //
    // A ╚════╗ B
    //
    // A ╚════╝ A
    //    ╔
    //  A ║ B
    //    ╝

    let mut filledIntMazeMap: Vec<Vec<FillMazePiece>> = vec![];
    for (y, mazeLine) in maze.iter().enumerate() {
        let mut filledStatus = Fill::Empty;
        let mut prevCorner: Option<MazePiece> = None;
        filledIntMazeMap.push(vec![]);
        for (x, mazePiece) in mazeLine.iter().enumerate() {
            if polygonPos.contains(&Position {
                x: x.try_into().unwrap(),
                y: y.try_into().unwrap(),
            }) {
                // on the poly
                filledIntMazeMap[y].push(FillMazePiece {
                    mazePiece: mazePiece.clone(),
                    fill: None,
                });

                let mappedMazePiece = if mazePiece == &MazePiece::Start {
                    &startTile
                } else {
                    mazePiece
                };

                filledStatus = match mappedMazePiece {
                    MazePiece::Vertical => !filledStatus,
                    MazePiece::Horizontal => filledStatus,
                    MazePiece::CornerTopLeft => {
                        prevCorner = Some(MazePiece::CornerTopLeft);
                        filledStatus
                    }
                    MazePiece::CornerTopRight => {
                        let aux = if prevCorner == Some(MazePiece::CornerTopLeft) {
                            filledStatus
                        } else {
                            !filledStatus
                        };
                        prevCorner = Some(MazePiece::CornerTopRight);
                        aux
                    }
                    MazePiece::CornerBottomLeft => {
                        prevCorner = Some(MazePiece::CornerBottomLeft);
                        filledStatus
                    }
                    MazePiece::CornerBottomRight => {
                        let aux = if prevCorner == Some(MazePiece::CornerBottomLeft) {
                            filledStatus
                        } else {
                            !filledStatus
                        };
                        prevCorner = Some(MazePiece::CornerBottomRight);
                        aux
                    }
                    _ => panic!("How the f"),
                };
                // filledStatus = !filledStatus;
            } else {
                // either in or out
                filledIntMazeMap[y].push(FillMazePiece {
                    mazePiece: mazePiece.clone(),
                    fill: Some(filledStatus),
                });
            }
        }
    }

    for l in filledIntMazeMap.iter() {
        println!("{}", FillMazeLine(l));
    }

    let p2 = filledIntMazeMap
        .iter()
        .flatten()
        .filter(|el| el.fill.is_some_and(|fill| fill == Fill::Filled))
        .count();

    return p2.try_into().unwrap();
}

fn main() {
    // let lines = parseFile("./square.txt");
    // let lines = parseFile("./test.txt");
    // let lines = parseFile("./test2.txt");
    let lines = parseFile("./input.txt");
    // let lines = parseFile("./visualizedMaze.txt");

    // for line in lines.iter() {
    //     println!("{}", MazeLine(line));
    // }

    let p1 = part1(&lines);
    let p2 = part2(&lines);
    println!("P1: {}", p1);
    println!("P2: {}", p2);
}
