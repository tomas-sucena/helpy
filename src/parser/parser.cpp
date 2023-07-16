#include "parser.h"

#include <stdexcept>

namespace Helpy {
    Parser::Parser(std::list<Token> &tokens) : tokens(tokens) {}

    std::string Parser::toSnakeCase(const std::string &string) {
        std::string string_;

        for (char c : string) {
            if (std::isupper(c) && !string_.empty())
                string_ += '_';

            string_ += (char) tolower(c);
        }

        return string_;
    }

    int Parser::findArguments() {
        int numArguments = 3;

        for (auto it = tokens.begin(); it != tokens.end(); ++it) {
            if (it->type != TokenType::ArgumentsKeyword) continue;
            it = tokens.erase(it);

            if (it == tokens.end())
                throw std::runtime_error("Error: Could not find ARGUMENTS!");
            else if (it->type != TokenType::Literal)
                throw std::runtime_error("Error: No value was assigned to ARGUMENTS!");

            try {
                numArguments = std::stoi(it->value);
            }
            catch (std::invalid_argument const &ex) {
                throw std::runtime_error("Error: The value assigned to ARGUMENTS is NOT a number!");
            }

            tokens.erase(it);
            break;
        }

        return numArguments;
    }

    std::string Parser::findName() {
        std::string name = "Helpy";

        for (auto it = tokens.begin(); it != tokens.end(); ++it) {
            if (it->type != TokenType::NameKeyword) continue;
            it = tokens.erase(it);

            if (it == tokens.end())
                throw std::runtime_error("Error: No value was assigned to NAME!");
            else if (it->type != TokenType::Literal)
                throw std::runtime_error("Error: Unexpected value assigned to NAME!");

            name = it->value;
            tokens.erase(it);

            break;
        }

        return name;
    }

    std::list<Command> Parser::findCommands(int numArguments) {
        // find the COMMANDS keyword
        auto it = tokens.begin();
        for (; it != tokens.end(); ++it) {
            if (it->type != TokenType::CommandsKeyword) continue;

            it = tokens.erase(it);
            break;
        }

        if (it == tokens.end())
            throw std::runtime_error("Error: Could not find COMMANDS!");

        // get the commands
        std::list<Command> commands;

        while (it != tokens.end() && it->type == TokenType::Hyphen) {
            if ((++it) == tokens.end() || it->type != TokenType::Literal)
                throw std::runtime_error("Error: Unexpected command!");

            Command command;
            int acc = 0;

            while (it != tokens.end() && it->type == TokenType::Literal) {
                command.push((it++)->value);
                ++acc;
            }

            if (acc > numArguments)
                throw std::runtime_error("Error: Command with too many arguments - "
                    "the maximum number should be " + std::to_string(numArguments) + '!');
            else if (acc < numArguments)
                throw std::runtime_error("Error: Command with too few arguments - "
                    "the minimum number should be " + std::to_string(numArguments) + '!');

            commands.push_back(command);
        }

        return commands;
    }

    ParserInfo Parser::execute() {
        ParserInfo info;

        // mandatory
        info.numArguments = findArguments();
        info.commands = findCommands(info.numArguments);

        // optional
        info.classname = findName();
        info.filename = toSnakeCase(info.classname);

        return info;
    }
}
