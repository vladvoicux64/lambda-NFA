//
// Created by vladvoicux64 on 5/22/24.
//

#ifndef LAMBDA_NFA_CFG_H
#define LAMBDA_NFA_CFG_H


#include <unordered_map>
#include <string>
#include <vector>

namespace CFG
{
    class CFG
    {
    private:
        std::unordered_map<std::string, std::vector<std::string>> prods_;
    public:
        explicit CFG(std::unordered_map<std::string, std::vector<std::string>> prods);

        bool verify_word(std::string word);
    };
}



#endif //LAMBDA_NFA_CFG_H
