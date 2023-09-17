#ifndef HELPY_LEXER_H
#define HELPY_LEXER_H

#include <fstream>
#include <unordered_map>
#include <vector>

#include "../utils/token.hpp"

#define uMap std::unordered_map

namespace Helpy {
    class Lexer {
        std::ifstream file;
        char curr;
        uint16_t line;
        bool error;

        static uMap<std::string, TokenType> keywords;

    /* CONSTRUCTOR */
    public:
        explicit Lexer(const std::string &path);

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
