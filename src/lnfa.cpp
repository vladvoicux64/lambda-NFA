#include "lnfa.h"
#include "stack"


void lnfa::LNFA::set_initial_state_id(int initial_state_id)
{
    this->initial_state_id_ = initial_state_id;
}

void lnfa::LNFA::add_states(const std::vector<int> &new_state_ids)
{
    for (auto const &state_id: new_state_ids)
    {
        this->states_[state_id] = State();
    }
}

void lnfa::LNFA::add_arcs(const std::vector<std::tuple<int, int, char>> &new_arcs)
{
    for (auto const &arc: new_arcs)
    {
        this->states_[std::get<0>(arc)].add_outgoing_arc(std::get<2>(arc), &this->states_[std::get<1>(arc)]);
        if (std::get<2>(arc) == this->lambda_character_)
            this->type = LAMBDANONDETERMINISTIC;
        if ((this->states_[std::get<0>(arc)].propagate(std::get<2>(arc)).size() >= 2) && this->type != LAMBDANONDETERMINISTIC)
            this->type = NONDETERMINISTIC;
    }
}

void lnfa::LNFA::set_final_states(const std::vector<int> &final_state_ids)
{
    for (auto const &state_id: final_state_ids)
    {
        this->states_[state_id].set_final();
    }
}

void lnfa::LNFA::clear_state_logs()
{
    for (auto &state: this->states_)
    {
        state.second.clear_log();
    }
}

void lnfa::LNFA::set_lambda(char lambda)
{
    this->lambda_character_ = lambda;
}

char lnfa::LNFA::get_lambda() const
{
    return this->lambda_character_;
}

bool lnfa::LNFA::test_acceptance(const std::string &word)
{
    std::stack<std::pair<State *, size_t>> dfs_stack;
    size_t word_lenght = word.length();
    dfs_stack.emplace(&this->states_[this->initial_state_id_], 0);
    while (!dfs_stack.empty())
    {
        State *current_state = dfs_stack.top().first;
        size_t current_index = dfs_stack.top().second;
        current_state->log_pass(current_index);
        dfs_stack.pop();
        if (current_state->check_final() && current_index == word_lenght)
        {
            this->clear_state_logs();
            return true;
        }
        for (const auto &state: current_state->propagate(word[current_index]))
        {
            dfs_stack.emplace(state, current_index + 1);
        }
        for (const auto &state: current_state->propagate(this->lambda_character_))
        {
            if (!state->reached_loop(current_index))
                dfs_stack.emplace(state, current_index);
        }
    }
    this->clear_state_logs();
    return false;
}

lnfa::aut_type lnfa::LNFA::get_type() const
{
    return this->type;
}
