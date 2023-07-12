#ifndef HELPY_TOKEN_HPP
#define HELPY_TOKEN_HPP

#include <string>

namespace Helpy {
    enum class TokenType {
        Literal,
        Hyphen,
        LineEnd,

        // keywords
        NameKeyword,
        ArgumentsKeyword,
        CommandsKeyword,
    };

    struct Token {
        TokenType type;
        std::string value;

        /* CONSTRUCTOR */
        explicit Token(TokenType type, std::string value = "") : type(type), value(std::move(value)) {}
    };
}

#endif //HELPY_TOKEN_HPP
