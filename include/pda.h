//
// Created by Voicu Ioan Vladut on 22.05.2024.
//

#ifndef LAMBDA_NFA_PDA_H
#define LAMBDA_NFA_PDA_H


#include "pdastate.h"

namespace pda
{

    class pda
    {
    private:
        std::unordered_map<int, State> states_;
        int initial_state_id_ = 0;
        char lambda_character_ = '_';

        void clear_state_logs();

    public:
        void set_initial_state_id(int initial_state_id);

        void add_states(const std::vector<int> &new_state_ids);

        void add_arcs(const std::vector<std::tuple<int, int, char, std::pair<char, std::string>>> &new_arcs);

        void set_final_states(const std::vector<int> &final_state_ids);

        [[maybe_unused]] void set_lambda(char lambda);

        [[maybe_unused]] char get_lambda() const;

        bool test_acceptance(const std::string &word);
    };

} // pda

#endif //LAMBDA_NFA_PDA_H
