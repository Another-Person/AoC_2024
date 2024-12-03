// Advent of Code 2024 -- Day 3
// Joey Sachtleben
// Licensed under the MIT license. See LICENSE for details.



use std::{env, fs, process};
use regex::Regex;

fn main() {
    let args: Vec<String> = env::args().collect();

    let file_path = get_path_from_args(&args).unwrap_or_else(|err| {
        println!("Problem parsing arguments: {err}");
        process::exit(1);
    });

    println!("Using file path {file_path} ...");

    let input = fs::read_to_string(&file_path).unwrap_or_else(|err| {
        println!("Error reading file: {err}");
        process::exit(1);
    });

    // Part 1
    // Find the multiplication commands
    let re = Regex::new(r"mul\(([0-9]+),([0-9]+)\)").unwrap();
    let mul_commands: Vec<(&str, &str)> = re.captures_iter(input.as_str()).map(|caps| {
        let (_, [num1, num2]) = caps.extract();
        (num1, num2)
    }).collect();

    // Turn them into numbers
    let mul_nums: Vec<(i32, i32)> = mul_commands.iter().map(|&(a, b)|
        (a.parse::<i32>().unwrap(), b.parse::<i32>().unwrap())
    ).collect();

    // Multiply and sum them
    let muls: Vec<i32> = mul_nums.iter().map(|&(a,b)| a*b).collect();
    let sum: i32 = muls.iter().sum();

    println!("Sum: {sum}");

    // Part 2
    // Find all valid commands
    let cmd_re = Regex::new(r"mul\([0-9]+,[0-9]+\)|do\(\)|don't\(\)").unwrap();
    let commands: Vec<&str> = cmd_re.captures_iter(input.as_str()).map(|caps| {
        let raw: (&str, [&str; 0]) = caps.extract();
        let val: &str = raw.0;
        val
    }).collect();

    // Find the enabled mul commands
    let mut enabled = true;
    let mut enabled_mul_commands: Vec<&str> = Vec::new();
    for command in commands.iter() {
        if enabled {
            if command.contains("mul") {
                enabled_mul_commands.push(command);
            }
        }

        if command.eq(&"do()") {
            enabled = true;
        }

        if command.eq(&"don't()") {
            enabled = false;
        }
    }

    // Extract the numbers, multiply, & sum
    let num_re = Regex::new(r"([0-9]+),([0-9]+)").unwrap();
    let enabled_mul_cmds: String = enabled_mul_commands.iter().map(|m| m.to_string()).collect();
    let enabled_mul_sum: i32 = num_re.captures_iter(&enabled_mul_cmds).map(|caps| {
        let (_, [num1, num2])= caps.extract();
        num1.parse::<i32>().unwrap() * num2.parse::<i32>().unwrap()
    }).sum();

    println!("Sum of enabled commands: {enabled_mul_sum}");
}

fn get_path_from_args(args: &[String]) -> Result<String, &'static str> {
    if args.len() > 2 {
        return Err("Too many arguments");
    }

    let mut file_path = "../input.txt";
    if args.len() == 2 {
        file_path = &args[1];
    }

    Ok(file_path.to_string())
}