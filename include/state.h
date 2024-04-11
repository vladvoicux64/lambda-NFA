#ifndef LAMBDA_NFA_STATE_H
#define LAMBDA_NFA_STATE_H

#include "unordered_map"
#include "vector"
#include <cstddef>
#include "set"

namespace lnfa
{
    class State {
    private:
        std::unordered_map<char, std::vector<State *>> outgoing_arcs_;
        std::set<size_t> log_;
        bool is_final_ = false;
    public:
        void set_final();

        bool check_final() const;

        bool reached_loop(const size_t &index) const;

        void clear_log();

        void add_outgoing_arc(char letter, State *destination);

        std::vector<State *> propagate(char letter);

        void log_pass(const size_t &string_index);
    };
}

#endif //LAMBDA_NFA_STATE_H
