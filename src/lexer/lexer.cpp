#include "lexer.h"

namespace Helpy {
    Lexer::Lexer(const std::string &path) : file(path), curr(0), line(0) {}

    void Lexer::read() {
        file.get(curr);
        line += (curr == '\n');
    }

    std::string Lexer::readWord() {
        std::string value;

        while (!file.eof() && curr != ' ' && curr != '\n') {
            switch (curr) {
                case 'a' ... 'z' :
                case 'A' ... 'Z' :
                case '0' ... '9' :
                    value += curr;
                case ':' :
                case '\n' :
                    break;
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

        while (!file.eof()) {
            read();

            switch (curr) {
                case '\n' :
                    tokens.emplace_back(TokenType::LineEnd);
                    break;
                case 'a' ... 'z' :
                case 'A' ... 'Z' : {
                    std::string value = readWord();
                    auto it = keywords.find(value);

                    if (it == keywords.end())
                        tokens.emplace_back(TokenType::Literal, value);
                    else
                        tokens.emplace_back(it->second);

                    break;
                }
            }
        }

        return tokens;
    }
}
