#include "pda.h"
#include <stack>
#include <queue>
#include <unordered_map>


void pda::pda::set_initial_state_id(int initial_state_id)
{
    this->initial_state_id_ = initial_state_id;
    this->states_.at(initial_state_id).set_initial_stack();
}

void pda::pda::add_states(const std::vector<int> &new_state_ids)
{
    for (auto const &state_id: new_state_ids) {
        this->states_[state_id] = State();
    }
}

void pda::pda::add_arcs(const std::vector<std::tuple<int, int, char, std::pair<char, std::string>>> &new_arcs)
{
    for (auto const &arc: new_arcs) {
        this->states_[std::get<0>(arc)].add_outgoing_arc(std::get<2>(arc), &this->states_[std::get<1>(arc)],
                                                         std::get<3>(arc));
    }
}

void pda::pda::set_final_states(const std::vector<int> &final_state_ids)
{
    for (auto const &state_id: final_state_ids) {
        this->states_[state_id].set_final();
    }
}

void pda::pda::clear_state_logs()
{
    for (auto &state: this->states_) {
        state.second.clear_log();
        state.second.clear_stack();
    }
}

void pda::pda::set_lambda(char lambda)
{
    this->lambda_character_ = lambda;
}

char pda::pda::get_lambda() const
{
    return this->lambda_character_;
}

bool pda::pda::test_acceptance(const std::string &word)
{
    std::stack<std::pair<State *, size_t>> dfs_stack;
    size_t word_lenght = word.length();
    dfs_stack.emplace(&this->states_[this->initial_state_id_], 0);
    while (!dfs_stack.empty()) {
        State *current_state = dfs_stack.top().first;
        size_t current_index = dfs_stack.top().second;
        current_state->log_pass(current_index);
        dfs_stack.pop();
        if (current_state->check_final() && current_index == word_lenght && current_state->stack_empty()) {
            this->clear_state_logs();
            return true;
        }
        for (const auto &state: current_state->propagate(word[current_index])) {
            dfs_stack.emplace(state.first, current_index + 1);
        }
        for (const auto &state: current_state->propagate(this->lambda_character_)) {
            if (!state.first->reached_loop(current_index))
                dfs_stack.emplace(state.first, current_index);
        }
    }
    this->clear_state_logs();
    return false;
}
