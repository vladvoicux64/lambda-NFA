#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include "pda.h"
#include "CFG.h"

int main()
{
    std::ifstream input("input2.txt");
    int N, M, S, nrF;
    std::vector<int> new_state_ids, final_state_ids;
    std::vector<std::tuple<int, int, char, std::pair<char, std::string>>> new_arcs;
    pda::pda automata = pda::pda();

    input >> N;
    for (int i = 0; i < N; ++i)
    {
        int id;
        input >> id;
        new_state_ids.emplace_back(id);
    }

    input >> M;
    for (int i = 0; i < M; ++i)
    {
        int source_id, destination_id;
        char letter, pop;
        std::string push;
        input >> source_id >> destination_id >> letter >> pop >> push;
        new_arcs.emplace_back(source_id, destination_id, letter, std::make_pair(pop, push));
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

    std::cout << automata.test_acceptance("aaaabbbb") << std::endl;

    std::ifstream inputcfg("input.txt");
    std::unordered_map<std::string, std::vector<std::string>> prods;
    std::string line;
    while (std::getline(inputcfg, line))
    {
        std::stringstream line_stream(line);
        std::string variable, rule;
        line_stream >> variable;
        while(line_stream >> rule) {
            prods[variable].emplace_back(rule);
        }
    }
    CFG::CFG grammar(prods);
    std::cout << grammar.verify_word("baaba");
    return 0;
}
