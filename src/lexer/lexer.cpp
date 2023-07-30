#include "../utils/utils.hpp"
#include "lexer.h"

namespace Helpy {
    uMap<std::string, TokenType> Lexer::keywords = {{"NAME", TokenType::NameKeyword},
                                                    {"COMMANDS", TokenType::CommandsKeyword},
                                                    {"COLOR", TokenType::ColorKeyword},
                                                    {"COLOUR", TokenType::ColorKeyword}};

    Lexer::Lexer(const std::string &path) : file(path), curr(0), line(1), error(false) {}

    void Lexer::read() {
        file.get(curr);
        line += (curr == '\n');
    }

    std::string Lexer::readWord() {
        std::string value;
        bool getNext = true;

        while (!file.eof() && getNext) {
            switch (curr) {
                case 'a' ... 'z' :
                case 'A' ... 'Z' :
                case '0' ... '9' :
                    value += curr;
                    break;
                case ':' :
                case ' ' :
                case '\n' :
                    getNext = false;
                    continue;
                default :
                    Utils::printError(std::string("Unexpected character '") + curr + "'!", line, false);
                    error = true;
            }

            read();
        }

        return value;
    }

    void Lexer::ignoreComment(bool multiline) {
        if (!multiline) {
            while (!file.eof() && curr != '\n')
                read();

            return;
        }

        int initialLine = line;

        while (!file.eof()) {
            read();
            if (file.eof() || curr != '*') continue;

            read();
            if (file.eof() || curr != '/') continue;

            // "*/" found
            break;
        }

        if (file.eof()) {
            Utils::printError(std::string("Badly formatted comment - expected closing '") + BOLD + "*/" 
                + R_BOLD + "' to match opening '" + BOLD + "/*" + R_BOLD + "'!", initialLine, false);
            error = true;
        }
    }

    std::list<Token> Lexer::execute() {
        std::list<Token> tokens;
        int getNext = true;

        while (true) {
            if (getNext++) read();

            // check if there is more to read
            if (file.eof()) break;

            switch (curr) {
                case 'a' ... 'z' :
                case 'A' ... 'Z' :
                case '1' ... '9' : {
                    int initialLine = line;

                    tokens.emplace_back(TokenType::Literal, initialLine, readWord());
                    getNext = false;

                    break;
                }
                case ':' : {
                    Token last = tokens.back();
                    tokens.pop_back();

                    auto it = keywords.find(last.value);

                    if (it == keywords.end()) {
                        Utils::printError('\'' + last.value + "' is NOT a valid keyword!", line, false);
                        error = true;
                    }
                    else tokens.emplace_back(it->second, last.line);

                    break;
                }
                case '/' :
                    read();

                    if (file.eof() || (curr != '/' && curr != '*')) {
                        Utils::printError(std::string("Badly formatted comment - unexpected character '")
                            + BOLD + curr + R_BOLD + "' after '" + BOLD + '/' + R_BOLD + "'!", line, false);
                        error = true;
                    }

                    ignoreComment(curr == '*');
                    break;
                case '-' :
                    tokens.emplace_back(TokenType::Hyphen, line);
                    break;
                case ' ' :
                case '\n' :
                    break;
                default :
                    Utils::printError(std::string("Unexpected character '") + BOLD + curr + R_BOLD
                        + "'!", line, false);
                    error = true;
            }
        }

        if (error) exit(1);
        return tokens;
    }
}
