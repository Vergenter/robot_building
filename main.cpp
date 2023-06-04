#include <iostream>
#include <array>
#include <unordered_set>
#include <functional>
#include <algorithm>
#include <cstring>
#include <fstream>
const int N = 100;
const uint8_t ORE_SPEED_BOUND = 3;
const uint8_t CLAY_SPEED_BOUND = 10;
const uint8_t OBSIDIAN_SPEED_BOUND = 7;
const uint8_t OBSIDIAN_CLAY_COST = 14;

struct GameState
{
    uint8_t ore;
    uint8_t clay;
    uint8_t obsidian;
    uint16_t geode;
    uint8_t ore_speed : 2;
    uint8_t clay_speed : 4;
    uint8_t obsidian_speed : 3;
    uint16_t geode_speed : 15;

    auto operator>(const GameState &other) const
    {
        // Compare each member to determine domination
        return (((ore > other.ore && other.ore_speed < ORE_SPEED_BOUND && (other.ore_speed == 1 || (other.ore_speed == 2 && other.ore < (OBSIDIAN_SPEED_BOUND - other.obsidian_speed)))) ||
                 (clay > other.clay && other.obsidian_speed < OBSIDIAN_SPEED_BOUND && other.clay < (OBSIDIAN_SPEED_BOUND - other.obsidian_speed) * (OBSIDIAN_CLAY_COST - other.clay_speed) - other.clay) ||
                 (obsidian > other.obsidian && other.obsidian_speed < OBSIDIAN_SPEED_BOUND) ||
                 (geode > other.geode) ||
                 (ore_speed > other.ore_speed && other.ore_speed < ORE_SPEED_BOUND && (other.ore_speed == 1 || (other.ore_speed == 2 && other.obsidian_speed < OBSIDIAN_SPEED_BOUND && other.ore < (OBSIDIAN_SPEED_BOUND - other.obsidian_speed)))) ||
                 (clay_speed > other.clay_speed && other.clay_speed < CLAY_SPEED_BOUND && other.obsidian_speed < OBSIDIAN_SPEED_BOUND) ||
                 (obsidian_speed > other.obsidian_speed && other.obsidian_speed < OBSIDIAN_SPEED_BOUND) ||
                 (geode_speed > other.geode_speed)) &&
                (ore >= other.ore || ore_speed >= ORE_SPEED_BOUND || (ore_speed == 2 && ore >= (OBSIDIAN_SPEED_BOUND - obsidian_speed))) &&
                (clay >= other.clay || obsidian_speed >= OBSIDIAN_SPEED_BOUND || clay >= (OBSIDIAN_SPEED_BOUND - obsidian_speed) * (OBSIDIAN_CLAY_COST - clay_speed) - clay) &&
                (obsidian >= other.obsidian || obsidian_speed >= OBSIDIAN_SPEED_BOUND) &&
                geode >= other.geode &&
                (ore_speed >= other.ore_speed || ore_speed >= ORE_SPEED_BOUND || (ore_speed == 2 && (obsidian_speed >= OBSIDIAN_SPEED_BOUND || ore >= (OBSIDIAN_SPEED_BOUND - obsidian_speed)))) &&
                (clay_speed >= other.clay_speed || clay_speed >= CLAY_SPEED_BOUND || obsidian_speed >= OBSIDIAN_SPEED_BOUND) &&
                (obsidian_speed >= other.obsidian_speed || obsidian_speed >= OBSIDIAN_SPEED_BOUND) &&
                geode_speed >= other.geode_speed);
    }
};
bool operator==(const GameState &state1, const GameState &state2)
{
    return std::memcmp(&state1, &state2, sizeof(GameState)) == 0;
}
struct GameStateHash
{
    std::size_t operator()(const GameState &state) const
    {
        uint64_t packedState = 0;
        packedState |= static_cast<uint64_t>(state.ore);
        packedState |= static_cast<uint64_t>(state.clay) << 8;
        packedState |= static_cast<uint64_t>(state.obsidian) << 16;
        packedState |= static_cast<uint64_t>(state.geode) << 24;
        packedState |= static_cast<uint64_t>(state.ore_speed) << 40;
        packedState |= static_cast<uint64_t>(state.clay_speed) << 42;
        packedState |= static_cast<uint64_t>(state.obsidian_speed) << 46;
        packedState |= static_cast<uint64_t>(state.geode_speed) << 49;

        return std::hash<uint64_t>{}(packedState);
    }
};
std::array<std::unordered_set<GameState, GameStateHash>, N> statesPerIteraions;

void generateNewState(GameState state, int oreCost, int clayCost, int obsidianCost, int geodeCost,
                      int oreSpeedBoost, int claySpeedBoost, int obsidianSpeedBoost, int geodeSpeedBoost,
                      int currentIndex)
{
    if (state.ore >= oreCost && state.clay >= clayCost && state.obsidian >= obsidianCost && state.geode >= geodeCost)
    {
        state.ore -= oreCost;
        state.clay -= clayCost;
        state.obsidian -= obsidianCost;
        state.geode -= geodeCost;
        state.ore_speed = std::min(3, state.ore_speed + oreSpeedBoost);                // Ensures it doesn't exceed the bit-field limit.
        state.clay_speed = std::min(15, state.clay_speed + claySpeedBoost);            // Ensures it doesn't exceed the bit-field limit.
        state.obsidian_speed = std::min(7, state.obsidian_speed + obsidianSpeedBoost); // Ensures it doesn't exceed the bit-field limit.
        state.geode_speed = std::min(32767, state.geode_speed + geodeSpeedBoost);      // Ensures it doesn't exceed the bit-field limit.
        statesPerIteraions[currentIndex + 1].insert(state);
    }
}

void generateGameStates(int currentIndex)
{
    for (const auto &state : statesPerIteraions[currentIndex])
    {
        GameState newState = {
            std::min(static_cast<uint8_t>(255), static_cast<uint8_t>(state.ore + state.ore_speed)),
            std::min(static_cast<uint8_t>(255), static_cast<uint8_t>(state.clay + state.clay_speed)),
            std::min(static_cast<uint8_t>(255), static_cast<uint8_t>(state.obsidian + state.obsidian_speed)),
            std::min(static_cast<uint16_t>(65535), static_cast<uint16_t>(state.geode + state.geode_speed)),
            state.ore_speed, state.clay_speed, state.obsidian_speed, state.geode_speed};
        // idle
        statesPerIteraions[currentIndex + 1].insert(newState);

        generateNewState(newState, 2, 0, 0, 0, 0, 1, 0, 0, currentIndex);  // build clay robot if possible
        generateNewState(newState, 3, 14, 0, 0, 0, 0, 1, 0, currentIndex); // build obsidian robot if possible
        generateNewState(newState, 4, 0, 0, 0, 1, 0, 0, 0, currentIndex);  // build ore robot if possible
        generateNewState(newState, 2, 0, 7, 0, 0, 0, 0, 1, currentIndex);  // build geode robot if possible
    }
}
template <int N>
void saveToCSV(const std::vector<GameState> &bestSolutions)
{
    std::string fileName = "best_solutions2_" + std::to_string(N) + ".csv";
    // Create a CSV file
    std::ofstream csvFile(fileName);

    // Write the CSV header
    csvFile << "ore,clay,obsidian,geode,ore_speed,clay_speed,obsidian_speed,geode_speed\n";

    // Iterate over the bestSolutions vector and write each GameState as a CSV row
    for (const auto &gameState : bestSolutions)
    {
        csvFile << static_cast<int>(gameState.ore) << ","
                << static_cast<int>(gameState.clay) << ","
                << static_cast<int>(gameState.obsidian) << ","
                << static_cast<int>(gameState.geode) << ","
                << static_cast<int>(gameState.ore_speed) << ","
                << static_cast<int>(gameState.clay_speed) << ","
                << static_cast<int>(gameState.obsidian_speed) << ","
                << static_cast<int>(gameState.geode_speed) << "\n";
    }

    // Close the CSV file
    csvFile.close();
}
int main()
{
    auto initialState = GameState({0, 0, 0, 0, 1, 0, 0, 0});
    statesPerIteraions[0].insert(initialState);
    for (int i = 0; i < N - 1; ++i)
    {
        generateGameStates(i);
        std::erase_if(statesPerIteraions[i + 1], [&](const GameState &state)
                      { return std::ranges::any_of(statesPerIteraions[i + 1], [&](const GameState &existingState)
                                                   { return existingState > state; }); });
    }
    auto maxGeode = 0;
    for (int i = 0; i < N; ++i)
    {
        maxGeode = 0;
        size_t countMaxGeode = 0;

        for (const auto &state : statesPerIteraions[i])
        {
            if (state.geode > maxGeode)
            {
                maxGeode = state.geode;
                countMaxGeode = 1; // Reset the count when finding a new maximum
            }
            else if (state.geode == maxGeode)
            {
                countMaxGeode++; // Increment the count when encountering another solution with the same maximum
            }
        }
        std::cout << "Steps " << i << ", Non dominated: " << statesPerIteraions[i].size() << ", Max geode: " << static_cast<int>(maxGeode) << ", Maxes count: " << countMaxGeode << std::endl;
    }
    std::vector<GameState> bestSolutions;

    std::copy_if(statesPerIteraions[N - 1].begin(), statesPerIteraions[N - 1].end(),
                 std::back_inserter(bestSolutions),
                 [maxGeode](const GameState &state)
                 { return state.geode == maxGeode; });
    saveToCSV<N>(bestSolutions);
}