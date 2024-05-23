//
// Created by vladvoicux64 on 5/22/24.
//

#include "CFG.h"


#include <utility>

CFG::CFG::CFG(std::unordered_map<std::string, std::vector<std::string>> prods) : prods_(std::move(prods))
{
}

bool CFG::CFG::verify_word(std::string word)
{
    size_t word_len = word.length();
    std::string dp_table[256][256];
    if(word.empty())
    {
        for(auto &rule : prods_["S"])
        {
            if(rule == "lambda") return true;
            return false;
        }
    }
    for (int i = 0; i < word_len; ++i) {
        for (auto &production: this->prods_) {
            for (auto &rule: production.second) {
                if (rule[0] == word[i] && rule.length() == 1) {
                    dp_table[i][i] += production.first;
                }
            }
        }
    }
    for (int l = 2; l < word_len + 1; ++l) {
        for (int i = 0; i < word_len - l + 1; ++i) {
            size_t section_end = i + l - 1;
            for (int j = i; j < section_end; ++j) {
                for (auto &production: this->prods_) {
                    for (auto &rule: production.second) {
                        if (rule.length() == 2 && dp_table[i][j].find(rule[0]) != std::string::npos &&
                            dp_table[j + 1][section_end].find(rule[1]) != std::string::npos)
                        {
                            dp_table[i][section_end] += production.first;
                        }
                    }
                }
            }
        }
    }
    return dp_table[0][word_len - 1].find('S') != std::string::npos;
}
