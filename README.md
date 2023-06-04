# Solution to advent of code 2022 day 19 or similar problem
Cost to make robot:
ore robot - 4 ore
clay robot - 2 ore
obsidian robot - 3 ore 14 clay
geode robot - 2 ore 7 obsidian

each robot produces 1 unit of material per turn
Starts with one ore robot
Can build up to 1 robot per turn, previously specified cost must be paid.
Maximize geodes for turn n
### How to build
```g++ -std=c++20 -march=native -Ofast -g main.cpp -o main```