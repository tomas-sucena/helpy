#include "../utils/utils.hpp"
#include "lexer.h"

namespace Helpy {
    uMap<std::string, TokenType> Lexer::keywords = {{"COLOR", TokenType::ColorKeyword},
                                                    {"COLOUR", TokenType::ColorKeyword},
                                                    {"COMMANDS", TokenType::CommandsKeyword},
                                                    {"DESCRIPTIONS", TokenType::DescriptionsKeyword},
                                                    {"NAME", TokenType::NameKeyword},};

    Lexer::Lexer(const std::string &path, Program &program)
        : file(path), curr(0), line(1), pos(-1), program(program) {}

    void Lexer::read() {
        file.get(curr);
        program.addChar(curr);

        if (curr == '\n') {
            ++line;
            pos = -1;
        }
        else ++pos;
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
                    program.error = true;
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

            uint16_t initialLine = line;
            uint16_t initialPos = pos;

            switch (curr) {
                case 'a' ... 'z' :
                case 'A' ... 'Z' :
                case '1' ... '9' : {
                    std::string word = readWord();

                    tokens.emplace_back(TokenType::Word, initialLine, initialPos, pos, word);
                    getNext = false;

                    break;

                }

                case '\'' :
                case '"' : {
                    std::string string = readString(curr);
                    tokens.emplace_back(TokenType::String, initialLine, initialPos, pos, string);

                    break;
                }

                case '(' : {
                    std::string string = readString(')');
                    tokens.emplace_back(TokenType::String, initialLine, initialPos, pos, string);

                    break;
                }

                case ':' : {
                    Token last = tokens.back();
                    tokens.pop_back();

                    auto it = keywords.find(last.value);

                    if (it == keywords.end()) {
                        Utils::printError('\'' + last.value + "' is NOT a valid keyword!", line, false);
                        program.error = true;
                    }
                    else
                        tokens.emplace_back(it->second, last.line, last.start, pos);

                    break;
                }

                case '-' :
                    tokens.emplace_back(TokenType::Hyphen, line, pos, pos);
                    break;

                case '/' :
                    read();

                    if (curr != '/' && curr != '*') {
                        Utils::printWarning(std::string("Unexpected character '") + BOLD + '/' + R_BOLD
                                            + "'!", line);

                        getNext = false;
                        ++program.warnings;
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
                    ++program.warnings;
            }
        }

        return tokens;
    }
}
