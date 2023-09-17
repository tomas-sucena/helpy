#ifndef HELPY_LEXER_H
#define HELPY_LEXER_H

#include <fstream>
#include <unordered_map>
#include <vector>

#include "../utils/program.hpp"
#include "../utils/token.hpp"

#define uMap std::unordered_map

namespace Helpy {
    class Lexer {
        std::ifstream file;
        char curr;
        uint16_t line, pos;
        Program &program;

        static uMap<std::string, TokenType> keywords;

    /* CONSTRUCTOR */
    public:
        explicit Lexer(const std::string &path, Program &program);

    /* METHODS */
    private:
        void read();
        std::string readWord();
        std::string readString(char delimiter);
        void ignoreComment(bool multiline);

    public:
        std::vector<Token> execute();
    };
}

#endif //HELPY_LEXER_H
