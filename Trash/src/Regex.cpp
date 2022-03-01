#include "Regex.hpp"


namespace glp
{
    Regex::Regex(const std::string& regex)
    {
        createAutomat(regex);
    }

    bool Regex::match(const std::string& word)
    {
        return m_automat.validateString(word);
    }

    Automat Regex::createAutomat(const std::string& token)
    {
        Automat result;
        for (int i = 0; i < token.size(); i++)
            result.setTransition(i, i + 1, token[i]);
        result.addFinalState(token.size());

        return result;
    }
    std::vector<Regex::RegexToken> Regex::tokenize(const std::string& regex)
    {
        std::vector<RegexToken> tokens;
        int index = 0;
        std::string ops = "*+()";
        while (index < regex.size())
        {
            if (regex[index] == '+')
            {
                tokens.emplace_back("+", Type::OP);
                index++;
            }
            else if (regex[index] == '*')
            {
                tokens.emplace_back("*", Type::OP);
                index++;
            }
            else if (regex[index] == '(')
            {
                auto closingBracketIndex = regex.find(')', index + 1);
                tokens.emplace_back(regex.substr(index + 1, closingBracketIndex - index - 1), Type::GROUP);
                index = closingBracketIndex + 1;
            }
            std::string buffer;
            while (ops.find(regex[index]) == std::string::npos && index < regex.size())
            {
                buffer += regex[index];
                index++;
            }
            if (!buffer.empty())
                tokens.emplace_back(buffer, Type::STRING);
        }
        return tokens;
    }
    
    Automat Regex::eval(const std::string& regex)
    {
        std::vector<RegexToken> tokens = tokenize(regex);

        std::vector<std::vector<Automat>> automats;
        int index = 0;
        for (auto& token : tokens)
        {
            if (token.TokenType == Type::OP && token.Value == "+")
                ;
        }
    }

}