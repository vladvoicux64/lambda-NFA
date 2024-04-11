#include "state.h"


void lnfa::State::set_final()
{
    this->is_final_ = true;
}

bool lnfa::State::check_final() const
{
    return this->is_final_;
}

bool lnfa::State::reached_loop(const size_t &index) const
{
    return (this->log_.find(index) != log_.end());
}

void lnfa::State::clear_log()
{
    this->log_.clear();
}

void lnfa::State::add_outgoing_arc(char letter, State *destination)
{
    this->outgoing_arcs_[letter].emplace_back(destination);
}

std::vector<lnfa::State *> lnfa::State::propagate(char letter)
{
    auto arc_with_letter = this->outgoing_arcs_.find(letter);
    if (arc_with_letter != this->outgoing_arcs_.end())
    {
        return arc_with_letter->second;
    } else
    {
        return std::vector<State *>{};
    }
}

void lnfa::State::log_pass(const size_t &string_index)
{
    this->log_.emplace(string_index);
}