namespace glp
{
    template <typename TypeEnum, typename TValue>
    Rule<TypeEnum, TValue>::Rule(TypeEnum ruleHead, const std::initializer_list<TypeEnum>& ruleBody, RuleAction action)
        : m_head(ruleHead), m_body(ruleBody), m_action(std::move(action))
    {}

    template <typename TypeEnum, typename TValue>
    bool Rule<TypeEnum, TValue>::compareTokens(const std::vector<Token<TypeEnum, TValue>>& tokens, int32_t start, int32_t& end)
    {
        int32_t tempEnd = start + 1 - m_body.size();
        if (tempEnd < 0)
            return false;
        if (m_body.size() <= start - tempEnd)
            return false;
        end = tempEnd;
        while (start >= end) 
        {
            if (m_body[start - end] != tokens[start].Type)
                return false;
            start--;
        }

        return true;
    }

    template <typename TypeEnum, typename TValue>
    TValue Rule<TypeEnum, TValue>::operator()(const TokensPtr<TypeEnum, TValue>& tokenPtr)
    {
        return m_action(tokenPtr);
    }

    template <typename TypeEnum, typename TValue>
    Lexer<TypeEnum, TValue>::Lexer(const std::vector<Token<TypeEnum, TValue>>& tokensList)
        : m_tokensList(tokensList), m_beginRuleDef(false)
    {}

    template <typename TypeEnum, typename TValue>
    void Lexer<TypeEnum, TValue>::beginRules(TypeEnum ruleHead)
    {
        if (m_beginRuleDef)
        {
            std::cerr << "Please end the definition of the current rules" << std::endl;
            return;
        }

        m_beginRuleDef = true;
        m_ruleHead = ruleHead;
    }

    template <typename TypeEnum, typename TValue>
    void Lexer<TypeEnum, TValue>::endRules()
    {
        m_beginRuleDef = false;
    }

    template <typename TypeEnum, typename TValue>
    void Lexer<TypeEnum, TValue>::addRule(const std::initializer_list<TypeEnum>& rule, typename Rule<TypeEnum, TValue>::RuleAction action)
    {
        if (!m_beginRuleDef)
        {
            std::cerr << "Please begin the definitions rules" << std::endl;
            return;
        }
        m_rules.emplace_back(m_ruleHead, rule, action);
    }

    template <typename TypeEnum, typename TValue>
    TValue Lexer<TypeEnum, TValue>::start()
    {
        std::vector<Token<TypeEnum, TValue>> currentTokens(m_tokensList.size());

        int32_t currentTokenIndex = 0;
        int32_t tokensStackIndex = -1;

        while (currentTokenIndex < m_tokensList.size())
        {
            tokensStackIndex++;
            currentTokens[tokensStackIndex] = m_tokensList[currentTokenIndex++];
            tokensStackIndex = reduce(currentTokens, tokensStackIndex); 
            if (tokensStackIndex == -1)
            {
                std::cerr << "Unexpected position\n";
                exit(-1);
            }
        }
        if (tokensStackIndex > 0)
        {
            std::cerr << "syntaxe error\n";
            exit(0);
        }
        return currentTokens[tokensStackIndex].Value;
    }
    
    template <typename TypeEnum, typename TValue>
    int32_t Lexer<TypeEnum, TValue>::reduce(std::vector<Token<TypeEnum, TValue>>& tokens, int32_t start)
    {
        std::cout << "reduce" << std::endl;
        bool isReducing = true;
        int32_t end = -1;
        while (isReducing)
        {
            isReducing = false;
            for (auto& rule : m_rules)
            {
                if (rule.compareTokens(tokens, start, end))
                {
                    Token<TypeEnum, TValue> t;
                    TokensPtr<TypeEnum, TValue> tptr(&tokens[end], start - end + 1);
                    t.Value = rule(tptr);
                    t.Type = rule.getRuleHead();
                    tokens[end] = t;
                    std::cout << "rule ok\n";
                    std::cout << t << std::endl;
                    isReducing = true;
                    start = end;
                }
            }
        }

        return end;
    }
}