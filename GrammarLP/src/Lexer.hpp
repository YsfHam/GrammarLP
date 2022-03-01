#pragma once

#include "utils.hpp"
#include "Tokenizer.hpp"

namespace glp
{
    template <typename TypeEnum, typename TValue>
    class TokensPtr
    {
    public:
        TokensPtr(Token<TypeEnum, TValue>* base, uint32_t size)
            : m_base(base), m_size(size)
        {}

        TValue operator[](uint32_t index) const
        {
            Token<TypeEnum, TValue>* token = m_base + index;
            return token->Value;
        }
    private:
        Token<TypeEnum, TValue>* m_base;
        uint32_t m_size;
    };
    template <typename TypeEnum, typename TValue>
    class Rule
    {
    public:
        using RuleAction = std::function<TValue(const TokensPtr<TypeEnum, TValue>&)>;
    public:
        Rule(TypeEnum ruleHead, const std::initializer_list<TypeEnum>& ruleBody, RuleAction action);
        bool compareTokens(const std::vector<Token<TypeEnum, TValue>>& tokens, int32_t start, int32_t& end);
        TValue operator()(const TokensPtr<TypeEnum, TValue>& tokenPtr);

        TypeEnum getRuleHead() const { return m_head; }

    private:
        TypeEnum m_head;
        std::vector<TypeEnum> m_body;
        RuleAction m_action;
    };

    template <typename TypeEnum, typename TValue>
    class Lexer
    {
    public:
        Lexer(const std::vector<Token<TypeEnum, TValue>>& tokensList);

        void beginRules(TypeEnum ruleHead);
        void endRules();
        void addRule(const std::initializer_list<TypeEnum>& rule, typename Rule<TypeEnum, TValue>::RuleAction action);

        TValue start();
    private:
        int32_t reduce(std::vector<Token<TypeEnum, TValue>>& tokens, int32_t start);
    private:
        const std::vector<Token<TypeEnum, TValue>>& m_tokensList;

        std::vector<Rule<TypeEnum, TValue>> m_rules;

        bool m_beginRuleDef;
        TypeEnum m_ruleHead;
    };
}

#include "Lexer.tpp"