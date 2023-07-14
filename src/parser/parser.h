#ifndef HELPY_PARSER_H
#define HELPY_PARSER_H

#include <list>
#include <string>

#include "../lexer/token.hpp"
#include "command.hpp"

namespace Helpy {
    struct ParserInfo {
        char numArguments;
        std::string name;
        std::list<Command> commands;
    };

    class Parser {
        std::list<Token> tokens;

    /* CONSTRUCTOR */
    public:
        explicit Parser(std::list<Token> &tokens);

    /* METHODS */
    private:
        char findArguments();

    public:
        ParserInfo execute();
    };
}

#endif //HELPY_PARSER_H
