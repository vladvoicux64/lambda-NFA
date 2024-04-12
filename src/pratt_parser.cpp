//
// Created by vladvoicux64 on 4/11/24.
//

#include "pratt_parser.h"

#include "utility"
#include "cctype"
#include "algorithm"
#include "cassert"
#include "sstream"

parser::token::token(char character) : character_(character)
{
    if (isalnum(character) || character == '_') type_ = ATOM;
    else if (character != 0) type_ = OPERATOR;
    else type_ = EOF_TOKEN;
}

parser::token_type parser::token::get_type() const
{
    return this->type_;
}

char parser::token::get_character() const
{
    return this->character_;
}

parser::lexer::lexer(const std::string& input)
{
    for (const auto &character : input) {
        this->tokens_.emplace_back(character);
    }
    std::reverse(this->tokens_.begin(), this->tokens_.end());
}

parser::token parser::lexer::next()
{
    token return_val;
    if (!this->tokens_.empty()) {
        return_val = this->tokens_.back();
        this->tokens_.pop_back();
    }
    else return_val = token();
    return return_val;
}

parser::token parser::lexer::peek()
{
    if (!this->tokens_.empty()) {
        return this->tokens_.back();
    }
    return token();
}

std::pair<char, char> parser::binding_power(token op) {
    assert(op.get_type() == OPERATOR);

    switch (op.get_character()) {
        case '*':
            return std::make_pair('5', 'a');
        case '|':
            return std::make_pair('1', '2');
        case '+':
            return std::make_pair('3', '4');
        default:
            return std::make_pair('a', 'a');
    }
}

std::vector<parser::token> parser::S_expression::expr_bp(parser::lexer &temp_lex, char min_bp)
{
    std::vector<token> lhs = {temp_lex.next()};

    if (lhs[0].get_type() != ATOM){

        assert(lhs[0].get_character() == '(');

        lhs = expr_bp(temp_lex, 0);

        assert(temp_lex.next().get_character() == ')');
    }

    while(true) {
        token op = temp_lex.peek();
        if (op.get_type() == EOF_TOKEN) break;

        //implicit concat
        bool is_implicit_concat = false;
        if(op.get_type() == ATOM) {
            op = token('+');
            is_implicit_concat = true;
        }

        auto power = binding_power(op);
        //postfix
        if (power.first != 'a' && power.second == 'a') {
            if (power.first < min_bp)
                break;
            temp_lex.next();

            lhs.insert(lhs.begin(), op);
            continue;
        }
        //infix
        if (power.first != power.second) {
            if (power.first < min_bp)
                break;
            if (!is_implicit_concat)
                temp_lex.next();
            auto rhs = expr_bp(temp_lex, power.second);
            lhs.insert(lhs.begin(), op);
            lhs.insert(lhs.end(), rhs.begin(), rhs.end());
            continue;
        }
        break;
    }
    return lhs;
}

parser::S_expression::S_expression(const std::string &input) : content_()
{
    lexer temp_lex = lexer(input);
    this->content_ = expr_bp(temp_lex, 0);
}

std::vector<parser::token> parser::S_expression::get_expr_vector() const
{
    return this->content_;
}

std::string parser::S_expression::get_expr_str() const
{
    std::stringstream buff;
    for (const auto &token : content_) {
        buff << token.get_character();
    }
    return buff.str();
}
