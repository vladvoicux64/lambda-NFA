#ifndef LFA_LAB_LAMBDA_NFA_H
#define LFA_LAB_LAMBDA_NFA_H

#include "state.h"
#include "unordered_map"
#include "vector"
#include "string"
#include "tuple"


class LNFA
{
private:
    std::unordered_map<int, State> states_;
    int initial_state_id_ = 0;
    char lambda_character_ = '_';
    void clear_state_logs();
public:
    void set_initial_state_id(int initial_state_id);
    void add_states(const std::vector<int> &new_state_ids);
    void add_arcs(const std::vector<std::tuple<int, int, char>> &new_arcs);
    void set_final_states(const std::vector<int> &final_state_ids);
    void set_lambda(char lambda);
    char get_lambda() const;
    bool test_acceptance(const std::string &word);
};


#endif //LFA_LAB_LAMBDA_NFA_H
