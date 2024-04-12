#ifndef LFA_LAB_LAMBDA_NFA_H
#define LFA_LAB_LAMBDA_NFA_H

#include "state.h"
#include "unordered_map"
#include "vector"
#include "string"
#include "tuple"
#include "pratt_parser.h"

namespace lnfa {
    typedef enum aut_type {
        DETERMINISTIC,
        NONDETERMINISTIC,
        LAMBDANONDETERMINISTIC
    } aut_type;

    class LNFA
    {
    private:
        aut_type type = DETERMINISTIC;
        std::unordered_map<int, State> states_;
        int initial_state_id_ = 0;
        char lambda_character_ = '_';
        void clear_state_logs();
    public:
        void set_initial_state_id(int initial_state_id);
        void add_states(const std::vector<int> &new_state_ids);
        void add_arcs(const std::vector<std::tuple<int, int, char>> &new_arcs);
        void set_final_states(const std::vector<int> &final_state_ids);
        [[maybe_unused]] void set_lambda(char lambda);
        [[maybe_unused]] char get_lambda() const;
        [[maybe_unused]] aut_type get_type() const;
        bool test_acceptance(const std::string &word);

        friend LNFA nfa2dfa(LNFA &nfa);

    };

    class automata_template {
    private:
        static int new_state_id;
        std::vector<int> new_state_ids, final_state_ids;
        std::vector<std::tuple<int, int, char>> new_arcs;
        int initial_state_id;

        friend automata_template char_automata(char input);
        friend automata_template automata_union(automata_template aut1, automata_template aut2);
        friend automata_template automata_concat(automata_template aut1, automata_template aut2);
        friend automata_template automata_star(automata_template aut);
    public:
        [[nodiscard]] LNFA get_automata() const;
    };

    LNFA nfa2dfa(LNFA &nfa);
    LNFA build_from_S_expr(std::vector<parser::token> input);
    automata_template char_automata(char input);
    automata_template automata_union(automata_template aut1, automata_template aut2);
    automata_template automata_concat(automata_template aut1, automata_template aut2);
    automata_template automata_star(automata_template aut);
}

#endif //LFA_LAB_LAMBDA_NFA_H
