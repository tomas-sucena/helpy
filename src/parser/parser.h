#ifndef HELPY_PARSER_H
#define HELPY_PARSER_H

#include <list>
#include <string>

#include "../lexer/lexer.h"
#include "../utils/command.hpp"

namespace Helpy {
    struct ParserInfo {
        int numArguments;
        std::string classname, filename;
        std::list<Command> commands;
    };

    class Parser {
        std::list<Token> tokens;

    /* CONSTRUCTOR */
    public:
        explicit Parser(std::list<Token> &tokens);

    /* METHODS */
    private:
        static std::string toSnakeCase(const std::string &string);

        int findArguments();
        std::string findName();
        std::list<Command> findCommands(int numArguments);

    public:
        ParserInfo execute();
    };
}

#endif //HELPY_PARSER_H
