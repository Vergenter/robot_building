Cost to make robot:
1. ore robot - 4 ore
2. clay robot - 2 ore
3. obsidian robot - 3 ore 14 clay
4. geode robot - 2 ore 7 obsidian

Start with one ore robot
Can build up to 1 robot per turn
Maximize geodes for turn n

Notes:
1 acion per turn
acion space per turn is 5
endgame is building geode robot per each turn so 7 obsidian robots and 2 ore robots needed
## minimal time to create geode?
0. start
2. build clay robot
4. build clay robot; has 2c
6. build clay robot; has 6c
9. build obsidian robot; has 1c
14. build obsidian robot; 2r 5o 1c
15. build geode robot; 1r 4c
16. first geode
vs
0. start
4. build ore robot
5. build clay robot
6. build clay robot; 1c
7. build clay robot; 3c
8. build clay robot; 6c
9. build clay robot; 10c
11. build obisidian robot; 1r 1c
12. build clay robot; 1r 6c 1o
13. build clay robot; 1r 12c 2o
14. build obsidian robot; ! TOO SLOW
vs
0. start
4. build ore robot
6. build ore robot
7. build clay robot; 1r
8. build clay robot 2r 1c
9. build clay robot 3r 3c
10. build clay robot 4r 6c
11. build clay robot 5r 10c
12. build obsidian robot 5r 1c
13. build clay robot; 6r 6c 1o
14. still too slow
vs 
0. start
2. crobot
6. rrobot; 4c
7. crobot; 5c
8. crobot; 7c
9. crobot; 10c
10. idle; 2r 14c
11. orobot; 1r 4c
12. crobot; 1r 8c 1o
13. crobot; 1r 13c 2o
14. orobot; 0r 5c 3o
15. TOO slow
## maximal time to create second geode robot? It is 7 turns
## There is place for maximization in every step where only undominated solution go further



1. ore robot - 4 ore
2. clay robot - 2 ore
3. obsidian robot - 3 ore 14 clay
4. geode robot - 2 ore 7 obsidian


1 ore is 1/4 ore production
1 ore is 1/2 clay production
1 ore is 1/27 obsidian production


maximizing g from geode
probably by maximizing dg from delta geode


### subproblem to solve:
for state x and turn y find how many geode could it maximally have on turn n

### moves from end
0. build geode robot after this all you build only geode robots
1. 7nth obsidian robot 

obsidian is resource only for geode robots
building first and second obsidian miner gives same value
but building thris is half, 4 1/3 5th 1/4 6th 1/5 and 7th 1/6

True actions are building geode robots
obsidian robot effect is 1/0...1/6 przyśpieszenie produkcji geody
clay robot przyśpiesza przyśpieszenie produkcji geody, więc jest zależny od ilości obsydianu
clay 1/0...1/13
ore przyśpiesza produkcje geody i przyśpiesza przyśpieszania i przyśpiesza przyśpieszanie przyśpieszania

clay robot może być robiony zamist geody, kiedy nie można jej wykonać


geode 
geode acceleration
obsidian geode acceleration acceleration constrain time for geode acceleration
clay geode acceleration acceleration acceleration constrain time for geode acceleration and geode acceleration acceleration
ore is geode acceleration, acceleration acceleration, acceleration acceleration acceleration, acceleration acceleration acceleration acceleration

Whole idea can be simplified to creation of plan that can be improved
Do we know if there is possible improvement after two actions but not after one? So local optima


Another idea
check all clay and obsidian combination
improve using ore
improve using geode


improvement of action 

### Limits on robots
There is maximally needed 3 ore robots and only temporarly
3 ore robots are needed only for fast obsidian construction

3rd ore robots cost 2 turns
waiting 2 turns gives 4 ores which is enough for building 4 obsidian 
4 turns 4 obsidian robots
8 turns 4 obsidianr robots
4 turns 4 obsidian robots
then 3rd ore robots cost 2 turns but spares 4 turns
there is chance that clay will be lacking hmm


### predict future moves
building other robots than geode robot
brings income only after future building of robots
so income from building robot should be calculated 
There should be plan to build geode robots
it should be improved by 



1. ore robot - 4 ore
2. clay robot - 2 ore
3. obsidian robot - 3 ore 14 clay
4. geode robot - 2 ore 7 obsidian
cost * time
4. geode robot - 2 ore (3 ore [2 ore]x14_time )*7_time

ot
ct
bt
gt

solution is binary matrix 5xn with only one 1 in dimension 5 or vector of values between 0 and 4
I was thinking to define cost by row, but it is not that simple as count of idle matter
I maximize geode robot sum of inverse index