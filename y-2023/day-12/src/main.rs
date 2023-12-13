#![allow(non_snake_case)]
// #![allow(dead_code)]
// #![allow(unused_imports)]
use std::{
    collections::{HashMap, VecDeque},
    fmt::Display,
    fs::File,
    io::BufRead,
    io::BufReader,
    iter,
};

#[derive(Debug, PartialEq, Eq, Clone)]
enum GroupType {
    Ok,
    Error,
    Maybe,
}

#[derive(Debug, PartialEq, Eq, Clone)]
struct Group {
    gtype: GroupType,
    val: u16,
}

impl Group {
    fn render(&self) -> String {
        let c = match self.gtype {
            GroupType::Ok => '.',
            GroupType::Error => '#',
            GroupType::Maybe => '?',
        };

        return iter::repeat(c).take(self.val.into()).collect();
    }
}

impl Display for Group {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(f, "{}", self.render())
    }
}

#[derive(Debug, PartialEq, Clone)]
struct Line {
    actual: VecDeque<Group>,
    actualLen: u16,
    errorGroups: VecDeque<u16>,
    minErrorLen: u16,
}

impl Line {
    fn new(actual: VecDeque<Group>, errors: VecDeque<u16>) -> Self {
        let mut retVal = Line {
            actual: actual,
            actualLen: 0,
            errorGroups: errors,
            minErrorLen: 0,
        };
        retVal.mergeGroups();
        retVal.reCalcLens();
        return retVal;
    }

    fn refreshLine(&mut self) {
        self.mergeGroups();
        self.reCalcLens();
    }

    fn reCalcLens(&mut self) {
        let actLen = self.actual.iter().map(|gr| gr.val).sum();
        let errLen = if self.errorGroups.len() > 0 {
            self.errorGroups.iter().map(|e| e + 1).sum::<u16>() - 1
        } else {
            0
        };
        self.actualLen = actLen;
        self.minErrorLen = errLen;
    }

    fn mergeGroups(&mut self) {
        if !(self.actual.len() > 1) {
            return;
        }

        let mut index = 1;
        while index < self.actual.len() {
            if self.actual[index - 1].gtype == self.actual[index].gtype {
                self.actual[index].val += self.actual[index - 1].val;
                self.actual.remove(index - 1);
            } else {
                index += 1;
            }
        }
    }

    fn chop_group_front(&mut self) {
        if let Some(group) = self.actual.front_mut() {
            group.val -= 1;
            if group.val == 0 {
                self.actual.pop_front();
            }
            self.reCalcLens();
        }
    }

    fn is_front_group(&self, gtype: GroupType) -> bool {
        return self.actual.front().map_or(false, |gr| gr.gtype == gtype);
    }

    fn render(&self) -> String {
        let reprStr = self
            .actual
            .iter()
            .fold(String::new(), |acc, el| acc + &el.render());
        let errorGroups = self
            .errorGroups
            .iter()
            .map(|el| el.to_string())
            .collect::<Vec<_>>()
            .join(", ");
        return format!("({} | {})", reprStr, errorGroups);
    }
}

impl Display for Line {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(f, "{}", self.render())
    }
}

fn parseFile(filePath: &str) -> Vec<Line> {
    let file = File::open(filePath).expect("Cannot Open");
    let lines = BufReader::new(file)
        .lines()
        .map(|l| l.expect("Error Line Found"));

    let mut ret_val = vec![];

    for line in lines {
        let mut lineIt = line.split(' ');
        let actualStr = lineIt.next().expect("There is no repr").to_string();

        let lineGroup = actualStr
            .chars()
            .map(|c| match c {
                '.' => GroupType::Ok,
                '#' => GroupType::Error,
                '?' => GroupType::Maybe,
                _ => panic!("Unknown '{}'", c),
            })
            .fold(VecDeque::new(), |mut acc, it| {
                if acc.back().is_none() {
                    acc.push_back(Group { gtype: it, val: 1 });
                } else if acc.back().unwrap().gtype == it {
                    acc.back_mut().unwrap().val += 1;
                } else if acc.back().unwrap().gtype != it {
                    acc.push_back(Group { gtype: it, val: 1 });
                }

                return acc;
            });

        let errGroupStr = lineIt.next().expect("There is no groups").to_string();
        let errGroups = errGroupStr
            .trim()
            .split(',')
            .map(|num| {
                num.parse()
                    .expect(&format!("This should be a number:'{}'", num))
            })
            .collect();

        ret_val.push(Line::new(lineGroup, errGroups));
    }

    // return lines;
    return ret_val;
}

fn countPossibleSolutions(line: Line, memo: &mut HashMap<String, u64>) -> u64 {
    // println!("  testing:{}", line);
    fn recFunc(mut line: Line, memo: &mut HashMap<String, u64>, insideErrGroup: bool) -> u64 {
        if line.actualLen < line.minErrorLen {
            return 0;
        }

        if line.actual.len() == 0 && line.errorGroups.len() == 0 {
            // println!("   ok:{}", line);
            return 1;
        }

        if line.actual.len() == 0 && line.errorGroups.len() != 0 {
            return 0;
        }

        if line.errorGroups.len() == 0 {
            if line.actual.iter().all(|gr| gr.gtype != GroupType::Error) {
                // println!("   ok:{}", line);
                return 1;
            } else {
                return 0;
            }
        }

        // println!("  Line:{}", line);
        let key = line.render();
        if let Some(val) = memo.get(&key) {
            return *val;
        }

        if let Some(group) = line.actual.front() {
            let retVal = match group.gtype {
                GroupType::Ok if insideErrGroup => 0,
                GroupType::Ok => {
                    line.chop_group_front();
                    recFunc(line, memo, false)
                }
                GroupType::Error if group.val > *line.errorGroups.front().unwrap() => 0,
                GroupType::Error => {
                    let mut groupEnded = false;

                    line.chop_group_front();
                    *line.errorGroups.front_mut().unwrap() -= 1;
                    if line.errorGroups.front().unwrap() == &0 {
                        line.errorGroups.pop_front();
                        groupEnded = true;
                    }
                    line.refreshLine();
                    if groupEnded {
                        if line.is_front_group(GroupType::Error) {
                            0
                        } else {
                            line.chop_group_front();
                            line.actual.push_front(Group {
                                gtype: GroupType::Ok,
                                val: 1,
                            });
                            line.refreshLine();
                            recFunc(line, memo, false)
                        }
                    } else {
                        recFunc(line, memo, true)
                    }
                }
                GroupType::Maybe => {
                    line.chop_group_front();
                    let okPosSol = if !insideErrGroup {
                        let mut lOk = line.clone();
                        lOk.actual.push_front(Group {
                            gtype: GroupType::Ok,
                            val: 1,
                        });
                        lOk.refreshLine();
                        recFunc(lOk, memo, insideErrGroup)
                    } else {
                        0
                    };
                    let mut lErr = line.clone();
                    lErr.actual.push_front(Group {
                        gtype: GroupType::Error,
                        val: 1,
                    });
                    lErr.refreshLine();
                    let errPosSol = recFunc(lErr, memo, insideErrGroup);

                    okPosSol + errPosSol
                }
            };

            // println!("   {} -> {}", key, retVal);
            memo.insert(key, retVal);

            return retVal;
        }

        panic!("How the fuck did you end up here boi?");
    }

    recFunc(line, memo, false)
}

fn unfoldLine(line: &Line) -> Line {
    let newActual =
        iter::repeat(line.actual.clone())
            .take(5)
            .fold(VecDeque::new(), |mut acc, mut el| {
                if acc.len() > 0 {
                    acc.push_back(Group {
                        gtype: GroupType::Maybe,
                        val: 1,
                    });
                }
                acc.append(&mut el);

                return acc;
            });
    let newErr =
        iter::repeat(line.errorGroups.clone())
            .take(5)
            .fold(VecDeque::new(), |mut acc, mut el| {
                acc.append(&mut el);
                return acc;
            });

    return Line::new(newActual, newErr);
}

fn main() {
    // let input = parseFile("./test.txt");
    let input = parseFile("./input.txt");

    let memo = HashMap::new();

    let p1: u64 = input
        .iter()
        .map(|line| {
            let val = countPossibleSolutions(line.clone(), &mut memo.clone());
            // println!("[P1] {} -> {}", line, val);
            val
        })
        .sum();

    // println!("");

    let p2: u64 = input
        .iter()
        .map(|line| {
            let unfoldedLine = unfoldLine(line);
            let val = countPossibleSolutions(unfoldedLine.clone(), &mut memo.clone());
            // println!("[P2] {} -> {}", unfoldedLine, val);
            val
        })
        .sum();

    // println!("");
    println!("P1: {}", p1);
    println!("P2: {}", p2);
}
