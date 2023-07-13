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

        /* METHODS */
        friend std::ostream& operator<<(std::ostream &os, const Token &token) {
            os << "{ type: ";

            switch (token.type) {
                case TokenType::Literal:
                    os << "Literal";
                    break;
                case TokenType::Hyphen:
                    os << "Hyphen";
                    break;
                case TokenType::LineEnd:
                    os << "LineEnd";
                    break;
                case TokenType::NameKeyword:
                    os << "NAME";
                    break;
                case TokenType::ArgumentsKeyword:
                    os << "ARGUMENTS";
                    break;
                case TokenType::CommandsKeyword:
                    os << "COMMANDS";
                    break;
            }

            if (!token.value.empty()) os << ", value: " << token.value;

            os << " }";
            return os;
        }
    };
}

#endif //HELPY_TOKEN_HPP
