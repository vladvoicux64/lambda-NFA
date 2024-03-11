#include "state.h"


void State::set_final()
{
    this->is_final_ = true;
}

bool State::check_final() const
{
    return this->is_final_;
}

bool State::check_loop(const size_t &index) const
{
    return (this->log_.find(index) != log_.end());
}

void State::clear_log()
{
    this->log_.clear();
}

void State::add_outgoing_arc(char letter, State *destination)
{
    this->outgoing_arcs_[letter].emplace_back(destination);
}

std::vector<State*> State::propagate(char letter)
{
    auto arc_with_letter = this->outgoing_arcs_.find(letter);
    if(arc_with_letter != this->outgoing_arcs_.end())
    {
        return arc_with_letter->second;
    }
    else
    {
        return std::vector<State*>{};
    }
}

void State::log_pass(const size_t &string_index)
{
    this->log_.emplace(string_index);
}