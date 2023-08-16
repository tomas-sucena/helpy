#ifndef HELPY_PARSER_H
#define HELPY_PARSER_H

#include "../lexer/lexer.h"
#include "../utils/command.hpp"

namespace Helpy {
    struct ParserInfo {
        unsigned numArguments;
        std::string color;
        std::string classname;
        std::string filename;
        std::vector<Command> commands;
    };

    class Parser {
        std::list<Token> tokens;

    /* CONSTRUCTOR */
    public:
        explicit Parser(std::list<Token> tokens);

    /* METHODS */
    private:
        std::string parseColor();
        std::vector<Command> parseCommands(unsigned &numArguments);
        void parseDescriptions(std::vector<Command> &commands);
        std::string parseName();

    public:
        ParserInfo execute();
    };
}

#endif //HELPY_PARSER_H
