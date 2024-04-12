#include <iostream>
#include <fstream>
#include <vector>
#include "lnfa.h"
#include "pratt_parser.h"

int main()
{
    std::ifstream input("input.txt");
    int N, M, S, nrF;
    std::vector<int> new_state_ids, final_state_ids;
    std::vector<std::tuple<int, int, char>> new_arcs;
    lnfa::LNFA automata = lnfa::LNFA();

    input >> N;
    for (int i = 0; i < N; ++i)
    {
        int id;
        input >> id;
        new_state_ids.emplace_back(id);
    }

    automata.set_lambda('.');

    input >> M;
    for (int i = 0; i < M; ++i)
    {
        int source_id, destination_id;
        char letter;
        input >> source_id >> destination_id >> letter;
        new_arcs.emplace_back(source_id, destination_id, letter);
    }

    input >> S;

    input >> nrF;
    for (int i = 0; i < nrF; ++i)
    {
        int id;
        input >> id;
        final_state_ids.emplace_back(id);
    }

    automata.add_states(new_state_ids);
    automata.add_arcs(new_arcs);
    automata.set_initial_state_id(S);
    automata.set_final_states(final_state_ids);

    std::cout << automata.get_type() << std::endl;
    lnfa::LNFA dfa = lnfa::nfa2dfa(automata);
    std::cout << dfa.get_type() << std::endl;

    parser::S_expression expr("(ab*)|a*b(ca)*");
    std::cout << expr.get_expr_str() << std::endl;
    lnfa::LNFA automata2 = lnfa::build_from_S_expr(expr.get_expr_vector());
    std::cout << automata2.test_acceptance("aaaabca") << std::endl;
    return 0;
}
