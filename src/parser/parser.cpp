#include "parser.h"

#include <stdexcept>
#include <unordered_set>
#include <utility>

#include "../utils/utils.hpp"

#define uSet std::unordered_set
#define MAX_ARGUMENTS 8

namespace Helpy {
    Parser::Parser(std::vector<Token> tokens) : tokens(std::move(tokens)), it(this->tokens.begin()) {}

    std::string Parser::parseColor() {
        unsigned line = (it++)->line;

        if (it == tokens.end())
            Utils::printError("No value was assigned to COLOR!", line);
        else if (it->type != TokenType::Word)
            Utils::printError("Unexpected value assigned to COLOR!", line);

        std::string color = (it++)->value;

        // convert the color to uppercase
        for (char &c : color)
            c = (char) toupper(c);

        // verify if the color exists
        uSet<std::string> colors = {"RED", "GREEN", "YELLOW", "BLUE", "PURPLE", "CYAN", "WHITE"};

        if (colors.find(color) == colors.end())
            Utils::printError("Unexpected value assigned to COLOR!", it->line);

        return color;
    }

    std::vector<Command> Parser::parseCommands(unsigned &numArguments) {
        ++it; // skip the COMMANDS token

        std::vector<Command> commands;
        numArguments = 0;

        while (it != tokens.end() && it->type == TokenType::Hyphen) {
            unsigned line = (it++)->line;

            if (it == tokens.end() || it->type != TokenType::Word)
                Utils::printError("Unexpected command!", line);

            Command command;
            int acc = 0;

            while (it != tokens.end() && it->type == TokenType::Word) {
                command.push((it++)->value);
                ++acc;
            }

            if (!numArguments) numArguments = acc;

            if (acc > MAX_ARGUMENTS)
                Utils::printError(std::string("Command with too many arguments - ")
                    + "the maximum number should be " + BOLD + '8' + R_BOLD + '!', it->line);
            else if (acc != numArguments)
                Utils::printError("Not all commands have the same number of arguments - "
                    "they should all have " + std::to_string(numArguments) + '!', it->line);

            // check if there is a description
            if (it->type == TokenType::String)
                command.setDescription((it++)->value);

            commands.push_back(command);
        }

        return commands;
    }

    void Parser::parseDescriptions(std::vector<Command> &commands) {
        unsigned line = (it++)->line;

        if (commands.empty())
            Utils::printError("DESCRIPTIONS cannot appear before COMMANDS!", line);

        auto commandIt = commands.begin();

        while (it != tokens.end() && it->type == TokenType::Hyphen) {
            line = (it++)->line;

            std::string description;

            while (it != tokens.end() && (it->type == TokenType::Word || it->type == TokenType::String)) {
                if (!description.empty()) description += ' ';
                description += (it++)->value;
            }

            // verify if there is a description
            if (description.empty())
                Utils::printError("Unexpected description!", line);

            (commandIt++)->setDescription(description);
        }
    }

    std::string Parser::parseName() {
        std::string name = "Helpy";
        unsigned line = (it++)->line;

        if (it == tokens.end())
            Utils::printWarning("No value was assigned to NAME!", line);
        else if (it->type != TokenType::Word)
            Utils::printWarning("Unexpected value assigned to NAME!", line);
        else
            name = it->value;

        ++it;
        return name;
    }

    ParserInfo Parser::execute() {
        ParserInfo info = {.color = "YELLOW", .classname = "Helpy"};

        while (it != tokens.end()) {
            switch (it->type) {
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
                    Utils::printWarning('\'' + it->value + "' is NOT a valid keyword!", it->line);
                    ++it;
            }
        }

        // verify if there are any commands
        if (info.commands.empty())
            Utils::printError("Could not find COMMANDS!");

        info.filename = Utils::toSnakeCase(info.classname);
        return info;
    }
}
