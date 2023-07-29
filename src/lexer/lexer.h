#ifndef HELPY_LEXER_H
#define HELPY_LEXER_H

#include <fstream>
#include <list>
#include <unordered_map>

#include "../utils/token.hpp"

#define uMap std::unordered_map

namespace Helpy {
    class Lexer {
        std::ifstream file;
        char curr;
        int line;
        bool error;

        static uMap<std::string, TokenType> keywords;

    /* CONSTRUCTOR */
    public:
        explicit Lexer(const std::string &path);

    /* METHODS */
    private:
        void read();
        std::string readWord();
        void ignoreComment(bool multiline);

    public:
        std::list<Token> execute();
    };
}

#endif //HELPY_LEXER_H
