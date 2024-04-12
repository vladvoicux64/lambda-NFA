#include "lnfa.h"
#include "stack"
#include "cassert"
#include "map"
#include "set"
#include "queue"
#include "unordered_map"
#include "algorithm"
#include "sstream"
#include "pratt_parser.h"

int lnfa::automata_template::new_state_id = 0;

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

lnfa::LNFA lnfa::nfa2dfa(lnfa::LNFA &nfa)
{
    assert(nfa.get_type() == NONDETERMINISTIC);
    std::vector<char> eng_sigma = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};

    std::vector<int> dfa_state_ids = {0};
    std::vector<std::tuple<int, int, char>> dfa_arcs;
    std::map<std::set<State *>, int> dfa_state_ids_table;
    std::vector<int> dfa_final_state_ids;
    std::queue<std::set<State *>> states_to_transition_from;

    dfa_state_ids_table[{&nfa.states_.at(nfa.initial_state_id_)}] = 0;
    int last_id = 1;
    states_to_transition_from.push({&nfa.states_.at(nfa.initial_state_id_)});

    while (!states_to_transition_from.empty()) {
        int curr_id = dfa_state_ids_table[states_to_transition_from.front()];
        for (const auto &letter: eng_sigma) {
            std::set<State *> new_state;
            bool is_final = false;
            for (const auto &state: states_to_transition_from.front()) {
                if (state->check_final())
                    is_final = true;
                auto state_fragment = state->propagate(letter);
                for (const auto &transitioned_state : state_fragment) {
                    new_state.insert(transitioned_state);
                }
            }
            if (is_final)
                dfa_final_state_ids.emplace_back(curr_id);
            if (!new_state.empty()) {
                if (dfa_state_ids_table.find(new_state) == dfa_state_ids_table.end()) {
                    dfa_state_ids.emplace_back(last_id);
                    dfa_state_ids_table[new_state] = last_id++;
                    states_to_transition_from.push(new_state);
                }
                dfa_arcs.emplace_back(curr_id, dfa_state_ids_table[new_state], letter);
            }
        }
        states_to_transition_from.pop();
    }

    LNFA dfa;
    dfa.add_states(dfa_state_ids);
    dfa.add_arcs(dfa_arcs);
    dfa.set_final_states(dfa_final_state_ids);
    dfa.set_initial_state_id(0);

    return dfa;
}

lnfa::automata_template lnfa::char_automata(char input) {
    if (input == '_') {
        lnfa::automata_template templ;
        templ.initial_state_id = automata_template::new_state_id++;
        templ.new_state_ids.emplace_back(templ.initial_state_id);
        templ.final_state_ids.emplace_back(templ.initial_state_id);
        return templ;
    }
    else {
        lnfa::automata_template templ;
        int state1 = automata_template::new_state_id++;
        int state2 = automata_template::new_state_id++;
        templ.initial_state_id = state1;
        templ.new_state_ids.emplace_back(state1);
        templ.new_state_ids.emplace_back(state2);
        templ.final_state_ids.emplace_back(state2);
        templ.new_arcs.emplace_back(state1, state2, input);
        return templ;
    }
}

lnfa::automata_template lnfa::automata_union(lnfa::automata_template aut1, lnfa::automata_template aut2)
{
    lnfa::automata_template templ;
    templ.new_state_ids.insert(templ.new_state_ids.end(), aut1.new_state_ids.begin(), aut1.new_state_ids.end());
    templ.new_state_ids.insert(templ.new_state_ids.end(), aut2.new_state_ids.begin(), aut2.new_state_ids.end());
    templ.new_arcs.insert(templ.new_arcs.end(), aut1.new_arcs.begin(), aut1.new_arcs.end());
    templ.new_arcs.insert(templ.new_arcs.end(), aut2.new_arcs.begin(), aut2.new_arcs.end());
    templ.final_state_ids.insert(templ.final_state_ids.end(), aut1.final_state_ids.begin(), aut1.final_state_ids.end());
    templ.final_state_ids.insert(templ.final_state_ids.end(), aut2.final_state_ids.begin(), aut2.final_state_ids.end());
    templ.initial_state_id = automata_template::new_state_id++;
    templ.new_arcs.emplace_back(templ.initial_state_id, aut1.initial_state_id, '_');
    templ.new_arcs.emplace_back(templ.initial_state_id, aut2.initial_state_id, '_');
    return templ;
}

lnfa::automata_template lnfa::automata_concat(lnfa::automata_template aut1, lnfa::automata_template aut2)
{
    lnfa::automata_template templ;
    templ.new_state_ids.insert(templ.new_state_ids.end(), aut1.new_state_ids.begin(), aut1.new_state_ids.end());
    templ.new_state_ids.insert(templ.new_state_ids.end(), aut2.new_state_ids.begin(), aut2.new_state_ids.end());
    templ.new_arcs.insert(templ.new_arcs.end(), aut1.new_arcs.begin(), aut1.new_arcs.end());
    templ.new_arcs.insert(templ.new_arcs.end(), aut2.new_arcs.begin(), aut2.new_arcs.end());
    templ.final_state_ids.insert(templ.final_state_ids.end(), aut2.final_state_ids.begin(), aut2.final_state_ids.end());
    templ.initial_state_id = aut1.initial_state_id;
    for (const auto &state : aut1.final_state_ids) {
        templ.new_arcs.emplace_back(state, aut2.initial_state_id, '_');
    }
    return templ;
}

lnfa::automata_template lnfa::automata_star(lnfa::automata_template aut)
{
    lnfa::automata_template templ;
    templ.new_state_ids.insert(templ.new_state_ids.end(), aut.new_state_ids.begin(), aut.new_state_ids.end());
    templ.new_arcs.insert(templ.new_arcs.end(), aut.new_arcs.begin(), aut.new_arcs.end());
    templ.final_state_ids.insert(templ.final_state_ids.end(), aut.final_state_ids.begin(), aut.final_state_ids.end());
    templ.initial_state_id = automata_template::new_state_id++;
    templ.new_state_ids.emplace_back(templ.initial_state_id);
    templ.final_state_ids.emplace_back(templ.initial_state_id);
    templ.new_arcs.emplace_back(templ.initial_state_id, aut.initial_state_id, '_');
    for (const auto &final_state : aut.final_state_ids) {
        templ.new_arcs.emplace_back(final_state, templ.initial_state_id, '_');
    }
    return templ;
}

lnfa::LNFA lnfa::automata_template::get_automata() const
{
    LNFA automata = LNFA();
    automata.add_states(this->new_state_ids);
    automata.add_arcs(this->new_arcs);
    automata.set_initial_state_id(this->initial_state_id);
    automata.set_final_states(this->final_state_ids);

    return automata;
}

lnfa::LNFA lnfa::build_from_S_expr(std::vector<parser::token> input)
{
    std::reverse(input.begin(), input.end());
    std::stack<automata_template> build_stack;
    for (const auto &token : input) {
        if (token.get_type() == parser::ATOM) {
            build_stack.push(char_automata(token.get_character()));
        }
        else {
            switch (token.get_character()) {
                break; case '+':
                {
                    automata_template aut1, aut2, aut;
                    aut1 = build_stack.top();
                    build_stack.pop();
                    aut2 = build_stack.top();
                    build_stack.pop();
                    aut = automata_concat(aut1, aut2);
                    build_stack.push(aut);
                }
                break; case '*':
                {
                    automata_template aut;
                    aut = build_stack.top();
                    build_stack.pop();
                    aut = automata_star(aut);
                    build_stack.push(aut);
                }
                break; case '|':
                {
                    automata_template aut1, aut2, aut;
                    aut1 = build_stack.top();
                    build_stack.pop();
                    aut2 = build_stack.top();
                    build_stack.pop();
                    aut = automata_union(aut1, aut2);
                    build_stack.push(aut);
                }
            }
        }
    }
    return build_stack.top().get_automata();
}



