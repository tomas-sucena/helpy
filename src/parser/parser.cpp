#include "parser.h"

#include <stdexcept>
#include <unordered_set>
#include <utility>

#include "../utils/utils.hpp"

#define uSet std::unordered_set
#define MAX_ARGUMENTS 8

namespace Helpy {
    Parser::Parser(std::list<Token> tokens) : tokens(std::move(tokens)) {}

    std::string Parser::parseColor() {
        auto it = tokens.begin();
        unsigned line = it->line;

        it = tokens.erase(it); // erase the COLOR token

        if (it == tokens.end())
            Utils::printError("No value was assigned to COLOR!", line);
        else if (it->type != TokenType::Word)
            Utils::printError("Unexpected value assigned to COLOR!", line);

        std::string color = it->value;
        tokens.erase(it);

        // convert the color to uppercase
        for (char &c : color)
            c = (char) toupper(c);

        // verify if the color exists
        uSet<std::string> colors = {"RED", "GREEN", "YELLOW", "BLUE", "PURPLE", "CYAN", "WHITE"};

        if (colors.find(color) == colors.end())
            Utils::printError("Unexpected value assigned to COLOR!", it->line);

        return color;
    }

    std::list<Command> Parser::parseCommands(unsigned &numArguments) {
        auto it = tokens.begin();
        it = tokens.erase(it); // erase the COMMANDS token

        std::list<Command> commands;
        numArguments = 0;

        while (it != tokens.end() && it->type == TokenType::Hyphen) {
            unsigned line = it->line;
            it = tokens.erase(it); // erase the HYPHEN token

            if (it == tokens.end() || it->type != TokenType::Word)
                Utils::printError("Unexpected command!", line);

            Command command;
            int acc = 0;

            while (it != tokens.end() && it->type == TokenType::Word) {
                command.push(it->value);
                ++acc;

                it = tokens.erase(it);
            }

            if (!numArguments) numArguments = acc;

            if (acc > MAX_ARGUMENTS)
                Utils::printError(std::string("Command with too many arguments - ")
                    + "the maximum number should be " + BOLD + '8' + R_BOLD + '!', it->line);
            else if (acc != numArguments)
                Utils::printError("Not all commands have the same number of arguments - "
                    "they should all have " + std::to_string(numArguments) + '!', it->line);

            commands.push_back(command);
        }

        return commands;
    }

    void Parser::parseDescriptions(std::list<Command> &commands) {
        auto it = tokens.begin();
        unsigned line = it->line;

        if (commands.empty())
            Utils::printError("DESCRIPTIONS cannot appear before COMMANDS!", line);

        auto commandIt = commands.begin();
        it = tokens.erase(it); // erase the DESCRIPTIONS token

        while (it != tokens.end() && it->type == TokenType::Hyphen) {
            line = it->line;
            it = tokens.erase(it); // erase the HYPHEN token

            std::string description;

            while (it != tokens.end() && (it->type == TokenType::Word || it->type == TokenType::String)) {
                if (!description.empty()) description += ' ';
                description += it->value;

                it = tokens.erase(it);
            }

            // verify if there is a description
            if (description.empty())
                Utils::printError("Unexpected description!", line);

            (commandIt++)->setDescription(description);
        }
    }

    std::string Parser::parseName() {
        auto it = tokens.begin();
        unsigned line = it->line;

        it = tokens.erase(it); // erase the NAME token

        if (it == tokens.end())
            Utils::printError("No value was assigned to NAME!", line);
        else if (it->type != TokenType::Word)
            Utils::printError("Unexpected value assigned to NAME!", line);

        std::string name = it->value;
        tokens.erase(it);

        return name;
    }

    ParserInfo Parser::execute() {
        ParserInfo info = {.color = "YELLOW", .classname = "Helpy"};

        while (!tokens.empty()) {
            Token curr = tokens.front();

            switch (curr.type) {
                // mandatory
                case TokenType::CommandsKeyword:
                    info.commands = parseCommands(info.numArguments);
                    break;

                // optional
                case TokenType::ColorKeyword:
                    info.color = parseColor();
                    break;
                case TokenType::DescriptionsKeyword:
                    parseDescriptions(info.commands);
                    break;
                case TokenType::NameKeyword:
                    info.classname = parseName();
                    break;

                default:
                    Utils::printError('\'' + curr.value + "' is NOT a valid keyword!", curr.line);
            }
        }

        // verify if there are any commands
        if (info.commands.empty())
            Utils::printError("Could not find COMMANDS!");

        info.filename = Utils::toSnakeCase(info.classname);
        return info;
    }
}
