#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <utility>
#include <iostream>
#include <unordered_set>


namespace glp
{

    class Automat
    {
    public:
        Automat() = default;
        void setTransition(uint32_t s1, uint32_t s2, char c);
        void addFinalState(uint32_t s);

        bool validateString(const std::string& word);

        friend Automat Aunion(const Automat& a1, const Automat& a2);
        friend Automat Aconcat(const Automat& a, const Automat& b);
        friend Automat Astar(const Automat& a);
        friend std::ostream& operator<<(std::ostream& stream, const Automat& a);
    private:
        struct Node
        {
            uint32_t State;
            char TransitionChar;
            Node(uint32_t state, char transitionChar) 
                : State(state), TransitionChar(transitionChar)
            {}

            bool operator==(const Node& other) const
            {
                return State == other.State && TransitionChar == other.TransitionChar;
            }
        };
        struct NodeHash
        {
            std::size_t operator()(const Node& node) const noexcept
            {
                std::size_t h1 = std::hash<uint32_t>{}(node.State);
                std::size_t h2 = std::hash<char>{}(node.TransitionChar);
                return h1 ^ (h2 << 1);
            }
        };
    private:
        std::unordered_set<uint32_t> m_finalStates;

        std::unordered_map<Node, uint32_t, NodeHash> m_transitions;
    };
}