#pragma once

#include "Automat.hpp"

namespace glp
{
    class Regex
    {
    public:
        Regex(const std::string& regex);

        bool match(const std::string& word);
    private:
        enum class Type
        {
            STRING,
            GROUP,
            OP
        };
        struct RegexToken
        {
            std::string Value;
            Type TokenType;  

            RegexToken(const std::string& value, Type type) : Value(value), TokenType(type){}
        };
    private:
        Automat createAutomat(const std::string& token);
        std::vector<RegexToken> tokenize(const std::string& regex);
        Automat eval(const std::string& regex);
    private:
        Automat m_automat;
    };
}