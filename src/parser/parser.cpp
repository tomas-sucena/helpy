#include "parser.h"

#include <stdexcept>

namespace Helpy {
    Parser::Parser(std::list<Token> &tokens) : tokens(tokens) {}

    char Parser::findArguments() {
        char numArguments;

        for (auto it = tokens.begin(); it != tokens.end(); ++it) {
            if (it->type != TokenType::ArgumentsKeyword) continue;
            it = tokens.erase(it);

            if (it == tokens.end())
                throw std::runtime_error("Error: No value was assigned to ARGUMENTS!");
            else if (it->type != TokenType::Literal)
                throw std::runtime_error("Error: Unexpected literal after ARGUMENTS!");

            try {
                numArguments = (char) std::stoi(it->value);
            }
            catch (std::invalid_argument const &ex) {
                throw std::runtime_error("Error: The value assigned to ARGUMENTS is not a number!");
            }

            tokens.erase(it);
            return numArguments;
        }

        throw std::runtime_error("Error: Could not find ARGUMENTS!");
    }

    ParserInfo Parser::execute() {
        ParserInfo info;

        info.numArguments = findArguments();

        return info;
    }
}
