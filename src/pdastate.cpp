#include "pdastate.h"
#include <string>
#include <algorithm>

void pda::State::set_final()
{
    this->is_final_ = true;
}

bool pda::State::check_final() const
{
    return this->is_final_;
}

bool pda::State::reached_loop(const size_t &index) const
{
    return (this->log_.find(index) != log_.end());
}

void pda::State::clear_log()
{
    this->log_.clear();
}

void pda::State::add_outgoing_arc(char letter, State *destination, std::pair<char, std::string> stack_values)
{
    std::reverse(stack_values.second.begin(), stack_values.second.end());
    this->outgoing_arcs_[letter].emplace_back(destination, stack_values);
}

std::vector<std::pair<pda::State *, std::pair<char, std::string>>>
pda::State::propagate(char letter)
{
    auto arc_with_letter = this->outgoing_arcs_.find(letter);
    if (arc_with_letter != this->outgoing_arcs_.end()) {
        std::vector<std::pair<pda::State *, std::pair<char, std::string>>> filtered_propagations;
        for (auto destination: arc_with_letter->second) {
            if (destination.second.first == this->stack_.top() || get<1>(destination).first == '_') {
                destination.first->stack_ = this->stack_;
                if (destination.second.first != '_')
                    destination.first->stack_.pop();
                if (destination.second.second != "_")
                    for (auto character: get<1>(destination).second) {
                        destination.first->stack_.push(character);
                    }
                filtered_propagations.push_back(destination);
            }
        }
        return filtered_propagations;
    } else {
        return std::vector<std::pair<State *, std::pair<char, std::string>>>{};
    }
}

void pda::State::log_pass(const size_t &string_index)
{
    this->log_.emplace(string_index);
}

void pda::State::clear_stack()
{
    while (!this->stack_.empty()) {
        this->stack_.pop();
    }
}

void pda::State::set_initial_stack()
{
    this->stack_.push('$');
}

bool pda::State::stack_empty()
{
    return this->stack_.top() == '$';
}
