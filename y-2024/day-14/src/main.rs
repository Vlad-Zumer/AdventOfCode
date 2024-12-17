#![allow(non_snake_case)]
// #![allow(dead_code)]
// #![allow(unused_imports)]

use std::{
    collections::HashSet,
    fmt::Write as _,
    fs::File,
    io::{BufReader, Read, Write as _},
    time::Instant,
};

use utils::nd_vec::Vec2;

type Number = i32;
type Vec2D = Vec2<Number>;

#[derive(Debug, Clone)]
struct Robot {
    pos: Vec2D,
    vel: Vec2D,
}

#[allow(dead_code)]
#[derive(Debug, Clone)]
enum FileType {
    TestFile(String),
    InputFile(String),
}

type Robots = Vec<Robot>;
type ParsedInput = (Robots, Vec2D);

fn parseFile(file: &FileType) -> ParsedInput {
    let (filePath, size) = match file {
        FileType::TestFile(s) => (s, Vec2D::new([11, 7])),
        FileType::InputFile(s) => (s, Vec2D::new([101, 103])),
    };

    let file = File::open(filePath).expect("Cannot Open");
    let mut content = String::new();
    BufReader::new(file)
        .read_to_string(&mut content)
        .expect("Failed to read file");

    let items = content
        .lines()
        .map(|line| {
            let lineSplit = line.split(" ").collect::<Vec<_>>();
            let posStr = lineSplit[0]
                .strip_prefix("p=")
                .expect("Expected first position");
            let xyPos = posStr
                .split(",")
                .map(|s| s.parse::<Number>().unwrap())
                .collect::<Vec<_>>();

            let velStr = lineSplit[1]
                .strip_prefix("v=")
                .expect("Expected first position");
            let xyVel = velStr
                .split(",")
                .map(|s| s.parse::<Number>().unwrap())
                .collect::<Vec<_>>();

            return Robot {
                pos: Vec2D::new([xyPos[0], xyPos[1]]),
                vel: Vec2D::new([xyVel[0], xyVel[1]]),
            };
        })
        .collect::<Vec<_>>();

    return (items, size);
}

fn wrappingMod(n: Number, modulo: Number) -> Number {
    return (n % modulo + modulo) % modulo;
}

fn getPosAfterSeconds(robot: &Robot, secs: Number, size: &Vec2D) -> Vec2D {
    let xPos = wrappingMod(
        (robot.pos[0] % size[0]) + (robot.vel[0] % size[0]) * (secs % size[0]),
        size[0],
    );
    let yPos = wrappingMod(
        (robot.pos[1] % size[1]) + (robot.vel[1] % size[1]) * (secs % size[1]),
        size[1],
    );
    return Vec2D::new([xPos, yPos]);
}

fn part1(input: &ParsedInput) -> u64 {
    let (robots, size) = input;
    let midXY = size / 2;
    let (tl, tr, bl, br) = robots
        .iter()
        .map(|robot| getPosAfterSeconds(robot, 100, size))
        .filter(|pos| pos[0] != midXY[0] && pos[1] != midXY[1])
        .fold((0, 0, 0, 0), |(tl, tr, bl, br), pos| {
            match (pos[0] < midXY[0], pos[1] < midXY[1]) {
                (true, true) => (tl + 1, tr, bl, br),
                (true, false) => (tl, tr, bl + 1, br),
                (false, true) => (tl, tr + 1, bl, br),
                (false, false) => (tl, tr, bl, br + 1),
            }
        });
    return tl * tr * bl * br;
}

fn part2(input: &ParsedInput) -> String {
    let (robots, size) = input;
    let outFilePath = "./out.txt";
    let outFile =
        File::create(outFilePath).expect(&format!("Could not create file {}", outFilePath));

    for secs in 0..(101 * 103) {
        let pos = robots
            .iter()
            .map(|robot| getPosAfterSeconds(robot, secs, size))
            .collect::<HashSet<_>>();

        let mut outStr = String::new();
        writeln!(&mut outStr, "s:{}", secs).unwrap();
        for y in 0..(size[1] as usize) {
            for x in 0..(size[0] as usize) {
                let ch =
                    if !pos.contains(&Vec2D::new([x.try_into().unwrap(), y.try_into().unwrap()])) {
                        "."
                    } else {
                        "#"
                    };
                write!(&mut outStr, "{}", ch).unwrap();
            }
            writeln!(&mut outStr).unwrap();
        }
        writeln!(&mut outStr, "-------------------------").unwrap();
        writeln!(&mut outStr).unwrap();

        write!(&outFile, "{}", outStr).unwrap();
    }

    return format!("Check '{}' file to find the tree", outFilePath);
}

fn main() {
    // let file = FileType::TestFile("./test.txt".to_owned());
    let file = FileType::InputFile("./input.txt".to_owned());
    let input = parseFile(&file);

    {
        let now = Instant::now();
        let p1: u64 = part1(&input);
        println!("[{:.3?}] P1: {}", now.elapsed(), p1);
    }
    {
        let now = Instant::now();
        let p2 = part2(&input);
        println!("[{:.3?}] P2: {}", now.elapsed(), p2);
    }
}
