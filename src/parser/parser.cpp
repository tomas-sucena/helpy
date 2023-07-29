#include "parser.h"

#include <stdexcept>
#include <unordered_set>
#include <utility>

#include "../utils/utils.hpp"

#define uSet std::unordered_set

namespace Helpy {
    Parser::Parser(std::list<Token> tokens) : tokens(std::move(tokens)) {}

    int Parser::findArguments() {
        int numArguments = 3;

        for (auto it = tokens.begin(); it != tokens.end(); ++it) {
            if (it->type != TokenType::ArgumentsKeyword) continue;

            unsigned line = it->line;
            it = tokens.erase(it);

            if (it == tokens.end())
                Utils::printError("No value was assigned to ARGUMENTS!", line);
            else if (it->type != TokenType::Literal)
                Utils::printError("Unexpected value assigned to ARGUMENTS!", line);

            try {
                numArguments = std::stoi(it->value);
            }
            catch (std::invalid_argument const &ex) {
                Utils::printError("The value assigned to ARGUMENTS is NOT a number!", it->line);
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

            unsigned line = it->line;
            it = tokens.erase(it);

            if (it == tokens.end())
                Utils::printError("No value was assigned to NAME!", line);
            else if (it->type != TokenType::Literal)
                Utils::printError("Unexpected value assigned to NAME!", line);

            name = it->value;
            tokens.erase(it);

            break;
        }

        return name;
    }

    std::string Parser::findColor() {
        std::string color = "YELLOW";
        uSet<std::string> colors = {"RED", "GREEN", "YELLOW", "BLUE", "PURPLE", "CYAN", "WHITE"};

        for (auto it = tokens.begin(); it != tokens.end(); ++it) {
            if (it->type != TokenType::ColorKeyword) continue;

            unsigned line = it->line;
            it = tokens.erase(it);

            if (it == tokens.end())
                Utils::printError("No value was assigned to COLOR!", line);
            else if (it->type != TokenType::Literal)
                Utils::printError("Unexpected value assigned to COLOR!", it->line);

            color = it->value;

            // convert the color to uppercase
            for (char &c : color)
                c = (char) toupper(c);

            // verify if the color exists
            if (colors.find(color) == colors.end())
                Utils::printError("Unexpected value assigned to COLOR!", it->line);
        }

        return color;
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
            Utils::printError("Could not find COMMANDS!");

        // get the commands
        std::list<Command> commands;

        while (it != tokens.end() && it->type == TokenType::Hyphen) {
            if ((++it) == tokens.end() || it->type != TokenType::Literal)
                Utils::printError("Unexpected command!", it->line);

            Command command;
            int acc = 0;

            while (it != tokens.end() && it->type == TokenType::Literal) {
                command.push((it++)->value);
                ++acc;
            }

            if (acc > numArguments)
                Utils::printError("Command with too many arguments - "
                    "the maximum number should be " + std::to_string(numArguments) + '!', it->line);
            else if (acc < numArguments)
                Utils::printError("Command with too few arguments - "
                    "the minimum number should be " + std::to_string(numArguments) + '!', it->line);

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
        info.color = findColor();

        info.filename = Utils::toSnakeCase(info.classname);
        return info;
    }
}
