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

    let re = Regex::new(r"mul\(([0-9]+),([0-9]+)\)").unwrap();
    let mul_commands: Vec<(&str, &str)> = re.captures_iter(input.as_str()).map(|caps| {
        let (_, [num1, num2]) = caps.extract();
        (num1, num2)
    }).collect();

    let mul_nums: Vec<(i32, i32)> = mul_commands.iter().map(|&(a, b)|
        (a.parse::<i32>().unwrap(), b.parse::<i32>().unwrap())
    ).collect();

    let muls: Vec<i32> = mul_nums.iter().map(|&(a,b)| a*b).collect();
    let sum: i32 = muls.iter().sum();

    println!("Sum: {sum}");

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