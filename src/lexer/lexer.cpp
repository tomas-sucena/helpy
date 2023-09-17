#include "../utils/utils.hpp"
#include "lexer.h"

namespace Helpy {
    uMap<std::string, TokenType> Lexer::keywords = {{"COLOR", TokenType::ColorKeyword},
                                                    {"COLOUR", TokenType::ColorKeyword},
                                                    {"COMMANDS", TokenType::CommandsKeyword},
                                                    {"DESCRIPTIONS", TokenType::DescriptionsKeyword},
                                                    {"NAME", TokenType::NameKeyword},};

    Lexer::Lexer(const std::string &path) : file(path), curr(0), line(1), error(false), warnings(0) {}

    void Lexer::read() {
        file.get(curr);
        line += (curr == '\n');
    }

    std::string Lexer::readWord() {
        std::string word;
        bool getNext = true;

        while (!file.eof() && getNext) {
            switch (curr) {
                case 'a' ... 'z' :
                case 'A' ... 'Z' :
                case '0' ... '9' :
                    word += curr;
                    break;
                case ':' :
                case '-' :
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

        return word;
    }

    std::string Lexer::readString(char delimiter) {
        std::string string;
        unsigned initialLine = line;

        bool getNext = true;
        bool escape;

        while (getNext) {
            read();

            if (file.eof())
                Utils::printError(std::string("Badly formatted string literal - expected closing '")
                    + BOLD + delimiter + R_BOLD + "'!", initialLine);

            switch (curr) {
                case '\\' :
                    escape ^= true;
                    continue;
                case '\n' :
                    break;
                case 'n' :
                    string += escape ? '\n' : 'n';
                    break;
                case '\'' :
                case '"' :
                case ')' :
                    if (curr == delimiter && !escape) {
                        getNext = false;
                        break;
                    }
                default :
                    string += curr;
                    break;
            }

            escape = false;
        }

        return string;
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

        if (file.eof())
            Utils::printError(std::string("Badly formatted comment - expected closing '") + BOLD + "*/" 
                + R_BOLD + "' to match opening '" + BOLD + "/*" + R_BOLD + "'!", initialLine);
    }

    std::vector<Token> Lexer::execute() {
        std::vector<Token> tokens;
        int getNext = true;

        while (true) {
            if (getNext++) read();

            // check if there is more to read
            if (file.eof()) break;

            switch (curr) {
                case 'a' ... 'z' :
                case 'A' ... 'Z' :
                case '1' ... '9' :
                    tokens.emplace_back(TokenType::Word, line, readWord());
                    getNext = false;

                    break;

                case '\'' :
                case '"' :
                    tokens.emplace_back(TokenType::String, line, readString(curr));
                    break;

                case '(' :
                    tokens.emplace_back(TokenType::String, line, readString(')'));
                    break;

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

                case '-' :
                    tokens.emplace_back(TokenType::Hyphen, line);
                    break;

                case '/' :
                    read();

                    if (curr != '/' && curr != '*') {
                        Utils::printWarning(std::string("Unexpected character '") + BOLD + '/' + R_BOLD
                                            + "'!", line);

                        getNext = false;
                        ++warnings;
                    }
                    else
                        ignoreComment(curr == '*');

                    break;

                case ' ' :
                case '\n' :
                    break;

                default :
                    Utils::printWarning(std::string("Unexpected character '") + BOLD + curr + R_BOLD
                        + "'!", line);
                    ++warnings;
            }
        }

        // check if there were any errors
        if (error) exit(1);
        return tokens;
    }
}
