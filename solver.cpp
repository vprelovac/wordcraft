// g++ -std=c++17 -O3 -o solver solver.cpp

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_set>
#include <chrono>
#include <thread>
#include <mutex>
#include <iomanip>
#include <queue>
#include <functional>
#include <condition_variable>
#include <memory>
#include <future>
#include <iterator>

const int MAX_PATH_LENGTH = 29; // Maximum allowed path length to avoid infinite loops
const int MAX_PATHS_TRAVERSED = 20000000; // Maximum number of paths to traverse to avoid excessive computation

std::mutex cout_mutex; // Mutex for thread-safe console output

using Position = std::pair<int, int>;

struct GameState { // Represents the state of the game at any point
    int level;
    std::string target_sentence;
    std::vector<Position> word_positions;
    std::vector<Position> walls;
    Position grid_size;
    std::vector<std::string> words;

    std::string reversed_target_sentence; // Target sentence in reverse order

    GameState(int lvl, std::string sentence, std::vector<Position> word_pos, std::vector<Position> wall_pos, Position grid_sz)
        : level(lvl), target_sentence(std::move(sentence)), word_positions(std::move(word_pos)), walls(std::move(wall_pos)), grid_size(grid_sz), reversed_target_sentence(reverse_sentence(target_sentence)) {
        std::istringstream iss(target_sentence);
        std::string word;
        while (iss >> word) {
            words.push_back(std::move(word));
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
        return pos.first < 0 || pos.first >= grid_size.first || pos.second < 0 || pos.second >= grid_size.second;
    }

    void move_word(int word_index, const Position& direction) { // Move a word in the specified direction until it hits a wall or another word
        Position current_position = word_positions[word_index];
        Position new_position = {current_position.first + direction.first, current_position.second + direction.second};
        while (!is_out_of_bounds(new_position) && !is_position_occupied(new_position)) {
            current_position = new_position;
            new_position = {current_position.first + direction.first, current_position.second + direction.second};
        }
        word_positions[word_index] = current_position;
    }

    bool is_solved() const { // Check if the current state matches the target sentence


        // Check horizontal arrangement of words
        for (int row = 0; row < grid_size.first; ++row) {
            std::vector<std::string> row_words;
            for (int col = 0; col < grid_size.second; ++col) {
                auto it = std::find(word_positions.begin(), word_positions.end(), Position{row, col});
                if (it != word_positions.end()) {
                    row_words.push_back(words[it - word_positions.begin()]);
                } else {
                    if (!row_words.empty()) break;
                }
            }
            std::string joined_row_words = join_words(row_words);
            if (joined_row_words == target_sentence || joined_row_words == reversed_target_sentence) {
                return true;
            }
        }

        // Check vertical arrangement of words
        for (int col = 0; col < grid_size.second; ++col) {
            std::vector<std::string> col_words;
            for (int row = 0; row < grid_size.first; ++row) {
                auto it = std::find(word_positions.begin(), word_positions.end(), Position{row, col});
                if (it != word_positions.end()) {
                    col_words.push_back(words[it - word_positions.begin()]);
                } else {
                    if (!col_words.empty()) break;
                }
            }
            std::string joined_col_words = join_words(col_words);
            if (joined_col_words == target_sentence || joined_col_words == reversed_target_sentence) {
                return true;
            }
        }

        return false;
    }

    std::pair<int, std::vector<GameState>> calculate_possible_positions_and_goal_states() const {
        int possible_positions = 0;
        std::vector<GameState> goal_states;
        int sentence_length = words.size();

        // Helper function to check if a sequence of positions is valid (no walls)
        auto is_valid_sequence = [this](const std::vector<Position>& sequence) {
            for (const auto& pos : sequence) {
                if (is_wall(pos)) {
                    return false;
                }
            }
            return true;
        };

        // Check horizontal positions
        for (int row = 0; row < grid_size.first; ++row) {
            for (int col = 0; col <= grid_size.second - sentence_length; ++col) {
                std::vector<Position> goal_state;
                for (int i = 0; i < sentence_length; ++i) {
                    goal_state.push_back({row, col + i});
                }
                if (is_valid_sequence(goal_state)) {
                    possible_positions += 2; // Count both left-to-right and right-to-left
                    GameState forward_state = *this;
                    forward_state.word_positions = goal_state;
                    goal_states.push_back(forward_state);

                    GameState backward_state = *this;
                    std::reverse(goal_state.begin(), goal_state.end());
                    backward_state.word_positions = goal_state;
                    goal_states.push_back(backward_state);
                }
            }
        }

        // Check vertical positions
        for (int col = 0; col < grid_size.second; ++col) {
            for (int row = 0; row <= grid_size.first - sentence_length; ++row) {
                std::vector<Position> goal_state;
                for (int i = 0; i < sentence_length; ++i) {
                    goal_state.push_back({row + i, col});
                }
                if (is_valid_sequence(goal_state)) {
                    possible_positions += 2; // Count both top-to-bottom and bottom-to-top
                    GameState forward_state = *this;
                    forward_state.word_positions = goal_state;
                    goal_states.push_back(forward_state);

                    GameState backward_state = *this;
                    std::reverse(goal_state.begin(), goal_state.end());
                    backward_state.word_positions = goal_state;
                    goal_states.push_back(backward_state);
                }
            }
        }

        return {possible_positions, goal_states};
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

    bool operator==(const GameState& other) const {
        return level == other.level &&
               target_sentence == other.target_sentence &&
               word_positions == other.word_positions &&
               walls == other.walls &&
               grid_size == other.grid_size &&
               words == other.words;
    }
};

#include <array>
#include <queue>
#include <unordered_map>
#include <limits>

// Forward declarations
bool has_realizable_path(const Position& start, const Position& goal, const GameState& state);
bool are_interacting(const Position& pos1, const Position& pos2, const Position& goal1, const Position& goal2);

struct VectorPositionHash {
    std::size_t operator()(const std::vector<Position>& vec) const {
        std::size_t seed = vec.size();
        for (const auto& pos : vec) {
            seed ^= std::hash<int>()(pos.first) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            seed ^= std::hash<int>()(pos.second) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        return seed;
    }
};

const std::array<std::pair<std::string, Position>, 4> DIRECTIONS = {{ // Possible movement directions
    {"up", {-1, 0}},
    {"down", {1, 0}},
    {"left", {0, -1}},
    {"right", {0, 1}}
}};

// Standard heuristic function for A* algorithm
int standard_heuristic(const GameState& state, const GameState& goal_state) {
    int total_distance = 0;
    for (size_t i = 0; i < state.word_positions.size(); ++i) {
        int min_distance = std::numeric_limits<int>::max();
        for (const auto& possible_goal : goal_state.word_positions) {
            int dx = std::abs(state.word_positions[i].first - possible_goal.first);
            int dy = std::abs(state.word_positions[i].second - possible_goal.second);
            min_distance = std::min(min_distance, dx + dy);
        }
        total_distance += min_distance;
    }
    return total_distance;
}

// Goal state count heuristic
int goal_count_heuristic(const GameState& state, const GameState& goal_state) {
    int count = 0;
    for (size_t i = 0; i < state.word_positions.size(); ++i) {
        if (std::find(goal_state.word_positions.begin(), goal_state.word_positions.end(), state.word_positions[i]) != goal_state.word_positions.end()) {
            count++;
        }
    }
    return state.word_positions.size() - count;
}

// Number of Realizable Generalized Paths (NRP) heuristic
int nrp_heuristic(const GameState& state, const GameState& goal_state) {
    int count = 0;
    for (size_t i = 0; i < state.word_positions.size(); ++i) {
        bool has_path = false;
        for (const auto& goal_pos : goal_state.word_positions) {
            if (has_realizable_path(state.word_positions[i], goal_pos, state)) {
                has_path = true;
                break;
            }
        }
        if (has_path) {
            count++;
        }
    }
    return state.word_positions.size() - count;
}

// Linear Conflict heuristic
int linear_conflict_heuristic(const GameState& state, const GameState& goal_state) {
    int conflicts = 0;
    for (size_t i = 0; i < state.word_positions.size(); ++i) {
        for (size_t j = i + 1; j < state.word_positions.size(); ++j) {
            if (state.word_positions[i].first == state.word_positions[j].first &&
                goal_state.word_positions[i].first == goal_state.word_positions[j].first &&
                (state.word_positions[i].second - state.word_positions[j].second) *
                    (goal_state.word_positions[i].second - goal_state.word_positions[j].second) < 0) {
                conflicts += 2;
            } else if (state.word_positions[i].second == state.word_positions[j].second &&
                       goal_state.word_positions[i].second == goal_state.word_positions[j].second &&
                       (state.word_positions[i].first - state.word_positions[j].first) *
                           (goal_state.word_positions[i].first - goal_state.word_positions[j].first) < 0) {
                conflicts += 2;
            }
        }
    }
    return conflicts;
}

// Manhattan Distance with Sliding heuristic
int manhattan_sliding_heuristic(const GameState& state, const GameState& goal_state) {
    int total_distance = 0;
    for (size_t i = 0; i < state.word_positions.size(); ++i) {
        int dx = std::abs(goal_state.word_positions[i].first - state.word_positions[i].first);
        int dy = std::abs(goal_state.word_positions[i].second - state.word_positions[i].second);
        total_distance += std::max(dx, dy);
    }
    return total_distance;
}

// Interaction Cost heuristic
int interaction_cost_heuristic(const GameState& state, const GameState& goal_state) {
    int cost = 0;
    for (size_t i = 0; i < state.word_positions.size(); ++i) {
        for (size_t j = i + 1; j < state.word_positions.size(); ++j) {
            if (are_interacting(state.word_positions[i], state.word_positions[j],
                                goal_state.word_positions[i], goal_state.word_positions[j])) {
                cost++;
            }
        }
    }
    return cost;
}

// Helper function for NRP heuristic
bool has_realizable_path(const Position& start, const Position& goal, const GameState& state) {
    // Simplified check: consider a path realizable if there are no obstacles in the way
    int dx = goal.first - start.first;
    int dy = goal.second - start.second;
    int steps = std::max(std::abs(dx), std::abs(dy));
    for (int i = 1; i <= steps; ++i) {
        int x = start.first + (dx * i) / steps;
        int y = start.second + (dy * i) / steps;
        if (state.is_position_occupied({x, y})) {
            return false;
        }
    }
    return true;
}

// Helper function for Interaction Cost heuristic
bool are_interacting(const Position& pos1, const Position& pos2, const Position& goal1, const Position& goal2) {
    return pos1.first == pos2.first || pos1.second == pos2.second;
}

// Combined heuristic function
int combined_heuristic(const GameState& state, const GameState& goal_state) {
    return std::max({
        standard_heuristic(state, goal_state),
        goal_count_heuristic(state, goal_state),
        nrp_heuristic(state, goal_state),
        linear_conflict_heuristic(state, goal_state),
        manhattan_sliding_heuristic(state, goal_state),
        interaction_cost_heuristic(state, goal_state)
    });
}

// A* algorithm implementation
std::pair<std::vector<std::pair<int, std::string>>, int> solve_game_astar(const GameState& initial_state) {
    struct Node {
        GameState state;
        int g_cost;
        int f_cost;
        double tie_breaker;
        std::vector<std::pair<int, std::string>> path;

        Node(GameState s, int g, int f, double tb, std::vector<std::pair<int, std::string>> p)
            : state(s), g_cost(g), f_cost(f), tie_breaker(tb), path(std::move(p)) {}
    };

    struct CompareNode {
        bool operator()(const Node& lhs, const Node& rhs) const {
            if (lhs.f_cost == rhs.f_cost) {
                return lhs.tie_breaker > rhs.tie_breaker;
            }
            return lhs.f_cost > rhs.f_cost;
        }
    };

    std::priority_queue<Node, std::vector<Node>, CompareNode> open_list;
    std::unordered_map<std::vector<Position>, int, VectorPositionHash> closed_list;

    auto [possible_positions, goal_states] = initial_state.calculate_possible_positions_and_goal_states();
    
    double initial_tie_breaker = 0.0;
    for (size_t i = 0; i < initial_state.word_positions.size(); ++i) {
        initial_tie_breaker += std::abs(initial_state.word_positions[i].first - goal_states[0].word_positions[i].first) +
                               std::abs(initial_state.word_positions[i].second - goal_states[0].word_positions[i].second);
    }
    initial_tie_breaker /= 1000.0; // Scale down the tie-breaker value

    open_list.emplace(initial_state, 0, combined_heuristic(initial_state, goal_states[0]), initial_tie_breaker, std::vector<std::pair<int, std::string>>());

    int paths_traversed = 0;

    while (!open_list.empty() && paths_traversed < MAX_PATHS_TRAVERSED) {
        Node current = open_list.top();
        open_list.pop();
        paths_traversed++;

        if (paths_traversed % 100000 == 0) {
            std::cout << "Level " << initial_state.level << ": Paths traversed: " << paths_traversed << std::endl;
        }

        if (std::find(goal_states.begin(), goal_states.end(), current.state) != goal_states.end()) {
            std::cout << "Solution found: ";
            for (const auto& move : current.path) {
                std::cout << "(" << move.first << ", " << move.second << ") ";
            }
            std::cout << std::endl;
            return {current.path, paths_traversed};
        }

        closed_list[current.state.word_positions] = current.g_cost;

        for (int word_index = 0; word_index < current.state.word_positions.size(); ++word_index) {
            for (const auto& [direction_name, direction] : DIRECTIONS) {
                GameState new_state = current.state;
                new_state.move_word(word_index, direction);

                if (closed_list.find(new_state.word_positions) != closed_list.end()) {
                    continue;
                }

                int new_g_cost = current.g_cost + 1;
                int new_f_cost = new_g_cost + combined_heuristic(new_state, goal_states[0]);

                auto new_path = current.path;
                new_path.emplace_back(word_index, direction_name);

                double new_tie_breaker = 0.0;
                for (size_t i = 0; i < new_state.word_positions.size(); ++i) {
                    new_tie_breaker += std::abs(new_state.word_positions[i].first - goal_states[0].word_positions[i].first) +
                                       std::abs(new_state.word_positions[i].second - goal_states[0].word_positions[i].second);
                }
                new_tie_breaker /= 1000.0; // Scale down the tie-breaker value

                open_list.emplace(new_state, new_g_cost, new_f_cost, new_tie_breaker, std::move(new_path));
            }
        }
    }

    return {{}, paths_traversed};
}

std::vector<std::unique_ptr<GameState>> load_level_data(const std::string& csv_file) { // Load level data from a CSV file
    std::vector<std::unique_ptr<GameState>> levels;
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
            levels.emplace_back(std::make_unique<GameState>(level, std::move(sentence), std::vector<Position>(), std::vector<Position>(), Position{8, 8}));
        }

        if (type.find("Word") != std::string::npos) { // Add word positions
            levels[level - 1]->word_positions.emplace_back(row, col);
        } else if (type.find("Wall") != std::string::npos) { // Add wall positions
            levels[level - 1]->walls.emplace_back(row, col);
        }
    }

    return levels;
}


std::pair<std::vector<std::pair<int, std::string>>, int> solve_game(const GameState& initial_state) {
    struct VectorPositionHash {
        std::size_t operator()(const std::vector<Position>& vec) const {
            std::size_t seed = vec.size();
            for (const auto& pos : vec) {
                seed ^= std::hash<int>()(pos.first) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
                seed ^= std::hash<int>()(pos.second) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            }
            return seed;
        }
    };

    std::queue<GameState> search_queue;
    std::unordered_map<std::vector<Position>, std::vector<std::pair<int, std::string>>, VectorPositionHash> visited;

    search_queue.push(initial_state);
    visited[initial_state.word_positions] = {};

    auto [possible_positions, goal_states] = initial_state.calculate_possible_positions_and_goal_states();
    std::unordered_set<std::vector<Position>, VectorPositionHash> goal_positions;
    for (const auto& goal_state : goal_states) {
        goal_positions.insert(goal_state.word_positions);
    }

    int paths_traversed = 0;
    auto start_time = std::chrono::steady_clock::now();
    auto last_checkpoint_time = start_time;

    while (!search_queue.empty() && paths_traversed < MAX_PATHS_TRAVERSED) {
        paths_traversed++;
        if (paths_traversed % 100000 == 0) {
            auto current_time = std::chrono::steady_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - last_checkpoint_time);
            double speed = 100000.0 / (duration.count() / 1000.0);
            std::cout << "Level " << initial_state.level << ": Paths traversed: " << paths_traversed 
                      << ", Speed: " << std::fixed << std::setprecision(2) << speed << " paths/sec" << std::endl;
            last_checkpoint_time = current_time;
        }

        GameState current_state = search_queue.front();
        search_queue.pop();

        if (goal_positions.count(current_state.word_positions)) {
            auto path = visited[current_state.word_positions];
            std::cout << "Solution found: ";
            for (const auto& move : path) {
                std::cout << "(" << move.first << ", " << move.second << ") ";
            }
            std::cout << std::endl;
            return {path, paths_traversed};
        }

        if (visited[current_state.word_positions].size() >= MAX_PATH_LENGTH) {
            continue;
        }

        for (int word_index = 0; word_index < current_state.word_positions.size(); ++word_index) {
            for (const auto& [direction_name, direction] : DIRECTIONS) {
                GameState new_state = current_state;
                new_state.move_word(word_index, direction);
                if (!visited.count(new_state.word_positions)) {
                    auto new_path = visited[current_state.word_positions];
                    new_path.emplace_back(word_index, direction_name);
                    visited[new_state.word_positions] = new_path;
                    search_queue.push(new_state);
                }
            }
        }
    }

    return {{}, paths_traversed};
}

void solve_level(const GameState& level_data, bool use_astar) {
    auto [possible_positions, goal_states] = level_data.calculate_possible_positions_and_goal_states();
    {
        std::lock_guard<std::mutex> lock(cout_mutex);
        std::cout << "Solving Level " << level_data.level << std::endl;
        std::cout << "Possible positions for sentence: " << possible_positions << std::endl;
    }

    auto start = std::chrono::high_resolution_clock::now();
    std::pair<std::vector<std::pair<int, std::string>>, int> result;
    
    if (use_astar) {
        result = solve_game_astar(level_data);
    } else {
        result = solve_game(level_data);
    }
    
    auto solution = result.first;
    auto paths_traversed = result.second;
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> time_taken = end - start;

    std::lock_guard<std::mutex> lock(cout_mutex);
    if (!solution.empty()) {
        std::cout << "Solution for Level " << level_data.level << " (" << (use_astar ? "A*" : "BFS") << "): ";
        for (const auto& move : solution) {
            std::cout << "(" << level_data.words[move.first] << ", " << move.second << ") ";
        }
        std::cout << std::endl;
        std::cout << "Minimum moves for Level " << level_data.level << ": " << solution.size() << std::endl;
        std::cout << "Time taken for Level " << level_data.level << ": " << time_taken.count() << " seconds" << std::endl;
        std::cout << "Paths traversed for Level " << level_data.level << ": " << paths_traversed << std::endl;
    } else {
        std::cout << "No solution found for Level " << level_data.level << " (" << (use_astar ? "A*" : "BFS") << ")" << std::endl;
        std::cout << "Paths traversed for Level " << level_data.level << ": " << paths_traversed << std::endl;
    }
    std::cout << std::endl;
}


int main(int argc, char* argv[]) {
    std::string csv_file = "import";
    Position grid_size = {8, 8};
    bool use_astar = false;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "a") {
            use_astar = true;
        }
        if (arg == "import2") {
            csv_file = "2";
            grid_size = {10, 10};
        }
    }

    auto levels = load_level_data(csv_file);

    // Update grid size for all levels
    for (auto& level : levels) {
        level->grid_size = grid_size;
    }

    // Use std::async to run solve_level in parallel for each level
    std::vector<std::future<void>> futures;
    for (const auto& level_data : levels) {
        futures.push_back(std::async(std::launch::async, [&level_data, use_astar]() {
            solve_level(*level_data, use_astar);
        }));
    }

    // Wait for all tasks to complete
    for (auto& future : futures) {
        future.get();
    }

    return 0;
}
