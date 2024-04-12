#ifndef LAMBDA_NFA_PRATT_PARSER_H
#define LAMBDA_NFA_PRATT_PARSER_H

#include <vector>
#include <string>

namespace parser {
    typedef enum token_type {
        ATOM,
        OPERATOR,
        EOF_TOKEN
    } token_type;

    class token {
    private:
        token_type type_;
        char character_;
    public:
        explicit token(char character = 0);
        [[nodiscard]] token_type get_type() const;
        [[nodiscard]] char get_character() const;
    };

    class lexer {
    private:
        std::vector<token> tokens_;
    public:
        explicit lexer(const std::string& input);
        token next();
        token peek();
    };

    std::pair<char, char> binding_power(token op);

    class S_expression {
    private:
        std::vector<token> content_;
        std::vector<token> expr_bp(parser::lexer &temp_lex, char min_bp);
    public:
        explicit S_expression(const std::string& input);
        [[nodiscard]] std::vector<token> get_expr_vector() const;
        [[nodiscard]] std::string get_expr_str() const;
    };
}



#endif //LAMBDA_NFA_PRATT_PARSER_H
