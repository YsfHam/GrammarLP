namespace glp
{

    template <typename TypeEnum, typename TValue>
    void Tokenizer<TypeEnum, TValue>::addPattern(const std::string& pattern, ActionFunc action)
    {
        m_patterns.emplace_back(pattern, std::move(action));
    }

    template <typename TypeEnum, typename TValue>
    void Tokenizer<TypeEnum, TValue>::generateTokens(std::istream& stream)
    {
        if (!stream)
        {
            std::cerr << "ERROR: stream is not good" << std::endl;
            return;
        }
        std::string buffer;
        uint32_t actionsIndex = 0;
        bool parse = true;
        char lastRead;
        stream.get(lastRead);
        while (parse)
        {
            buffer = lastRead;
            while (std::regex_match(buffer, m_patterns[actionsIndex].Pattern) && !stream.eof())
            {
                if (stream.get(lastRead))
                    buffer += lastRead;
            }
            if (!stream.eof())
                buffer.pop_back();
            if (buffer.empty())
                actionsIndex++;
            else
            {
                auto& action = m_patterns[actionsIndex].Action;
                if (action)
                {
                    Token<TypeEnum, TValue> token;
                    token.Type = m_patterns[actionsIndex].Action(buffer, token.Value);
                    m_tokens.push_back(token);
                }
                actionsIndex = 0;
            }

            parse = actionsIndex < m_patterns.size() && !stream.eof();
        }

        if (!stream.eof())
        {
            std::cerr << "Unexpected char \'" << lastRead  << '\'' << std::endl;
            exit(-1);
        }
    }

    template <typename TypeEnum, typename TValue>
    const std::vector<Token<TypeEnum, TValue>>& Tokenizer<TypeEnum, TValue>::getTokens()
    {
        return m_tokens;
    }

    template <typename TypeEnum, typename TValue>
    std::ostream& operator<<(std::ostream& out, const Token<TypeEnum, TValue>& token)
    {
        out << "Token Type " << token.Type << std::endl;
        out << "Token Value " << token.Value << std::endl;

        return out;
    }
}
