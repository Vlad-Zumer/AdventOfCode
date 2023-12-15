#![allow(non_snake_case)]
// #![allow(dead_code)]
// #![allow(unused_imports)]
use std::{
    fs::File,
    io::BufReader,
    io::Read,
};

#[derive(Debug, PartialEq, Eq, Clone)]
enum ActionType {
    Add(u8),
    Remove,
}

#[derive(Debug, PartialEq, Eq, Clone)]
struct Action(String, ActionType);

#[derive(Debug, PartialEq, Eq, Clone)]
struct Lens(String, u8);

#[derive(Debug, PartialEq, Eq, Clone)]
struct Box(Vec<Lens>);

fn parseFile(filePath: &str) -> Vec<String> {
    let file = File::open(filePath).expect("Cannot Open");
    let lines = BufReader::new(file)
        .bytes()
        .map(|byte| byte.expect("Could not read byte"))
        .filter(|byte| !char::from(*byte).is_control() || !char::from(*byte).is_ascii())
        .collect::<Vec<_>>()
        .split(|byte| *byte == b',')
        .map(|bytes| {
            std::str::from_utf8(bytes)
                .expect("Received bytes were not valid utf-8")
                .to_owned()
        })
        .collect::<Vec<_>>();

    if lines.iter().any(|str| !str.is_ascii()) {
        panic!("Input contains non-ascii value");
    }

    return lines;
}

fn hashAlg1(string: &str) -> u8 {
    string.chars().fold(0, |acc, new| {
        let mut sum = acc as u16 + new as u16;
        sum = (sum * 17) % 256;
        sum as u8
    })
}

fn splitElement(string: &str) -> Action {
    let mut label = String::new();
    let mut doneLabel = false;
    let mut action = None;
    for ch in string.chars() {
        if ch == '-' {
            action = Some(ActionType::Remove);
            doneLabel = true;
            continue;
        }
        if ch == '=' {
            action = Some(ActionType::Add(0));
            doneLabel = true;
            continue;
        }
        if !doneLabel {
            label.push(ch);
        }

        if doneLabel && ch.is_ascii_digit() {
            if let Some(ActionType::Add(val)) = action.as_mut() {
                *val = ch.to_string().parse().unwrap();
            }
        }
    }

    Action(label, action.unwrap())
}

fn applyAction(boxes: &mut [Box; 256], action: Action) {
    let Action(label, actionType) = action;
    let labelHash = hashAlg1(&label);
    let indexOfLens = (&boxes[labelHash as usize])
        .0
        .iter()
        .enumerate()
        .filter(|(_, el)| el.0 == label)
        .map(|(indx, _)| indx)
        .next();

    match actionType {
        ActionType::Remove if indexOfLens.is_some() => {
            boxes[labelHash as usize].0.remove(indexOfLens.unwrap());
        }
        ActionType::Add(focalLen) if indexOfLens.is_some() => {
            boxes[labelHash as usize].0[indexOfLens.unwrap()].1 = focalLen;
        }
        ActionType::Add(focalLen) if !indexOfLens.is_some() => {
            boxes[labelHash as usize].0.push(Lens(label, focalLen));
        }
        _ => (),
    }
}

fn main() {
    // let input = parseFile("./test.txt");
    let input = parseFile("./input.txt");

    let p1: u64 = input
        .iter()
        .map(|lexeme| hashAlg1(&lexeme) as u64)
        .sum::<u64>();

    let p2: u64 = input
        .iter()
        .map(|lexeme| splitElement(&lexeme))
        .fold(vec![Box(vec![]); 256], |acc, action| {
            let mut boxes = acc.try_into().unwrap();
            applyAction(&mut boxes, action);
            Vec::from(boxes)
        })
        .iter()
        .enumerate()
        .fold(0u64, |acc, (boxIndex, el)| {
            let boxPower: u64 =
                el.0.iter()
                    .enumerate()
                    .map(|(lensIndex, lens)| {
                        (((boxIndex + 1) as u64) * (lensIndex + 1) as u64) * (lens.1 as u64)
                    })
                    .sum();
            acc + boxPower
        });

    println!("P1: {}", p1);
    println!("P2: {}", p2);
}
