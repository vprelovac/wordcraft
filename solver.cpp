// g++ -std=c++11 -O3 -o solver solver.cpp

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_set>
#include <tuple>
#include <chrono>
#include <thread>
#include <mutex>
#include <iomanip>

const int MAX_PATH_LENGTH = 59; // Maximum allowed path length to avoid infinite loops
const int MAX_PATHS_TRAVERSED = 10000000; // Maximum number of paths to traverse to avoid excessive computation

struct Position { // Represents a position in the grid
    int row, col;
    bool operator==(const Position& other) const { // Equality operator for Position
        return row == other.row && col == other.col;
    }


};

namespace std { // Custom hash function for Position to use in unordered_set
    template <>
    struct hash<Position> {
        size_t operator()(const Position& pos) const {
            return hash<int>()(pos.row) ^ hash<int>()(pos.col);
        }
    };
}

struct GameState { // Represents the state of the game at any point
    int level;
    std::string target_sentence;
    std::vector<Position> word_positions;
    std::vector<Position> walls;
    Position grid_size;
    std::vector<std::string> words;

    std::string reversed_target_sentence; // Target sentence in reverse order

    GameState(int lvl, const std::string& sentence, const std::vector<Position>& word_pos, const std::vector<Position>& wall_pos, const Position& grid_sz)
        : level(lvl), target_sentence(sentence), word_positions(word_pos), walls(wall_pos), grid_size(grid_sz), reversed_target_sentence(reverse_sentence(sentence)) {
        std::istringstream iss(sentence);
        std::string word;
        while (iss >> word) {
            words.push_back(word);
        }
    }

    GameState() : level(0), grid_size({0, 0}) {}

    bool is_position_occupied(const Position& pos) const { // Check if a position is occupied by a word or wall
        return std::find(word_positions.begin(), word_positions.end(), pos) != word_positions.end() ||
               std::find(walls.begin(), walls.end(), pos) != walls.end();
    }

    bool is_wall(const Position& pos) const { // Check if a position is occupied by a wall
        return std::find(walls.begin(), walls.end(), pos) != walls.end();
    }

    bool is_out_of_bounds(const Position& pos) const { // Check if a position is out of the grid bounds
        return pos.row < 0 || pos.row >= grid_size.row || pos.col < 0 || pos.col >= grid_size.col;
    }

    void move_word(int word_index, const Position& direction) { // Move a word in the specified direction until it hits a wall or another word
        Position current_position = word_positions[word_index];
        Position new_position = {current_position.row + direction.row, current_position.col + direction.col};
        while (!is_out_of_bounds(new_position) && !is_position_occupied(new_position)) {
            current_position = new_position;
            new_position = {current_position.row + direction.row, current_position.col + direction.col};
        }
        word_positions[word_index] = current_position;
    }

    bool is_solved() const { // Check if the current state matches the target sentence

        auto check_words = [&](const std::vector<std::string>& words) -> bool { // Helper function to check if joined words match the target sentence
            std::string joined_words = join_words(words);
            return joined_words == target_sentence || joined_words == reversed_target_sentence;
        };

        // Check horizontal arrangement of words
        for (int row = 0; row < grid_size.row; ++row) {
            std::vector<std::string> row_words;
            for (int col = 0; col < grid_size.col; ++col) {
                auto it = std::find(word_positions.begin(), word_positions.end(), Position{row, col});
                if (it != word_positions.end()) {
                    row_words.push_back(words[it - word_positions.begin()]);
                } else {
                    if (!row_words.empty()) break;
                }
            }
            if (check_words(row_words)) {
                return true;
            }
        }

        // Check vertical arrangement of words
        for (int col = 0; col < grid_size.col; ++col) {
            std::vector<std::string> col_words;
            for (int row = 0; row < grid_size.row; ++row) {
                auto it = std::find(word_positions.begin(), word_positions.end(), Position{row, col});
                if (it != word_positions.end()) {
                    col_words.push_back(words[it - word_positions.begin()]);
                } else {
                    if (!col_words.empty()) break;
                }
            }
            if (check_words(col_words)) {
                return true;
            }
        }

        return false;
    }

    std::vector<std::vector<Position>> generate_goal_states() const { // Generate all possible goal states for the target sentence
        std::vector<std::vector<Position>> goal_states;
        int sentence_length = words.size();

        // Generate horizontal goal states (both left-to-right and right-to-left)
        for (int row = 0; row < grid_size.row; ++row) {
            for (int col = 0; col <= grid_size.col - sentence_length; ++col) {
                std::vector<Position> goal_state;
                for (int i = 0; i < sentence_length; ++i) {
                    goal_state.push_back({row, col + i});
                }
                goal_states.push_back(goal_state);
                std::reverse(goal_state.begin(), goal_state.end());
                goal_states.push_back(goal_state);
            }
        }

        // Generate vertical goal states (both top-to-bottom and bottom-to-top)
        for (int col = 0; col < grid_size.col; ++col) {
            for (int row = 0; row <= grid_size.row - sentence_length; ++row) {
                std::vector<Position> goal_state;
                for (int i = 0; i < sentence_length; ++i) {
                    goal_state.push_back({row + i, col});
                }
                goal_states.push_back(goal_state);
                std::reverse(goal_state.begin(), goal_state.end());
                goal_states.push_back(goal_state);
            }
        }

        return goal_states;
    }
    std::string join_words(const std::vector<std::string>& words) const { // Join words into a single string with spaces
        std::ostringstream oss;
        bool first = true;
        for (const auto& word : words) {
            if (!first) {
                oss << " ";
            }
            oss << word;
            first = false;
        }
        return oss.str();
    }

    std::string reverse_sentence(const std::string& sentence) const { // Reverse the order of words in a sentence
        std::istringstream iss(sentence);
        std::vector<std::string> words((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());
        std::reverse(words.begin(), words.end());
        return join_words(words);
    }
};

const std::unordered_map<std::string, Position> DIRECTIONS = { // Possible movement directions
    {"up", {-1, 0}},
    {"down", {1, 0}},
    {"left", {0, -1}},
    {"right", {0, 1}}
};

int calculate_possible_positions(const GameState& state) { // Calculate the number of possible positions for the target sentence
    int count = 0;
    int sentence_length = state.words.size();

    // Check horizontal positions for validity
    for (int row = 0; row < state.grid_size.row; ++row) {
        for (int col = 0; col <= state.grid_size.col - sentence_length; ++col) {
            bool valid = true;
            for (int i = 0; i < sentence_length; ++i) {
                if (state.is_wall({row, col + i})) {
                    valid = false;
                    break;
                }
            }
            if (valid) {
                count += 2; // Count both left-to-right and right-to-left
            }
        }
    }

    // Check vertical positions for validity
    for (int col = 0; col < state.grid_size.col; ++col) {
        for (int row = 0; row <= state.grid_size.row - sentence_length; ++row) {
            bool valid = true;
            for (int i = 0; i < sentence_length; ++i) {
                if (state.is_wall({row + i, col})) {
                    valid = false;
                    break;
                }
            }
            if (valid) {
                count += 2; // Count both top-to-bottom and bottom-to-top
            }
        }
    }

    return count;
}

std::vector<GameState> load_level_data(const std::string& csv_file) { // Load level data from a CSV file
    std::vector<GameState> levels;
    std::ifstream file(csv_file);
    std::string line;
    std::getline(file, line); // Skip the header line

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string level_str, sentence, type, row_str, col_str;
        std::getline(iss, level_str, ',');
        std::getline(iss, sentence, ',');
        std::getline(iss, type, ',');
        std::getline(iss, row_str, ',');
        std::getline(iss, col_str, ',');

        int level = std::stoi(level_str);
        int row = std::stoi(row_str);
        int col = std::stoi(col_str);

        if (levels.size() < level) { // Ensure the levels vector is large enough
            levels.emplace_back(level, sentence, std::vector<Position>(), std::vector<Position>(), Position{8, 8});
        }

        if (type.find("Word") != std::string::npos) { // Add word positions
            levels[level - 1].word_positions.push_back({row, col});
        } else if (type.find("Wall") != std::string::npos) { // Add wall positions
            levels[level - 1].walls.push_back({row, col});
        }
    }

    return levels;
}


std::pair<std::vector<std::pair<int, std::string>>, int> solve_game(const GameState& initial_state) { // Solve the game using bidirectional search
    std::queue<std::pair<GameState, std::vector<std::pair<int, std::string>>>> forward_queue; // Queue for forward search
    std::queue<std::pair<GameState, std::vector<std::pair<int, std::string>>>> backward_queue; // Queue for backward search

    struct VectorPositionHash { // Custom hash function for vector of Positions
        std::size_t operator()(const std::vector<Position>& vec) const {
            std::size_t seed = vec.size();
            for (const auto& pos : vec) {
                seed ^= std::hash<int>()(pos.row) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
                seed ^= std::hash<int>()(pos.col) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            }
            return seed;
        }
    };

    std::unordered_set<std::vector<Position>, VectorPositionHash> forward_visited; // Visited positions in forward search
    std::unordered_set<std::vector<Position>, VectorPositionHash> backward_visited; // Visited positions in backward search

    forward_queue.push({initial_state, {}}); // Initialize forward search with the initial state
    forward_visited.insert(initial_state.word_positions);

    auto goal_states = initial_state.generate_goal_states(); // Generate all possible goal states
    for (const auto& goal_state : goal_states) {
        GameState goal_game_state = initial_state;
        goal_game_state.word_positions = goal_state;
        backward_queue.push({goal_game_state, {}}); // Initialize backward search with goal states
        backward_visited.insert(goal_state);
    }

    int paths_traversed = 0; // Counter for the number of paths traversed
    auto start_time = std::chrono::steady_clock::now(); // Start time for overall performance measurement
    auto last_checkpoint_time = start_time; // Start time for the last 100,000 paths

    bool forward = true; // Flag to indicate the direction of search
    while (!forward_queue.empty() && !backward_queue.empty() && paths_traversed < MAX_PATHS_TRAVERSED) {
        paths_traversed++; // Increment the paths traversed counter
        if (paths_traversed % 100000 == 0) { // Periodically print the progress
            auto current_time = std::chrono::steady_clock::now(); // Current time for performance measurement
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - last_checkpoint_time); // Calculate elapsed time for the last 100,000 paths
            double speed = 100000.0 / (duration.count() / 1000.0); // Calculate search speed for the last 100,000 paths
            std::cout << "Level " << initial_state.level << ": Paths traversed: " << paths_traversed 
                      << ", Speed: " << std::fixed << std::setprecision(2) << speed << " paths/sec" << std::endl;
            last_checkpoint_time = current_time; // Update the checkpoint time
        }
        GameState current_state;
        std::vector<std::pair<int, std::string>> path;

        if (!forward_queue.empty()) { // Choose the next state from the forward queue
            current_state = forward_queue.front().first;
            path = forward_queue.front().second;
            forward_queue.pop();
        } else {
            current_state = backward_queue.front().first; // Choose the next state from the backward queue
            path = backward_queue.front().second;
            backward_queue.pop();
            forward = false;
        }

        if (path.size() > MAX_PATH_LENGTH) { // Skip paths that exceed the maximum allowed length
            continue;  // Skip this branch as it exceeds the maximum allowed path length
        }

        auto& visited = forward ? forward_visited : backward_visited; // Choose the appropriate visited set
        auto& queue = forward ? forward_queue : backward_queue; // Choose the appropriate queue

        if (forward) {
            if (backward_visited.count(current_state.word_positions)) { // Check if the current state is in the backward visited set
                std::cout << "Solution found: ";
                for (const auto& move : path) {
                    std::cout << "(" << move.first << ", " << move.second << ") ";
                }
                std::cout << std::endl;
                return {path, paths_traversed};
            }
        } else {
            if (forward_visited.count(current_state.word_positions)) { // Check if the current state is in the forward visited set
                std::cout << "Solution found: ";
                for (const auto& move : path) {
                    std::cout << "(" << move.first << ", " << move.second << ") ";
                }
                std::cout << std::endl;
                return {path, paths_traversed};
            }
        }

        for (int word_index = 0; word_index < current_state.word_positions.size(); ++word_index) { // Try moving each word in all possible directions
            for (const auto& direction_pair : DIRECTIONS) {
                const auto& direction_name = direction_pair.first;
                const auto& direction = direction_pair.second;
                GameState new_state = current_state; // Create a new state by moving the word
                new_state.move_word(word_index, direction);
                if (!visited.count(new_state.word_positions)) { // Check if the new state has been visited
                    visited.insert(new_state.word_positions); // Mark the new state as visited
                    auto new_path = path;
                    new_path.emplace_back(word_index, direction_name);
                    queue.push({new_state, new_path}); // Add the new state to the queue
                }
            }
        }
    }

    return {{}, paths_traversed}; // Return an empty solution if no solution is found
}

int main() { // Main function to load levels and solve them
    std::string csv_file = "import";
    Position grid_size = {8, 8}; // Assuming a grid size of 8x8
    auto levels = load_level_data(csv_file); // Load level data from the CSV file

    for (const auto& level_data : levels) {
        int possible_positions = calculate_possible_positions(level_data); // Calculate possible positions for the target sentence
        std::cout << "Solving Level " << level_data.level << std::endl;
        std::cout << "Possible positions for sentence: " << possible_positions << std::endl;

        auto start = std::chrono::high_resolution_clock::now(); // Start time for performance measurement
        auto result = solve_game(level_data); // Solve the game for the current level
        auto solution = result.first; // Extract the solution path
        auto paths_traversed = result.second;
        auto end = std::chrono::high_resolution_clock::now(); // End time for performance measurement
        std::chrono::duration<double> time_taken = end - start; // Calculate the time taken to solve the level

        if (!solution.empty()) { // If a solution is found, print it
            std::cout << "Solution for Level " << level_data.level << ": ";
            for (const auto& move : solution) {
                std::cout << "(" << move.first << ", " << move.second << ") ";
            }
            std::cout << std::endl;
            std::cout << "Minimum moves for Level " << level_data.level << ": " << solution.size() << std::endl; // Print the number of moves
            std::cout << "Time taken for Level " << level_data.level << ": " << time_taken.count() << " seconds" << std::endl; // Print the time taken
            std::cout << "Paths traversed for Level " << level_data.level << ": " << paths_traversed << std::endl; // Print the number of paths traversed
        } else {
            std::cout << "No solution found for Level " << level_data.level << std::endl; // If no solution is found, print a message
            std::cout << "Paths traversed for Level " << level_data.level << ": " << paths_traversed << std::endl;
        }
        std::cout << std::endl; // Add a blank line between levels for better readability
    }

    return 0; // Return success
}
