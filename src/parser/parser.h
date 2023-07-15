#ifndef HELPY_PARSER_H
#define HELPY_PARSER_H

#include <list>
#include <string>

#include "../lexer/lexer.h"
#include "../utils/command.hpp"

namespace Helpy {
    struct ParserInfo {
        int numArguments;
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
        int findArguments();
        std::string findName();
        std::list<Command> findCommands(int numArguments);

    public:
        ParserInfo execute();
    };
}

#endif //HELPY_PARSER_H
