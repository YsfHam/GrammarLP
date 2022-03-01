#include <iostream>
#include <regex>
#include <string>
#include <vector>
#include <fstream>

#include "Lexer.hpp"
using namespace glp;

enum class Types
{
    // terminals
    INT,

    // non terminals
    EXP,

    // operators
    PLUS,
    MINUS,
};

std::ostream& operator<<(std::ostream& stream, const Types& types)
{
    switch (types)
    {
        case Types::INT: stream << "int"; break;
        case Types::EXP: stream << "exp"; break;
        case Types::PLUS: stream << "+"; break;

        default: stream << "unknown"; break;
    }

    return stream;
}

auto initTokens()
{
    std::ifstream file("tokenize_test.txt");
    Tokenizer<Types, int32_t> tokenizer;

    tokenizer.addPattern("[1-9][0-9]*", [](auto& text, int32_t& integer){
        integer = atoi(text.c_str());
        return Types::INT;
    });

    tokenizer.addPattern("\\+", [](auto& , auto& ){
        return Types::PLUS;
    });
    tokenizer.addPattern("[-]", [](auto& , auto& ){
        return Types::MINUS;
    });
    tokenizer.addPattern("\\s");

    tokenizer.generateTokens(file);
    auto& tokens = tokenizer.getTokens();

    return tokens;
}

int main() 
{
    auto tokens = initTokens();
    for (auto& token : tokens)
        std::cout << token << std::endl;

    Lexer<Types, int32_t> lexer(tokens);
    lexer.beginRules(Types::EXP);
    lexer.addRule({Types::EXP, Types::PLUS, Types::EXP}, [](auto& ptr){
        std::cout << "Rule1" << std::endl;
        std::cout << ptr[0] << std::endl;
        std::cout << ptr[2] << std::endl;
        return ptr[0] + ptr[2];
    });
    lexer.addRule({Types::EXP, Types::MINUS, Types::EXP}, [](auto& ptr){
        std::cout << "Rule1" << std::endl;
        std::cout << ptr[0] << std::endl;
        std::cout << ptr[2] << std::endl;
        return ptr[0] - ptr[2];
    });
    lexer.addRule({Types::INT}, [](auto& ptr){
         std::cout << "Rule2" << std::endl;
        std::cout << ptr[0] << std::endl;
        return ptr[0];
    });
    lexer.endRules();
    int32_t result = lexer.start();
    std::cout << "lexer final value " << result << std::endl;
    return 0;
}