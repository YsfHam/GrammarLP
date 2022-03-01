#pragma once

#include "utils.hpp"

namespace glp
{

    template <typename TypeEnum, typename TValue>
    struct Token
    {
        TypeEnum Type;
        TValue Value;
    };

    template <typename TypeEnum, typename TValue>
    std::ostream& operator<<(std::ostream& out, const Token<TypeEnum, TValue>& token);

    template <typename TypeEnum, typename TValue>
    class Tokenizer
    {
    private:
        template<typename T, typename V>
        using Func = std::function<T(const std::string&, V&)>;
        
        using ActionFunc = Func<TypeEnum, TValue>;
    public:
        void addPattern(const std::string& pattern, ActionFunc action = nullptr);
        void generateTokens(std::istream& stream);
        const std::vector<Token<TypeEnum, TValue>>& getTokens();
    private:
        struct TokenizerAction
        {
            std::regex Pattern;
            ActionFunc Action;

            TokenizerAction(const std::string& pattern, ActionFunc action) 
                : Pattern(pattern), Action(action)
            {}
        };

    private:
        std::vector<TokenizerAction> m_patterns;
        std::vector<Token<TypeEnum, TValue>> m_tokens;
    };
}

#include "Tokenizer.tpp"