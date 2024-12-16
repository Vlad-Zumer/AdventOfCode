#![allow(non_snake_case)]
// #![allow(dead_code)]
// #![allow(unused_imports)]

use std::{
    cmp::{max, min},
    fs::File,
    io::{BufReader, Read},
    time::Instant,
};

use utils::{algorithms::gcd, nd_vec::Vec3};

type Number = i64;
type Equation = Vec3<Number>;

#[derive(Debug, Clone)]
struct TwoEqSystem {
    eq1: Equation,
    eq2: Equation,
}

#[derive(Debug, Clone)]
enum TwoEqSystemSols {
    None,
    One(Number, Number),
    InfYABC(Number, Number, Number),
}

impl TwoEqSystem {
    fn getResult(&self, x: Number, y: Number) -> (Number, Number) {
        let (A, B) = (self.eq1[0], self.eq1[1]);
        let (M, N) = (self.eq2[0], self.eq2[1]);
        return (A * x + B * y, M * x + N * y);
    }

    fn solve(&self) -> TwoEqSystemSols {
        // Ax+By=C
        // Mx+Ny=P
        let (A, B, C) = (self.eq1[0], self.eq1[1], self.eq1[2]);
        let (M, N, P) = (self.eq2[0], self.eq2[1], self.eq2[2]);
        let det = A * N - B * M;

        if det != 0 {
            // y = (PA-MC)/(AN-BM)
            let y = (P * A - M * C) / det;
            // (C - By) / A
            let x = (C - B * y) / A;

            if self.getResult(x, y) == (C, P) {
                return TwoEqSystemSols::One(x, y);
            } else {
                return TwoEqSystemSols::None;
            }
        }

        let gcd = gcd(A.abs(), M.abs());
        let sc1 = A / gcd;
        let sc2 = M / gcd;
        let c1 = C / sc1;
        let p1 = P / sc2;

        if c1 != p1 {
            return TwoEqSystemSols::None;
        }

        return TwoEqSystemSols::InfYABC(A, B, C);
    }
}

type ParsedInputItem = TwoEqSystem;
type ParsedInput = Vec<ParsedInputItem>;

fn parseFile(filePath: &str) -> ParsedInput {
    let file = File::open(filePath).expect("Cannot Open");
    let mut content = String::new();
    BufReader::new(file)
        .read_to_string(&mut content)
        .expect("Failed to read file");

    let mut items = vec![];

    for lChunks in content
        .lines()
        .filter(|l| !l.is_empty())
        .collect::<Vec<_>>()
        .chunks(3)
    {
        if lChunks.len() != 3 {
            panic!("Expected only chunks of 3 but got:'{:?}'", lChunks);
        }
        let mut item = TwoEqSystem {
            eq1: Equation::zero(),
            eq2: Equation::zero(),
        };

        let buttonAstr = lChunks[0].strip_prefix("Button A: ").expect(&format!(
            "Expected first chunk to be Button A, but got '{}'",
            lChunks[0]
        ));
        let AXYstrs = buttonAstr.split(',').map(|s| s.trim()).collect::<Vec<_>>();
        item.eq1[0] = AXYstrs[0].strip_prefix("X+").unwrap().parse().unwrap();
        item.eq2[0] = AXYstrs[1].strip_prefix("Y+").unwrap().parse().unwrap();

        let buttonBstr = lChunks[1].strip_prefix("Button B: ").expect(&format!(
            "Expected first chunk to be Button B, but got '{}'",
            lChunks[1]
        ));
        let BXYstrs = buttonBstr.split(',').map(|s| s.trim()).collect::<Vec<_>>();
        item.eq1[1] = BXYstrs[0].strip_prefix("X+").unwrap().parse().unwrap();
        item.eq2[1] = BXYstrs[1].strip_prefix("Y+").unwrap().parse().unwrap();

        let prizeStr = lChunks[2].strip_prefix("Prize: ").expect(&format!(
            "Expected first chunk to be Prize, but got '{}'",
            lChunks[2]
        ));
        let prizeXYstrs = prizeStr.split(',').map(|s| s.trim()).collect::<Vec<_>>();
        item.eq1[2] = prizeXYstrs[0].strip_prefix("X=").unwrap().parse().unwrap();
        item.eq2[2] = prizeXYstrs[1].strip_prefix("Y=").unwrap().parse().unwrap();

        items.push(item);
    }

    return items;
}

fn part1(input: &ParsedInput) -> u64 {
    let ret: i64 = input
        .iter()
        .map(|eqSys| match eqSys.solve() {
            TwoEqSystemSols::None => 0,
            TwoEqSystemSols::One(x, y) => x * 3 + y,
            TwoEqSystemSols::InfYABC(a, b, c) => {
                let mut res = 0;
                let maxY = max(eqSys.eq1[2], eqSys.eq2[2]) / min(eqSys.eq1[1], eqSys.eq2[1]);
                for yCandidate in (0..=maxY).rev() {
                    let x = (c - b * yCandidate) / a;
                    if x > 0 && eqSys.getResult(x, yCandidate) == (eqSys.eq1[2], eqSys.eq2[2]) {
                        res = x * 3 + yCandidate;
                        break;
                    }
                }
                res
            }
        })
        .sum();

    return ret.try_into().unwrap();
}

fn part2(input: &ParsedInput) -> u64 {
    // update pos
    let newInput: ParsedInput = input
        .iter()
        .map(|eqSys| {
            let mut cpy = eqSys.clone();
            cpy.eq1[2] += 10000000000000;
            cpy.eq2[2] += 10000000000000;
            return cpy;
        })
        .collect();
    return part1(&newInput);
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
