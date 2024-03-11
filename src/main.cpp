#include "iostream"
#include "fstream"
#include "vector"
#include "lnfa.h"
#include "string"
#

int main()
{
    std::ifstream input("input.txt");
    int N, M, S, nrF, NrCuv;
    std::vector<int> new_state_ids, final_state_ids;
    std::vector<std::tuple<int, int, char>> new_arcs;
    LNFA automata = LNFA();

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

    input >> NrCuv;
    for (int i = 0; i < NrCuv; ++i)
    {
        std::string word;
        input >> word;
        std::cout << "word: " << word << "    acceptance test result: " << automata.test_acceptance(word) << std::endl;
    }


    return 0;
}
