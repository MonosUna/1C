#include <iostream>
#include <stack>
#include <map>
#include <queue>
#include <algorithm>


// Давайте считать что, карты - числа от 1 до 9
// Так как масти не важны будем считать, что все они одинаковы

// Считаем положением игры - std::vector<std::string>. строка - кодировка кучки.
// (строки для удобства сравнения)

std::pair<std::vector<std::string>, int> swap_card(const std::vector<std::string>& state, int i, int j) {
    char a, b;
    // код символа A больше чем у цифр, значит на него можно что угодно ложить
    if (state[i].size() == 0) {
        a = 'A';
    } else {
        a = state[i].back();
    }
    if (state[j].size() == 0) {
        b = 'A';
    } else {
        b = state[j].back();
    }

    auto new_state = state;
    if (a == b) {
        return {new_state, -1};
    }
    if (a < b) {
        new_state[i].pop_back();
        new_state[j] += a;
        return {new_state, j};
    }
    new_state[j].pop_back();
    new_state[i] += b;
    return {new_state, i};
}

bool can_remove(std::vector<std::string>& state, int i) {
    char need = '1';
    if (state[i].size() < 9) {
        return false;
    }
    for (int j = state[i].size() - 1; need <= '9'; --j) {
        if (state[i][j] != need) {
            return false;
        }
        need++;
    }
    return true;
}

bool is_end(std::vector<std::string>& state) {
    for (int i = 0; i < state.size(); ++i) {
        if (state[i].size() != 0) {
            return false;
        }
    }
    return true;
}


int FindSolution(std::vector<std::string>& start) {
    std::map<std::vector<std::string>, bool> visited_states;
    std::sort(start.begin(), start.end());
    visited_states[start] = true;

    for (int i = 0; i < start.size(); ++i) {
        if (can_remove(start, i)) {
            for (int k = 0; k < 9; ++k) {
                start[i].pop_back();
            }
        }
    }
    if (is_end(start)) {
        return 0;
    }

    std::queue<std::pair<std::vector<std::string>, int>> possible_states;
    
    possible_states.push({start, 0});

    while (!possible_states.empty()) {
        auto state = possible_states.front().first;
        int step = possible_states.front().second;
        possible_states.pop();
        if (visited_states[state] == true) {
            continue;
        }
        visited_states[state] = true;

        for (int i = 0; i < state.size(); ++i) {
            for (int j = i + 1; j < state.size(); ++j) {
                auto new_state_and_pos = swap_card(state, i, j);
                int pos = new_state_and_pos.second;
                if (pos != -1 && can_remove(new_state_and_pos.first, pos)) {
                    new_state_and_pos.first[pos].resize(new_state_and_pos.first[pos].size() - 9);
                }
                if (is_end(new_state_and_pos.first)) {
                    return step + 1;
                }
                std::sort(new_state_and_pos.first.begin(), new_state_and_pos.first.end());
                possible_states.push({new_state_and_pos.first, step + 1});
            }
        }
        std::cout << step << "\n";
    }
    return -1;

}

int main() {
    std::vector<std::string> input(8);
    for (int i = 0; i < 8; ++i) {
        std::cin >> input[i];
    }
    int steps = FindSolution(input);
    std::cout << "Game can be solved for:" << steps << " moves" << '\n';
}
