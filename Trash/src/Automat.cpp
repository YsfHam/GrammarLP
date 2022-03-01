#include "Automat.hpp"


#include <algorithm>
namespace glp
{
    void Automat::setTransition(uint32_t s1, uint32_t s2, char c)
    {
        m_transitions[Node(s1, c)] = s2;
    }

    void Automat::addFinalState(uint32_t s)
    {
        m_finalStates.emplace(s);
    }

    bool Automat::validateString(const std::string& word)
    {
        uint32_t currentState = 0;
        for (char c : word)
        {
            auto it = m_transitions.find(Node(currentState, c));
            if (it == m_transitions.end())
                return false;
            currentState = it->second;
        }

        return m_finalStates.find(currentState) != m_finalStates.end();
    }

    Automat Aunion(const Automat& a1, const Automat& a2)
    {
        Automat result;

        for (auto[node, nextState] : a1.m_transitions)
            result.setTransition(node.State, nextState, node.TransitionChar);
        for (uint32_t finalState : a1.m_finalStates)
            result.addFinalState(finalState);

        for (auto[node, nextState] : a2.m_transitions)
            result.setTransition(node.State, nextState, node.TransitionChar);
        for (uint32_t finalState : a2.m_finalStates)
            result.addFinalState(finalState);

        return result;
    }

    Automat Aconcat(const Automat& a, const Automat& b)
    {
        Automat result;
        uint32_t maxBase = 0;
        for (auto[node, nextState] : a.m_transitions)
        {
            result.setTransition(node.State, nextState, node.TransitionChar);

            maxBase = std::max(maxBase, nextState);
        }
        for (auto[node, nextState] : b.m_transitions)
            result.setTransition(node.State + maxBase, nextState + maxBase, node.TransitionChar);

        return result;
    }

    Automat Astar(const Automat& a)
    {
        Automat result;
        for (auto[node, nextState] : a.m_transitions)
        {
            if (a.m_finalStates.find(nextState) != a.m_finalStates.end())
            {
                result.setTransition(node.State, 0, node.TransitionChar);
            }
            else
            {
                result.setTransition(node.State, nextState, node.TransitionChar);
            }
        }
        result.addFinalState(0);

        return result;
    }

    std::ostream& operator<<(std::ostream& stream, const Automat& a)
    {
        for (auto[node, nextState] : a.m_transitions)
            stream << node.State << "--\'" << node.TransitionChar << "\'--" << nextState << std::endl;

        return stream;
    }
}