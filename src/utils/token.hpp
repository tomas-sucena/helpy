#ifndef HELPY_TOKEN_HPP
#define HELPY_TOKEN_HPP

#include <string>

namespace Helpy {
    /**
     * @brief An enum that details all the types of tokens.
     */
    enum class TokenType {
        Literal, /**< a literal value, such as a number or a string (except keywords) */
        Hyphen, /**< a hyphen */

        // keywords
        NameKeyword, /**< the string 'NAME'*/
        ArgumentsKeyword, /**< the string 'ARGUMENTS'*/
        CommandsKeyword, /**< the string 'COMMANDS'*/
    };

    /**
     * @brief A class that represents a Helpyfile token.
     */
    struct Token {
        TokenType type;
        std::string value;

        /* CONSTRUCTOR */
        /**
         * @brief Creates a token.
         * @param type the type of the token
         * @param value the value (if any) of the token
         */
        explicit Token(TokenType type, std::string value = "") : type(type), value(std::move(value)) {}

        /* METHODS */
        /**
         * @brief Outputs a token to a stream.
         * @param os output stream
         * @param token token to be output
         * @return output stream, after outputting the token
         */
        friend std::ostream& operator<<(std::ostream &os, const Token &token) {
            os << "{ type: ";

            switch (token.type) {
                case TokenType::Literal:
                    os << "Literal";
                    break;
                case TokenType::Hyphen:
                    os << "Hyphen";
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
