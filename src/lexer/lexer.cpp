#include "lexer.h"

namespace Helpy {
    uMap<std::string, TokenType> Lexer::keywords = {{"NAME", TokenType::NameKeyword},
                                                    {"ARGUMENTS", TokenType::ArgumentsKeyword},
                                                    {"COMMANDS", TokenType::CommandsKeyword},
                                                    {"COLOR", TokenType::ColorKeyword},
                                                    {"COLOUR", TokenType::ColorKeyword}};

    Lexer::Lexer(const std::string &path) : file(path), curr(0), line(0) {}

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
                    throw std::runtime_error("Error in line " + std::to_string(line) + ": Unexpected character '"
                        + curr + "'!");
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

        std::string initialLine = std::to_string(line);

        while (!file.eof()) {
            read();
            if (file.eof() || curr != '*') continue;

            read();
            if (file.eof() || curr != '/') continue;

            // "*/" found
            break;
        }

        if (file.eof())
            throw std::runtime_error("Error in line " + initialLine + ": Badly formatted comment - expected closing"
                " '*/' to match opening '/*'!");
    }

    std::list<Token> Lexer::execute() {
        std::list<Token> tokens;
        int getNext = true;

        while (!file.eof()) {
            if (getNext++) read();

            switch (curr) {
                case 'a' ... 'z' :
                case 'A' ... 'Z' :
                case '1' ... '9' :
                    tokens.emplace_back(TokenType::Literal, readWord());
                    getNext = false;

                    break;
                case ':' : {
                    Token last = tokens.back();
                    tokens.pop_back();

                    auto it = keywords.find(last.value);

                    if (it == keywords.end())
                        throw std::runtime_error("Error in line " + std::to_string(line) + ": '" + last.value
                            + "' is NOT a valid keyword!");

                    tokens.emplace_back(it->second);
                    break;
                }
                case '/' :
                    read();

                    if (file.eof() || (curr != '/' && curr != '*'))
                        throw std::runtime_error("Error in line " + std::to_string(line) + ": Badly formatted comment - "
                            "unexpected character '" + curr + "' after '/'!");

                    ignoreComment(curr == '*');
                    break;
                case '-' :
                    tokens.emplace_back(TokenType::Hyphen);
                    break;
                case ' ' :
                case '\n' :
                    break;
                default :
                    throw std::runtime_error("Error in line " + std::to_string(line) + ": Unexpected character '"
                        + curr + "'!");
            }
        }

        return tokens;
    }
}
